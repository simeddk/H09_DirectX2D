#include "stdafx.h"
#include "System/Device.h"
#include "Objects/Marco.h"

Shader* shader = nullptr;
Marco* marco = nullptr;
Sprite* sprite = nullptr;

void InitScene()
{
	shader = new Shader(L"04_Sprite.fx");
	PerFrame::Get()->SetShader(shader);

	marco = new Marco(shader, Vector2(200, 140), Vector2(2.5f, 2.5f));
	sprite = new Sprite(shader, L"Background/FinalFightStage.png");
	sprite->Position(400, 300);
	sprite->Scale(2.5f, 2.5f);
}

void DestroyScene()
{
	SafeDelete(shader);
	SafeDelete(marco);
	SafeDelete(sprite);
}


void Update()
{
	static float cameraSpeed = 200.f;
	ImGui::SliderFloat("Camera Speed", &cameraSpeed, 10, 10000);
	Freedom* freedom = dynamic_cast<Freedom*>(PerFrame::Get()->GetCamera());
	if (freedom != nullptr)
		freedom->Speed(cameraSpeed);

	sprite->Update();
	marco->Update();
}

void Render()
{
	sprite->Render();
	marco->Render();
}