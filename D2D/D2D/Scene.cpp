#include "stdafx.h"
#include "System/Device.h"

Shader* shader = nullptr;
Sprite* fire = nullptr;
Sprite* marioSheet = nullptr;

void InitScene()
{
	shader = new Shader(L"04_Sprite.fx");
	PerFrame::Get()->SetShader(shader);

	fire = new Sprite(shader, L"Fire.png");
	fire->Position(300, 300);
	fire->Scale(200, 200);

	marioSheet = new Sprite(shader, L"MarioSheet.png", 260, 165, 320, 250);
	marioSheet->Position(650, 300);
	marioSheet->Scale(marioSheet->GetTextureSize());
	Vector2 textureSize = marioSheet->GetTextureSize();
}

void DestroyScene()
{
	SafeDelete(shader);
	SafeDelete(fire);
	SafeDelete(marioSheet);
}


void Update()
{
	fire->Update();
	marioSheet->Update();
}

void Render()
{
	fire->Render();
	marioSheet->Render();
}