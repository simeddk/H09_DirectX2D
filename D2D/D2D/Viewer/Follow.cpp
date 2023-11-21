#include "stdafx.h"
#include "Follow.h"

Follow::Follow(IFocus* focusObject)
	: focusObject(focusObject)
{
}

Follow::~Follow()
{
}

void Follow::Update()
{
	CheckNull(focusObject);

	Vector2 outLocation;
	focusObject->Focus(&outLocation);

	outLocation.x -= (float)Width * 0.5f;
	outLocation.y -= (float)Height * 0.5f;

	position = outLocation;

	__super::Update();
}

void Follow::SetFocusObject(IFocus* newFocusObject)
{
	focusObject = newFocusObject;
}
