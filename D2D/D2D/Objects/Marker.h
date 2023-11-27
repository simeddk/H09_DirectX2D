#pragma once

class Marker
{
public:
	Marker(Shader* shader, Vector2 position);
	~Marker();

	void Update();
	void Render();

	Vector2 Position() { return position; }

private:
	Clip* clip = nullptr;

	Vector2 position;
};