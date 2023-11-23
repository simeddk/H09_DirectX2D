#include "stdafx.h"
#include "System/Device.h"
#include "Objects/Marco.h"

Shader* shader = nullptr;
Marco* marco = nullptr;
Marco* marco2 = nullptr;
Sprite* sprite = nullptr;

void InitScene()
{
	shader = new Shader(L"04_Sprite.fx");
	PerFrame::Get()->SetShader(shader);

	marco = new Marco(shader, Vector2(200, 140), Vector2(2.5f, 2.5f));
	PerFrame::Get()->SetFollowCamera(marco);
	
	marco2 = new Marco(shader, Vector2(400, 300), Vector2(1.5f, 1.5f));

	sprite = new Sprite(shader, L"Background/FinalFightStage.png");
	sprite->Position(400, 300);
	sprite->Scale(2.5f, 2.5f);
}

void DestroyScene()
{
	SafeDelete(shader);
	SafeDelete(marco);
	SafeDelete(marco2);
	SafeDelete(sprite);
}


void Update()
{
	Follow* follow = dynamic_cast<Follow*>(PerFrame::Get()->GetCamera());
	if (follow != nullptr)
	{
		if (Key->Down('1'))
			follow->SetFocusObject(marco);
		else if (Key->Down('2'))
			follow->SetFocusObject(marco2);
	}

	sprite->Update();
	marco->Update();
	marco2->Update();

	static Vector2 test;
	ImGui::SliderFloat2("Test", test, 1, 100);

	static bool bCheck;
	ImGui::Checkbox("Check", &bCheck);

	static Color color;
	ImGui::ColorEdit3("Color", color);
}

void Render()
{
	sprite->Render();
	marco->Render();
	marco2->Render();
}

void PostRender()
{
	//Pangram
	static Color fontColor = Color(1, 1, 1, 1);
	ImGui::ColorEdit4("Font Color", fontColor);

	RECT rect = { 0, 0, 800, 200 };
	wstring text = L"참나무 타는 소리와 야경만큼 밤의 여유를 표현해 주는 것도 없다.";
	DirectWirte::SetFontSize(20);
	DirectWirte::SetFontColor(fontColor);
	DirectWirte::RenderText(text, rect);

	//FPS
	rect.top += 25;
	rect.bottom += 25;
	text = L"FPS : " + to_wstring(ImGui::GetIO().Framerate);
	DirectWirte::SetFontSize(30);
	DirectWirte::SetFontColor(Color(1, 0, 0, 1));
	DirectWirte::RenderText(text, rect);
}
