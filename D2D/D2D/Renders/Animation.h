#pragma once

class Animation
{
public:
	Animation();
	~Animation();

	void Update();
	void Render();

public:
	void AddClip(Clip* clip);
	void Play(UINT clipIndex);
	void Stop();

	void Position(float x, float y);
	void Position(Vector2& vec);
	Vector2 Position() { return position; }

	void Scale(float x, float y);
	void Scale(Vector2& vec);
	Vector2 Scale() { return scale; }

	void Rotation(float x, float y, float z);
	void Rotation(Vector3& vec);
	Vector3 Rotation() { return rotation; }

	void RotationDegree(float x, float y, float z);
	void RotationDegree(Vector3& vec);
	Vector3 RotationDegree();

	Vector2 ScaledTextureSize();
	Clip* GetCurrentClip();
	Sprite* GetCurrentFrame();

private:
	Vector2 position = Vector2(0, 0);
	Vector2 scale = Vector2(1, 1);
	Vector3 rotation = Vector3(0, 0, 0);

	vector<Clip*> clips;
	int currentClip = -1;
};