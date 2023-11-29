#include "stdafx.h"
#include "ScreenSpaceDemo.h"
#include "Objects/Rect.h"
#include "Objects/Marco.h"

ScreenSpaceDemo::ScreenSpaceDemo()
{
	shader = new Shader(L"04_Sprite.fx");
	perFrame = new PerFrame(shader);

	background = new Sprite(shader, L"Background/SonicStage.png");
	
	rectShader = new Shader(L"05_Rect.fx");
	rect = new Rect(rectShader, Vector2(0, 0), Vector2(100, 100));
}

ScreenSpaceDemo::~ScreenSpaceDemo()
{
	SafeDelete(shader);
	SafeDelete(perFrame);
	SafeDelete(background);

	SafeDelete(rectShader);
	SafeDelete(rect);
}

void ScreenSpaceDemo::Update()
{
	ImGui::LabelText("Mouse Position", "%d, %d", mousePosition.x, mousePosition.y);

	perFrame->Update();
	background->Update();

	rect->Update();
}

void ScreenSpaceDemo::Render()
{
	perFrame->Render();
	background->Render();

	rect->Render();
}
