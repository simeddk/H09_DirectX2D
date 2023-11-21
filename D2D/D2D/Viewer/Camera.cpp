#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	D3DXMatrixIdentity(&view);
}

Camera::~Camera()
{
}

void Camera::Update()
{
	Vector3 eye(position.x, position.y, 0.f);
	Vector3 at(0, 0, 1);
	Vector3 up(0, 1, 0);

	D3DXMatrixLookAtLH(&view, &eye, &(eye + at), &up);
}

void Camera::Position(float x, float y)
{
	Position(Vector2(x, y));
}

void Camera::Position(Vector2& vec)
{
	position = vec;
}
