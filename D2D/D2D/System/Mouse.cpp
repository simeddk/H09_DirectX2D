#include "stdafx.h"
#include "Mouse.h"

CMouse::CMouse()
{
	ZeroMemory(buttonStatus, sizeof(BYTE) * MAX_MOUSE_INPUT);
	ZeroMemory(buttonPrevStatus, sizeof(BYTE) * MAX_MOUSE_INPUT);
	ZeroMemory(buttonMap, sizeof(EButtonStateType) * MAX_MOUSE_INPUT);
	ZeroMemory(buttonCount, sizeof(int) * MAX_MOUSE_INPUT);

	doubleClickTime = GetDoubleClickTime();
	startDoubleClickTime[0] = GetTickCount();

	for (int i = 1; i < MAX_MOUSE_INPUT; i++)
		startDoubleClickTime[i] = startDoubleClickTime[0];
}

void CMouse::Update()
{
	//�ӱ��� ���� ���콺 ������ ����
	CheckTrue(ImGui::IsAnyItemHovered() || ImGui::IsAnyWindowHovered());

	//���� ������Ʈ �� �־��� ��ư Ŭ�� ���¸� prev �迭�� ����
	memcpy(buttonPrevStatus, buttonStatus, sizeof(BYTE) * MAX_MOUSE_INPUT);

	//�˻縦 �ϱ� ���� ���� ���¸� ����
	ZeroMemory(buttonStatus, sizeof(BYTE) * MAX_MOUSE_INPUT);
	ZeroMemory(buttonMap, sizeof(EButtonStateType) * MAX_MOUSE_INPUT);

	//���� ��ư Ŭ�� ���� ����
	buttonStatus[(int)LButton] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[(int)RButton] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[(int)MButton] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	//prev �� current(buttonStatus)�� ���ؼ� ���������� Up, Down���� �Ǵ�
	for (int i = 0; i < MAX_MOUSE_INPUT; i++)
	{
		BYTE prev = buttonPrevStatus[i];
		BYTE current = buttonStatus[i];

		if (prev == 0 && current == 1)
			buttonMap[i] = EButtonStateType::Down;
		else if (prev == 1 && current == 0)
			buttonMap[i] = EButtonStateType::Up;
		else if (prev == 1 && current == 1)
			buttonMap[i] = EButtonStateType::Press;
		else
			buttonMap[i] = EButtonStateType::None;
	}

	//����Ʈ �ٱ� ��ǥ ���� & ��Ÿ ��ġ & ��Ÿ ��
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(Hwnd, &point);

	wheelOldStatus.x = wheelStatus.x;
	wheelOldStatus.y = wheelStatus.y;

	wheelStatus.x = (float)point.x;
	wheelStatus.y = (float)point.y;

	wheelMoveValue = wheelStatus - wheelOldStatus;

	//���� Ŭ��
	UINT time = GetTickCount();
	for (int i = 0; i < MAX_MOUSE_INPUT; i++)
	{
		//��ư Ŭ��
		if (buttonMap[i] == EButtonStateType::Down)
		{
			//�̹� �ѹ� ���� ���� �ִ�
			if (buttonCount[i] == 1)
			{
				//ù��° ���� �ð��� ���� ���� �ð��� ��� �ð��� �Ѿ�� => ����
				if (time - startDoubleClickTime[i] >= doubleClickTime)
					buttonCount[i] = 0;
			}

			//��ư ���� ȸ�� ����
			buttonCount[i]++;

			//������ �� ����� �ð��� ����
			if (buttonCount[i] == 1)
				startDoubleClickTime[i] = time;
		}//if(down)

		//��ư�� ���� ��
		if (buttonMap[i] == EButtonStateType::Up)
		{
			//��� �ð� ���� �ι��� ������
			if (buttonCount[i] >= 2)
			{
				//��ư�� ��� �ð� ���ķ� �ʹ� �ʰ� ���� ��
				if (time - startDoubleClickTime[i] > doubleClickTime)
					buttonCount[i] = 0;

				//���� ����
				else if (time - startDoubleClickTime[i] <= doubleClickTime)
				{
					buttonMap[i] = EButtonStateType::DoubleClick;
					buttonCount[i] = 0;
				}
			}
		}//if(up)

	}//for
}

void CMouse::WndProc(UINT message, WPARAM wParam, LPARAM lparam)
{
	if (message == WM_MOUSEMOVE)
	{
		position.x = (float)LOWORD(lparam);
		position.y = (float)HIWORD(lparam);
	}

	if (message == WM_MOUSEWHEEL)
	{
		 short wheelValue = (short)HIWORD(wParam);

		 wheelOldStatus.z = wheelStatus.z;
		 wheelStatus.z += (float)wheelValue;
	}
}

Vector2 CMouse::Position_World()
{
	//viewport -> ratio(0~1)
	Vector2 ratio;
	ratio.x = position.x / (float)Width;
	ratio.y = position.y / (float)Height;

	//ratio -> ndc(-1~1)
	Vector2 ndc;
	ndc.x = ratio.x * 2.f - 1.f;
	ndc.y = (ratio.y * 2.f - 1.f) * -1.f;

	//(V * P)[-1]
	Matrix vp = Context::Get()->GetCamera()->View() * Context::Get()->Projection();
	D3DXMatrixInverse(&vp, nullptr, &vp);

	//ndc * (V * P)[-1] == ndc / (V * P)
	Vector2 world;
	D3DXVec2TransformCoord(&world, &ndc, &vp);
	ImGui::Text("%.2f, %.2f", world.x, world.y);
	//viewport(SS) = W * V * P
	//W = viewport(SS) / VP

	return world;
}
