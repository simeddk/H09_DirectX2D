#include "stdafx.h"
#include "MovableRect.h"

MovableRect::MovableRect(Shader* shader)
	: Rect(shader)
{
}

MovableRect::MovableRect(Shader* shader, Vector2 position, Vector2 scale, D3DXCOLOR color)
	: Rect(shader, position, scale, color)
{
}

void MovableRect::Update(Matrix& V, Matrix& P)
{
	__super::Update(V, P);

	Vector2 position = Position();

	//SpaceBar is Pressed
	if (bGround == false)
	{
		jumpVelocity -= gravity * Time::Delta();
		position.y += jumpVelocity;

		Position(position);
	}

	//On Ground
	if (position.y <= Scale().y * 0.5f)
	{
		position.y = Scale().y * 0.5f;
		Position(position);

		jumpVelocity = 0.f;
		bGround = true;
	}

	if (ImGui::CollapsingHeader("MovableRect"))
	{
		ImGui::SliderFloat("Jump Velocity", &jumpVelocity, 0.0001f, 0.5f);
		ImGui::TextColored
		(
			jumpVelocity > 0.f ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1),
			"Vecto%f", jumpVelocity
		);
		ImGui::LabelText("Position.Y", "%.2f", position.y);
	}
	
}

void MovableRect::MoveRight()
{
	Vector2 position = Position();
	position.x += moveSpeed * Time::Delta();
	Position(position);
}

void MovableRect::MoveLeft()
{
	Vector2 position = Position();
	position.x -= moveSpeed * Time::Delta();
	Position(position);
}

void MovableRect::StartJump()
{
	if (bGround == true)
	{
		jumpVelocity = 0.35f;
		bGround = false;
	}
}

void MovableRect::EndJump()
{
	if (jumpVelocity > D3DX_16F_EPSILON)
		jumpVelocity = D3DX_16F_EPSILON;
}
