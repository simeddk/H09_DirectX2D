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
	//임구이 위에 마우스 있으면 리턴
	CheckTrue(ImGui::IsAnyItemHovered() || ImGui::IsAnyWindowHovered());

	//이전 업데이트 때 넣었던 버튼 클릭 상태를 prev 배열로 복사
	memcpy(buttonPrevStatus, buttonStatus, sizeof(BYTE) * MAX_MOUSE_INPUT);

	//검사를 하기 전에 이전 상태를 리셋
	ZeroMemory(buttonStatus, sizeof(BYTE) * MAX_MOUSE_INPUT);
	ZeroMemory(buttonMap, sizeof(EButtonStateType) * MAX_MOUSE_INPUT);

	//현재 버튼 클릭 상태 저장
	buttonStatus[(int)LButton] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[(int)RButton] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[(int)MButton] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	//prev 와 current(buttonStatus)를 비교해서 최종적으로 Up, Down등을 판단
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

	//뷰포트 바깥 좌표 저장 & 델타 위치 & 델타 휠
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(Hwnd, &point);

	wheelOldStatus.x = wheelStatus.x;
	wheelOldStatus.y = wheelStatus.y;

	wheelStatus.x = (float)point.x;
	wheelStatus.y = (float)point.y;

	wheelMoveValue = wheelStatus - wheelOldStatus;

	//더블 클릭
	UINT time = GetTickCount();
	for (int i = 0; i < MAX_MOUSE_INPUT; i++)
	{
		//버튼 클릭
		if (buttonMap[i] == EButtonStateType::Down)
		{
			//이미 한번 눌린 적이 있다
			if (buttonCount[i] == 1)
			{
				//첫번째 누른 시간과 현재 누른 시간이 허용 시간을 넘어가면 => 실패
				if (time - startDoubleClickTime[i] >= doubleClickTime)
					buttonCount[i] = 0;
			}

			//버튼 누른 회수 증가
			buttonCount[i]++;

			//눌렀을 때 당시의 시간을 저장
			if (buttonCount[i] == 1)
				startDoubleClickTime[i] = time;
		}//if(down)

		//버튼을 뗐을 때
		if (buttonMap[i] == EButtonStateType::Up)
		{
			//허용 시간 내에 두번을 눌렀다
			if (buttonCount[i] >= 2)
			{
				//버튼을 허용 시간 이후로 너무 늦게 뗐을 때
				if (time - startDoubleClickTime[i] > doubleClickTime)
					buttonCount[i] = 0;

				//성공 조건
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
