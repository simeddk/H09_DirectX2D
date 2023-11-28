#pragma once

class Fire
{
public:
	Fire(Shader* shader, Vector2 position, Vector2 scale);
	~Fire();

	void Update();
	void Render();

	Vector2& Position() { return position; }
	Vector2& Scale() { return scale; }

	Collider* GetCollider() { return collider; }
	Matrix& GetWorld();

	void Play();
	void Stop();

private:
	Clip* clip = nullptr;

	Vector2 position;
	Vector2 scale;

	Collider* collider = nullptr;
	Gizmo* gizmo = nullptr;
};
