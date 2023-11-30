#include "stdafx.h"
#include "Freedom.h"

Freedom::Freedom(float speed)
	: speed(speed)
{
}

Freedom::~Freedom()
{
}

void Freedom::Update()
{
	/*if (Key->Press(VK_RIGHT))
		Move(Vector2(+1, 0));
	else if (Key->Press(VK_LEFT))
		Move(Vector2(-1, 0));

	if (Key->Press(VK_UP))
		Move(Vector2(0, +1));
	else if (Key->Press(VK_DOWN))
		Move(Vector2(0, -1));*/

	if (Mouse->Press(RButton))
	{
		Vector2 mouseDelta = Vector2
		(
			Mouse->Position_Delta().x * -1.f,
			Mouse->Position_Delta().y
		);

		Move(mouseDelta);
	}

	__super::Update();
}

void Freedom::Move(Vector2 direction)
{
	position += direction * speed * Time::Delta();
}
