#include "stdafx.h"
#include "Scene01Demo.h"
#include "Objects/Rect.h"
#include "Objects/Marco.h"

Scene01Demo::Scene01Demo()
{
	shader = new Shader(L"04_Sprite.fx");
	perFrame = new PerFrame(shader);

	cloud[0] = new Sprite(shader, L"Cloud.png", 138, 128);
	cloud[1] = new Sprite(shader, L"Cloud.png", 141, 0, 336, 128);

	bush[0] = new Sprite(shader, L"Bush.png");
	bush[1] = new Sprite(shader, L"Bush2.png");

	tile = new Sprite(shader, L"Tile.png");

	rectShader = new Shader(L"05_Rect.fx");
	rect = new Rect(rectShader);
	rect->Position((float)Width * 0.5f, (float)Height * 0.5f);
	rect->Scale((float)Width, (float)Height);
	rect->Color(0, 0, 1);

	marco = new Marco(shader, Vector2(100, 170), Vector2(2.5f, 2.5f));
	Context::Get()->SetFollowCamera(marco);
}

Scene01Demo::~Scene01Demo()
{
	SafeDelete(cloud[0]);
	SafeDelete(cloud[1]);

	SafeDelete(bush[0]);
	SafeDelete(bush[1]);

	SafeDelete(tile);

	SafeDelete(rect);
	SafeDelete(rectShader);

	SafeDelete(shader);
	SafeDelete(perFrame);

	SafeDelete(marco);
}

void Scene01Demo::Update()
{
	/*if (Key->Down(VK_NUMPAD1))
		Context::Get()->SetFreedomCamera();
	else if (Key->Down(VK_NUMPAD2))
		Context::Get()->SetFollowCamera(marco);*/

	Freedom* freedom = dynamic_cast<Freedom*>(Context::Get()->GetCamera());
	if (freedom != nullptr)
		Context::Get()->SetFollowCamera(marco);

	perFrame->Update();

	rect->Update();

	Vector2 cameraPosition = Context::Get()->GetCamera()->Position();
	cameraPosition.x += Width * 0.5f;
	cameraPosition.y += Height * 0.5f;
	rect->Position(cameraPosition);

	marco->Update();
}

void Scene01Demo::Render()
{
	perFrame->Render();

	rect->Render();

	cloud[0]->Position(240, 600);
	cloud[0]->Render();

	cloud[0]->Position(400, 600);
	cloud[0]->Render();

	cloud[1]->Position(700, 600);
	cloud[1]->Render();

	bush[1]->Position(150, 170);
	bush[1]->Render();

	bush[1]->Position(500, 170);
	bush[1]->Render();

	bush[1]->Position(800, 170);
	bush[1]->Render();

	bush[0]->Position(300, 130);
	bush[0]->Render();

	bush[0]->Position(850, 130);
	bush[0]->Render();

	for (int i = 0; i < 9; i++)
	{
		tile->Position(i * tile->GetTextureSize().x * tile->Scale().x, 60);
		tile->Render();
	}

	marco->Render();
}
