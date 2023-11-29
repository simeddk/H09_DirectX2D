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
	Vector2 position = Vector2(0, 0);				//���콺 ��ǥ(����Ʈ ���� ��ǥ)
	BYTE buttonStatus[MAX_MOUSE_INPUT];				//Ư�� ��ư�� ������ �ش� �ε����� 1, �ƴϸ� 0
	BYTE buttonPrevStatus[MAX_MOUSE_INPUT];			//Up, Down ���� �Ǵ��ϱ� ����
	EButtonStateType buttonMap[MAX_MOUSE_INPUT];	//���� ����

	DWORD doubleClickTime;							//���� ��� �ð�(OS���� ������)
	DWORD startDoubleClickTime[MAX_MOUSE_INPUT];	//ù��° ��ư�� Ŭ���� �ð�
	int buttonCount[MAX_MOUSE_INPUT];				//��ư�� �� �� ���ȴ���

private:
	Vector3 wheelStatus = Vector3(0, 0, 0);			//xy:���콺 ��ǥ(����Ʈ �ٱ� ��ǥ), z:�ٰ�
	Vector3 wheelOldStatus = Vector3(0, 0, 0);		//���� �������� ���콺 ��ǥ
	Vector3 wheelMoveValue = Vector3(0, 0, 0);		//xy:���콺 ��ǥ ��Ÿ ����, z:��Ÿ �ٰ�
};