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
	//wheelOldStatus.z = wheelStatus.z;

	//���� Ŭ��

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
		 WORD wheelValue = HIWORD(wParam);

		 wheelOldStatus.z = wheelStatus.z;
		 wheelStatus.z += (float)wheelValue;
	}
}
