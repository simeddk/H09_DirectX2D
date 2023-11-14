#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet(Shader* shader, Vector2 start, float degree, float speed)
	: position(start)
{
	sprite = new Sprite(shader, L"bullet.png", 2, 120, 22, 143);
	sprite->Position(position);
	sprite->Scale(sprite->GetTextureSize());

	Vector2 direction;
	float radian = Math::ToRadian(degree);
	direction.x = cos(radian);
	direction.y = sin(radian);

	velocity = direction * speed;
}

Bullet::~Bullet()
{
	SafeDelete(sprite);
}

void Bullet::Update()
{
	position += velocity * Time::Delta();
	sprite->Position(position);
}

void Bullet::Render()
{
	sprite->Render();
}
