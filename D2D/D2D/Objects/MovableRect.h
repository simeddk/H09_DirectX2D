#pragma once

#include "Rect.h"

class MovableRect : public Rect
{
public:
	MovableRect(Shader* shader);
	MovableRect(Shader* shader, Vector2 position, Vector2 scale, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1));

	virtual void Update() override;

	void MoveRight();
	void MoveLeft();

	void StartJump();
	void EndJump();

	void MoveSpeed(float val) { moveSpeed = val; }
	float MoveSpeed() { return moveSpeed; }

private:
	float moveSpeed = 600.f;

	float jumpVelocity = 0.f;
	float gravity = 0.98f;
	bool bGround = true;
};