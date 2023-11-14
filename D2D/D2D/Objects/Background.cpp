#include "stdafx.h"
#include "Background.h"
#include "Rect.h"

Background::Background(Shader* shader)
{
	cloud[0] = new Sprite(shader, L"Cloud.png", 138, 128);
	cloud[1] = new Sprite(shader, L"Cloud.png", 141, 0, 336, 128);

	bush[0] = new Sprite(shader, L"Bush.png");
	bush[1] = new Sprite(shader, L"Bush2.png");

	tile = new Sprite(shader, L"Tile.png");

	cloud[0]->Scale(cloud[0]->GetTextureSize());
	cloud[1]->Scale(cloud[1]->GetTextureSize());

	bush[0]->Scale(bush[0]->GetTextureSize());
	bush[1]->Scale(bush[1]->GetTextureSize());

	tile->Scale(tile->GetTextureSize());

	rectShader = new Shader(L"05_Rect.fx");
	rect = new Rect(rectShader);
	rect->Position(Width * 0.5f, Height * 0.5f);
	rect->Scale(800, 600);
}

Background::~Background()
{
	SafeDelete(cloud[0]);
	SafeDelete(cloud[1]);

	SafeDelete(bush[0]);
	SafeDelete(bush[1]);

	SafeDelete(tile);

	SafeDelete(rect);
}

void Background::Update()
{
	rect->Update(PerFrame::Get()->View(), PerFrame::Get()->Projection());
}

void Background::Render()
{
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
		tile->Position(i * tile->Scale().x, 60);
		tile->Render();
	}

	static Vector2 rectScale = Vector2(800, 600);
	ImGui::SliderFloat2("Rect Scale", (float*)&rectScale, 100, 1500);
	rect->Scale(rectScale);
	rect->Render();
}
