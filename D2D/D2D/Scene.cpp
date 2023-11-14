#include "stdafx.h"
#include "System/Device.h"
#include "Objects/Background.h"
#include "Objects/Bullet.h"

Shader* shader = nullptr;
Sprite* marioSheet = nullptr;
Background* background = nullptr;

vector<Bullet*> bullets;
float pressedTime = 0.f;
float delay = 0.05f;

void ClearBullet()
{
	bullets.clear();
}

void InitScene()
{
	shader = new Shader(L"04_Sprite.fx");
	PerFrame::Get()->SetShader(shader);

	background = new Background(shader);
	
	marioSheet = new Sprite(shader, L"MarioSheet.png", 260, 165, 320, 250);
	marioSheet->Position(160, 160);
	marioSheet->Scale(marioSheet->GetTextureSize());

	//GBC
	/*
	Timer timer;
	timer.Start(ClearBullet, 1500); 
	*/
}

void DestroyScene()
{
	SafeDelete(shader);
	SafeDelete(background);
	SafeDelete(marioSheet);

	for (Bullet* bullet : bullets)
		SafeDelete(bullet);
}


void Update()
{
	//Move
	Vector2 position = marioSheet->Position();
	{
		if (Key->Press('W'))
			position.y += 200.f * Time::Delta();
		else if (Key->Press('S'))
			position.y -= 200.f * Time::Delta();

		if (Key->Press('D'))
			position.x += 200.f * Time::Delta();
		else if (Key->Press('A'))
			position.x -= 200.f * Time::Delta();

		marioSheet->Position(position);
	}

	//Shoot
	{
		float degree = 0;

		if (Key->Press(VK_SPACE))
		{
			pressedTime += Time::Delta();

			if (pressedTime > delay)
			{
				degree = Math::Lerp<float>(0.f, 360.f, fmodf(Time::Get()->Running(), 1.0f));

				bullets.push_back(new Bullet(shader, position, degree, 600.f));
				pressedTime = 0.f;
			}
		}
	}

	if (Key->Down('X'))
		bullets.clear();

	vector<Bullet*>::iterator it = bullets.begin();
	for (it; it != bullets.end(); ++it)
	{
		Bullet* bullet = *it;

		if ((bullet->Position().x > (float)Width) ||
			(bullet->Position().x < 0.f) ||
			(bullet->Position().y > (float)Height) ||
			(bullet->Position().y < 0.f))
		{
			bullets.erase(it);
			break;
		}
	}

	for (Bullet* bullet : bullets)
		bullet->Update();


	background->Update();
	marioSheet->Update();
}

void Render()
{
	background->Render();
	marioSheet->Render();

	for (Bullet* bullet : bullets)
		bullet->Render();

	ImGui::Text("FPS : %.2f", ImGui::GetIO().Framerate);
	ImGui::Text("Bullet Number : %d", bullets.size());
}