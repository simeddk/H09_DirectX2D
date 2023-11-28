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
	Sprite* GetSprite();
	Matrix& GetWorld();

	void Position(Vector2 vec);
	Vector2 Position();

	void Scale(Vector2 vec);
	Vector2 Scale();

	void Rotation(Vector3 vec);
	Vector3 Rotation();

private:
	Animation* animation = nullptr;
	float speed = 200.f;

	Vector2 focusOffset = Vector2(180, 140);

	PerFrame* perFrame = nullptr;
	Collider* collider = nullptr;
};
