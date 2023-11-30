#include "stdafx.h"
#include "MouseDemo.h"
#include "Objects/Rect.h"
#include "Objects/Marco.h"

MouseDemo::MouseDemo()
{
	shader = new Shader(L"04_Sprite.fx");
	perFrame = new PerFrame(shader);

	background = new Sprite(shader, L"Background/SonicStage.png");
	
	rectShader = new Shader(L"05_Rect.fx");
	rect = new Rect(rectShader, Vector2(0, 0), Vector2(100, 100));
}

MouseDemo::~MouseDemo()
{
	SafeDelete(shader);
	SafeDelete(perFrame);
	SafeDelete(background);

	SafeDelete(rectShader);
	SafeDelete(rect);
}

void MouseDemo::Update()
{
	if (Mouse->DoubleClick(LButton))
		MessageBox(Hwnd, L"왼버튼", L"DoubleClick", MB_OK);
	if (Mouse->Down(MButton))
		MessageBox(Hwnd, L"중버튼", L"Down", MB_OK);
	//if (Mouse->Up(RButton))
	//	MessageBox(Hwnd, L"우버튼", L"Up", MB_OK);

	ImGui::LabelText("OnViewport", "%.2f, %.2f", Mouse->Position_OnViewport().x, Mouse->Position_OnViewport().y);
	ImGui::LabelText("OnClient", "%.2f, %.2f", Mouse->Position_OnClient().x, Mouse->Position_OnClient().y);
	ImGui::LabelText("Wheel", "%.2f", Mouse->Wheel());
	ImGui::LabelText("Wheel_Delta", "%.2f", Mouse->Wheel_Delta());
	ImGui::LabelText("Delta", "%.2f, %.2f", Mouse->Position_Delta().x, Mouse->Position_Delta().y);

	perFrame->Update();
	background->Update();

	rect->Update();
}

void MouseDemo::Render()
{
	perFrame->Render();
	background->Render();

	rect->Render();
}
