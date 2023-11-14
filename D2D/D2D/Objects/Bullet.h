#pragma once

class Bullet
{
public:
	Bullet(Shader* shader, Vector2 start, float degree, float speed);
	~Bullet();

	void Update();
	void Render();

	Vector2 Position() { return position; }

private:
	Sprite* sprite;

	Vector2 position;
	Vector2 velocity;

};