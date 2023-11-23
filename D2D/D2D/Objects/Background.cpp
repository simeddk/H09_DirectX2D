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

	rectShader = new Shader(L"05_Rect.fx");
	rect = new Rect(rectShader);
	rect->Position((float)Width * 0.5f, (float)Height * 0.5f);
	rect->Scale((float)Width, (float)Height);
	rect->Color(0, 0, 1);
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
	//rect->Update(Context::Get()->View(), Context::Get()->Projection());
}

void Background::Render()
{
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
}
