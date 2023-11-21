#include "stdafx.h"
#include "Animation.h"

Animation::Animation()
{
}

Animation::~Animation()
{
	for (Clip* clip : clips)
		SafeDelete(clip);
}

void Animation::Update()
{
	CheckTrue(currentClip < 0);

	clips[currentClip]->Position(position);
	clips[currentClip]->Rotation(rotation);
	clips[currentClip]->Scale(scale);

	clips[currentClip]->Update();
}

void Animation::Render()
{
	CheckTrue(currentClip < 0);

	clips[currentClip]->Render();
}

void Animation::AddClip(Clip* clip)
{
	clips.push_back(clip);
}

void Animation::Play(UINT clipIndex)
{
	CheckTrue(clipIndex == currentClip);

	if (clipIndex > -1)
		clips[currentClip]->Stop();

	currentClip = clipIndex;
	clips[currentClip]->Play();
}

void Animation::Stop()
{
	CheckFale(currentClip > -1);
	clips[currentClip]->Stop();
}

void Animation::Position(float x, float y)
{
	Position(Vector2(x, y));
}

void Animation::Position(Vector2& vec)
{
	position = vec;
}

void Animation::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}

void Animation::Scale(Vector2& vec)
{
	scale = vec;
}

void Animation::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}

void Animation::Rotation(Vector3& vec)
{
	rotation = vec;
}

void Animation::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void Animation::RotationDegree(Vector3& vec)
{
	vec.x = Math::ToRadian(vec.x);
	vec.y = Math::ToRadian(vec.y);
	vec.z = Math::ToRadian(vec.z);

	Rotation(vec);
}

Vector3 Animation::RotationDegree()
{
	Vector3 degree = rotation;
	degree.x = Math::ToDegree(degree.x);
	degree.y = Math::ToDegree(degree.y);
	degree.z = Math::ToDegree(degree.z);

	return degree;
}

Vector2 Animation::ScaledTextureSize()
{
	assert(currentClip > -1);

	return clips[currentClip]->ScaledTextureSize();
}

Clip* Animation::GetCurrentClip()
{
	assert(currentClip > -1);

	return clips[currentClip];
}

Sprite* Animation::GetCurrentFrame()
{
	return GetCurrentClip()->GetCurrentFrameAsSprite();
}
