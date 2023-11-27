#include "stdafx.h"
#include "Scene02Demo.h"
#include "Objects/Rect.h"
#include "Objects/Marco.h"

Scene02Demo::Scene02Demo()
{
	shader = new Shader(L"04_Sprite.fx");
	perFrame = new PerFrame(shader);

	background = new Sprite(shader, L"Background/FinalFightStage.png");
	background->Position(400, 300);
	background->Scale(2.5f, 2.5f);
}

Scene02Demo::~Scene02Demo()
{
	SafeDelete(shader);
	SafeDelete(perFrame);
	SafeDelete(background);
}

void Scene02Demo::Update()
{
	Follow* follow = dynamic_cast<Follow*>(Context::Get()->GetCamera());
	if (follow != nullptr)
		Context::Get()->SetFreedomCamera();

	perFrame->Update();
	background->Update();
}

void Scene02Demo::Render()
{
	perFrame->Render();
	background->Render();
}
