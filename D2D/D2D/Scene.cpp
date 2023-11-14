#include "stdafx.h"
#include "System/Device.h"
#include "Objects/Background.h"
#include "Objects/Bullet.h"

Shader* shader = nullptr;
Background* background = nullptr;
Clip* clip = nullptr;

void InitScene()
{
	shader = new Shader(L"04_Sprite.fx");
	PerFrame::Get()->SetShader(shader);

	background = new Background(shader);
	
	//Clip 0 - Idle
	clip = new Clip(EPlayType::Loop);
	clip->AddFrame(new Sprite(shader, L"Tarma.png", 4, 2, 34, 40), 0.22f);
	clip->AddFrame(new Sprite(shader, L"Tarma.png", 35, 2, 64, 40), 0.22f);
	clip->AddFrame(new Sprite(shader, L"Tarma.png", 64, 2, 94, 40), 0.22f);

	clip->Position(100, 170);
	clip->Scale(2.5f, 2.5f);
	clip->Play();
}

void DestroyScene()
{
	SafeDelete(shader);
	SafeDelete(background);
	SafeDelete(clip);
}


void Update()
{
	background->Update();
	clip->Update();
}

void Render()
{
	background->Render();
	clip->Render();
}