#include "stdafx.h"
#include "System/Device.h"
#include "Objects/Background.h"
#include "Objects/Bullet.h"

Shader* shader = nullptr;
Background* background = nullptr;
Clip* clip = nullptr;
Animation* animation = nullptr;

void InitScene()
{
	shader = new Shader(L"04_Sprite.fx");
	PerFrame::Get()->SetShader(shader);

	background = new Background(shader);
	
	animation = new Animation();

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

	animation->Position(100, 170);
	animation->Scale(2.5f, 2.5f);
	animation->Play(0);
}

void DestroyScene()
{
	SafeDelete(shader);
	SafeDelete(background);
	SafeDelete(animation);
}


void Update()
{
	static bool bMove = false;
	ImGui::Checkbox("Move", &bMove);
	animation->Play(bMove ? 1 : 0);

	background->Update();
	animation->Update();
}

void Render()
{
	background->Render();
	animation->Render();
}