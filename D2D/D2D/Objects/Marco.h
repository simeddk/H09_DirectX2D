#pragma once

#include "Viewer/IFocus.h"

enum class EStateType { Idle, Run };

class Marco : public IFocus
{
public:
	Marco(Shader* shader, Vector2 position, Vector2 scale);
	~Marco();

	void Update();
	void Render();

	virtual void Focus(OUT Vector2* focusPosition) override;

	Collider* GetCollider() { return collider; }
	Matrix& GetWorld();

private:
	Animation* animation = nullptr;
	float speed = 200.f;

	Vector2 focusOffset = Vector2(180, 140);

	PerFrame* perFrame = nullptr;
	Collider* collider = nullptr;
};
