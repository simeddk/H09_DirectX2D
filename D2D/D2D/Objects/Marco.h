#pragma once

enum class EStateType { Idle, Run };

class Marco
{
public:
	Marco(Shader* shader, Vector2 position, Vector2 scale);
	~Marco();

	void Update();
	void Render();

private:
	Animation* animation = nullptr;
	float speed = 200.f;
};
