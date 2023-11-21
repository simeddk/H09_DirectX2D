#pragma once

class Freedom : public Camera
{
public:
	Freedom(float speed = 200.f);
	~Freedom();

	void Update() override;

	void Speed(float val) { speed = val; }

private:
	void Move(Vector2 direction);

private:
	float speed;
};