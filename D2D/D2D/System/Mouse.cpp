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
	//wheelOldStatus.z = wheelStatus.z;

	//더블 클릭

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
