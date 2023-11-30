#include "stdafx.h"
#include "SpawnDemo.h"
#include "Objects/Rect.h"
#include "Objects/Marco.h"
#include "Objects/Marker.h"

SpawnDemo::SpawnDemo()
{
	shader = new Shader(L"04_Sprite.fx");
	perFrame = new PerFrame(shader);

	background = new Sprite(shader, L"Background/SonicStage.png");
	
	rectShader = new Shader(L"07_Edge.fx");
	rect = new Rect(rectShader, Vector2(0, 0), Vector2(100, 100));
}

SpawnDemo::~SpawnDemo()
{
	SafeDelete(shader);
	SafeDelete(perFrame);
	SafeDelete(background);

	SafeDelete(rectShader);
	SafeDelete(rect);

	for (auto marker : markers)
		SafeDelete(marker);
}

void SpawnDemo::Update()
{
	ImGui::LabelText("OnViewport", "%.2f, %.2f", Mouse->Position_OnViewport().x, Mouse->Position_OnViewport().y);
	ImGui::LabelText("OnClient", "%.2f, %.2f", Mouse->Position_OnClient().x, Mouse->Position_OnClient().y);
	ImGui::LabelText("Wheel", "%.2f", Mouse->Wheel());
	ImGui::LabelText("Wheel_Delta", "%.2f", Mouse->Wheel_Delta());
	ImGui::LabelText("Delta", "%.2f, %.2f", Mouse->Position_Delta().x, Mouse->Position_Delta().y);

#ifdef OnViewport
	//OnViewport + Camera
	{
		Vector2 mouse = Mouse->Position_OnViewport(); //ScreenSpace - Viewport(LeftTop : 0,0)
		mouse.x = mouse.x - (float)Width * 0.5f;
		mouse.y = (mouse.y - (float)Height * 0.5f) * -1.f; //Projection(Center : 0,0)

		Vector2 camera = Context::Get()->GetCamera()->Position(); //View(Eye : 0, 0)
		Vector2 wPosition = mouse + camera;
		//-> Mouse(SS) -> Game(WS)
	}
#endif

	Vector2 wPosition = Mouse->Position_World();

	//Spawn Marker
	if (Mouse->Down(LButton))
		markers.push_back(new Marker(shader, wPosition));

	if (Key->Down(VK_ESCAPE))
		markers.clear();

	//Rect Move
	bool bPicked = true;
	bPicked &= Collider::Aabb(rect->World(), wPosition);
	rect->Color(bPicked ? Color(1, 0, 0, 1) : Color(1, 1, 1, 1));

	bPicked &= Mouse->Press(MButton);
	if (bPicked)
		rect->Position(wPosition);


	perFrame->Update();
	background->Update();

	rect->Update();

	for (auto marker : markers)
		marker->Update();
}

void SpawnDemo::Render()
{
	perFrame->Render();
	background->Render();

	rect->Render();

	for (auto marker : markers)
		marker->Render();
}
