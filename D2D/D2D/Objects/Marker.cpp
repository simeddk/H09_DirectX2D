#include "stdafx.h"
#include "Marker.h"

Marker::Marker(Shader* shader, Vector2 position)
	: position(position)
{
	clip = new Clip(EPlayType::Loop);
	clip->AddFrame(new Sprite(shader, L"Bullet.png", 37, 2, 55, 18), 0.3f);
	clip->AddFrame(new Sprite(shader, L"Bullet.png", 117, 7, 132, 22), 0.3f);
	clip->AddFrame(new Sprite(shader, L"Bullet.png", 37, 116, 53, 133), 0.3f);
	clip->AddFrame(new Sprite(shader, L"Bullet.png", 204, 109, 220, 126), 0.3f);
	clip->Play();

	clip->Position(this->position);
}

Marker::~Marker()
{
	SafeDelete(clip);
}

void Marker::Update()
{
	if (Key->Press('I'))
		position.y += 200.f * Time::Delta();
	else if (Key->Press('K'))
		position.y -= 200.f * Time::Delta();

	if (Key->Press('L'))
		position.x += 200.f * Time::Delta();
	else if (Key->Press('J'))
		position.x -= 200.f * Time::Delta();

	clip->Position(position);
	clip->Update();
}

void Marker::Render()
{
	clip->Render();
}
