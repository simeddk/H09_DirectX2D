#pragma once

enum class EPlayType
{
	OnceKeepFirstFrame,
	OnceKeepLastFrame,
	Loop
};

//-----------------------------------------------------------------------------
// Frame
//-----------------------------------------------------------------------------
struct Frame
{
	Frame(Sprite* Image, float Time);
	~Frame();

	Sprite* Image;
	float Time;
};

//-----------------------------------------------------------------------------
// Clip
//-----------------------------------------------------------------------------
class Clip
{
public:
	Clip(EPlayType playType = EPlayType::OnceKeepLastFrame, float playRate = 1.f);
	~Clip();

	void Update();
	void Render();

public:
	void AddFrame(Sprite* sprite, float time);

	void Play();
	void Play(UINT startFrame);
	void Stop();

	Vector2 ScaledTextureSize();
	Sprite* GetCurrentFrameAsSprite();
	UINT GetCurrentFrameAsIndex() { return currentFrame; }

public:
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

	void PlayRate(float val) { playRate = val; }

private:
	float playRate = 1.f;
	float playTime = 0.f;
	UINT currentFrame = 0;
	bool bPlaying = false;

	EPlayType playType;
	vector<Frame*> frames;

	Vector2 position = Vector2(0, 0);
	Vector2 scale = Vector2(1, 1);
	Vector3 rotation = Vector3(0, 0, 0);
};