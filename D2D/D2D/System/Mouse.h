#pragma once

#define MAX_MOUSE_INPUT 8
enum class EButtonStateType { None, Down, Up, Press, DoubleClick };
enum EMouseButton { LButton, RButton, MButton };

class CMouse
{
public:
	CMouse();
	~CMouse() = default;

	void Update();

	void WndProc(UINT message, WPARAM wParam, LPARAM lparam);

public:
	Vector2 Position_OnViewport() { return position; }
	Vector2 Position_OnClient() { return Vector2(wheelStatus.x, wheelStatus.y); }
	float Wheel() { return wheelStatus.z; }

	Vector2 Position_Delta() { return Vector2(wheelMoveValue.x, wheelMoveValue.y); }
	float Wheel_Delta() { return wheelMoveValue.z; }

public:
	bool Down(EMouseButton button) { return buttonMap[(int)button] == EButtonStateType::Down; }
	bool Up(EMouseButton button) { return buttonMap[(int)button] == EButtonStateType::Up; }
	bool Press(EMouseButton button) { return buttonMap[(int)button] == EButtonStateType::Press; }
	bool DoubleClick(EMouseButton button) { return buttonMap[(int)button] == EButtonStateType::DoubleClick; }

private:
	Vector2 position = Vector2(0, 0);				//마우스 좌표(뷰포트 내부 좌표)
	BYTE buttonStatus[MAX_MOUSE_INPUT];				//특정 버튼이 눌리면 해당 인덱스에 1, 아니면 0
	BYTE buttonPrevStatus[MAX_MOUSE_INPUT];			//Up, Down 등을 판단하기 위해
	EButtonStateType buttonMap[MAX_MOUSE_INPUT];	//최종 상태

	DWORD doubleClickTime;							//더블 허용 시간(OS에서 지정한)
	DWORD startDoubleClickTime[MAX_MOUSE_INPUT];	//첫번째 버튼을 클릭한 시간
	int buttonCount[MAX_MOUSE_INPUT];				//버튼이 몇 번 눌렸는지

private:
	Vector3 wheelStatus = Vector3(0, 0, 0);			//xy:마우스 좌표(뷰포트 바깥 좌표), z:휠값
	Vector3 wheelOldStatus = Vector3(0, 0, 0);		//이전 프레임의 마우스 좌표
	Vector3 wheelMoveValue = Vector3(0, 0, 0);		//xy:마우스 좌표 델타 차이, z:델타 휠값
};