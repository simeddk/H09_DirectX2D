#include "stdafx.h"
#include "Keyboard.h"

Keyboard::Keyboard()
{
	upList.reset();
	downList.reset();
}

Keyboard::~Keyboard()
{
}

bool Keyboard::Down(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (downList[key] == false)
		{
			downList[key] = true;
			return true;
		}
	}
	else
	{
		downList[key] = false;
	}

	return false;
}

bool Keyboard::Up(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		upList[key] = true;
	}
	else
	{
		if (upList[key] == true)
		{
			upList[key] = false;
			return true;
		}
	}

	return false;
}

bool Keyboard::Press(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		return true;

	return false;
}

bool Keyboard::Toggle(int key)
{
	if (GetAsyncKeyState(key) & 0x0001)
		return true;

	return false;
}
