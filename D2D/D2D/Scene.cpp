#include "stdafx.h"
#include "System/Device.h"
#include "Objects/Marco.h"

Shader* shader = nullptr;
Marco* marco = nullptr;
Marco* marco2 = nullptr;
Sprite* sprite = nullptr;
PerFrame* perFrame = nullptr;

void InitScene()
{
	shader = new Shader(L"04_Sprite.fx");

	marco = new Marco(shader, Vector2(200, 140), Vector2(2.5f, 2.5f));
	Context::Get()->SetFollowCamera(marco);
	
	marco2 = new Marco(shader, Vector2(400, 300), Vector2(1.5f, 1.5f));

	sprite = new Sprite(shader, L"Background/FinalFightStage.png");
	sprite->Position(400, 300);
	sprite->Scale(2.5f, 2.5f);

	perFrame = new PerFrame(shader);
}

void DestroyScene()
{
	SafeDelete(shader);
	SafeDelete(marco);
	SafeDelete(marco2);
	SafeDelete(sprite);
	SafeDelete(perFrame);
}


void Update()
{
	sprite->Update();
	marco->Update();
	marco2->Update();
	perFrame->Update();
}

void Render()
{
	sprite->Render();
	marco->Render();
	marco2->Render();
	perFrame->Render();
}

void PostRender()
{
	//FPS
	RECT rect = { 0, 0, 800, 200 };
	wstring text = L"FPS : " + to_wstring(ImGui::GetIO().Framerate);
	DirectWirte::RenderText(text, rect);
}
