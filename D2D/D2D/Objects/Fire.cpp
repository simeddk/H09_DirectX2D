#include "stdafx.h"
#include "Fire.h"

Fire::Fire(Shader* shader, Vector2 position, Vector2 scale)
	: position(position)
	, scale(scale)
{
	clip = new Clip(EPlayType::Loop);

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			float startX = (float)x * 43.f;
			float startY = (float)y * 67.f;
			float endX = (float)(x + 1) * 43.f;
			float endY = (float)(y + 1) * 67.f;

			clip->AddFrame(new Sprite(shader, L"FireSprite.png", startX, startY, endX, endY), 0.075f);;
		}
	}
	

	clip->Position(position);
	clip->Scale(scale);
	clip->Play();

	collider = new Collider();
}

Fire::~Fire()
{
	SafeDelete(collider);
	SafeDelete(clip);
}

void Fire::Update()
{
	clip->Position(position);
	clip->Scale(scale);

	clip->Update();
	collider->Update(GetWorld());
}

void Fire::Render()
{
	clip->Render();
	collider->Render();
}

Matrix& Fire::GetWorld()
{
	return clip->GetCurrentFrameAsSprite()->GetWorldMatrix();
}

void Fire::Play()
{
	clip->Play();
}

void Fire::Stop()
{
	clip->Stop();
}
