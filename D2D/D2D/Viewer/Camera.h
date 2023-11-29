#pragma once

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update();

	Matrix View() { return view; }

	void Position(float x, float y);
	void Position(Vector2& vec);
	Vector2 Position() { return position; }

	float& Depth() { return depth; }

protected:
	Vector2 position = Vector2(0, 0);
	float depth = 0.f;

private:
	Matrix view;

};