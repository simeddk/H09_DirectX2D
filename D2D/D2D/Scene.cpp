#include "stdafx.h"
#include "System/Device.h"
#include "Objects/Background.h"

Shader* shader = nullptr;
Sprite* marioSheet = nullptr;
Background* background = nullptr;

void InitScene()
{
	shader = new Shader(L"04_Sprite.fx");
	PerFrame::Get()->SetShader(shader);

	background = new Background(shader);
	
	marioSheet = new Sprite(shader, L"MarioSheet.png", 260, 165, 320, 250);
	marioSheet->Position(650, 300);
	marioSheet->Scale(marioSheet->GetTextureSize());
}

void DestroyScene()
{
	SafeDelete(shader);
	SafeDelete(background);
	SafeDelete(marioSheet);
}


void Update()
{
	background->Update();
	marioSheet->Update();
}

void Render()
{
	background->Render();
	marioSheet->Render();
}