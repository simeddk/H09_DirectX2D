#include "stdafx.h"
#include "Marco.h"

Marco::Marco(Shader* shader, Vector2 position, Vector2 scale)
{
	animation = new Animation();
	Clip* clip = nullptr;

	perFrame = new PerFrame(shader);
	collider = new Collider();
	gizmo = new Gizmo();

	//Clip 0 - Idle
	{
		clip = new Clip(EPlayType::Loop);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 4, 2, 34, 40), 0.22f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 35, 2, 64, 40), 0.22f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 64, 2, 94, 40), 0.22f);

		animation->AddClip(clip);
	}

	//Clip 1 - Run
	{
		clip = new Clip(EPlayType::Loop);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 0, 600, 32, 640), 0.1f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 33, 600, 64, 640), 0.1f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 65, 600, 96, 640), 0.1f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 97, 600, 124, 640), 0.1f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 125, 600, 154, 640), 0.1f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 158, 600, 188, 640), 0.1f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 191, 600, 222, 640), 0.1f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 224, 599, 258, 640), 0.1f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 259, 600, 294, 640), 0.1f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 295, 600, 326, 640), 0.1f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 329, 600, 360, 640), 0.1f);
		clip->AddFrame(new Sprite(shader, L"Tarma.png", 362, 600, 393, 640), 0.1f);

		animation->AddClip(clip);
	}

	animation->Position(position);
	animation->Scale(scale);
	animation->Play((UINT)EStateType::Idle);
}

Marco::~Marco()
{
	SafeDelete(animation);
	SafeDelete(perFrame);
	SafeDelete(collider);
	SafeDelete(gizmo);
}

void Marco::Update()
{
	Vector2 position = animation->Position();
	bool bMove = false;

	if (Key->Press('D'))
	{
		bMove = true;
		position.x += speed * Time::Delta();
		animation->Rotation(0, 0, 0);
	}
	else if (Key->Press('A'))
	{
		bMove = true;
		position.x -= speed * Time::Delta();
		animation->Rotation(0, Math::PI, 0);
	}
	
	if (Key->Press('W'))
	{
		bMove = true;
		position.y += speed * Time::Delta();
	}
	else if (Key->Press('S'))
	{
		bMove = true;
		position.y -= speed * Time::Delta();
	}
	animation->Position(position);

	EStateType currentState = bMove ? EStateType::Run : EStateType::Idle;
	animation->Play((UINT)currentState);

	perFrame->Update();
	animation->Update();
	collider->Update(GetWorld());
}

void Marco::Render()
{
	perFrame->Render();
	animation->Render();
	collider->Render();

	CheckFalse(bGizmo);
	Vector3 r;
	Vector2 s, t;
	gizmo->Set(animation->GetCurrentFrame(), &s, &r, &t);

	animation->Scale(s);
	animation->Rotation(r);
	animation->Position(t);
}

void Marco::Focus(OUT Vector2* focusPosition)
{
	*focusPosition = animation->Position() + focusOffset;
}

Sprite* Marco::GetSprite()
{
	return animation->GetCurrentFrame();
}

Matrix& Marco::GetWorld()
{
	return animation->GetCurrentFrame()->GetWorldMatrix();
}

void Marco::Position(Vector2 vec)
{
	animation->Position(vec);
}

Vector2 Marco::Position()
{
	return animation->Position();
}

void Marco::Scale(Vector2 vec)
{
	animation->Scale(vec);
}

Vector2 Marco::Scale()
{
	return animation->Scale();
}

void Marco::Rotation(Vector3 vec)
{
	animation->Rotation(vec);
}

Vector3 Marco::Rotation()
{
	return animation->Rotation();
}
