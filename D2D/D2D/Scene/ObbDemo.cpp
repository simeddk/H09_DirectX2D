#include "stdafx.h"
#include "ObbDemo.h"
#include "Objects/Marco.h"
#include "Objects/Fire.h"

ObbDemo::ObbDemo()
{
	shader = new Shader(L"04_Sprite.fx");
	
	marco = new Marco(shader, Vector2(100, 170), Vector2(2.5f, 2.5f));
	fire = new Fire(shader, Vector2(350, 170), Vector2(1.5f, 1.5f));
}

ObbDemo::~ObbDemo()
{
	SafeDelete(shader);
	SafeDelete(marco);
	SafeDelete(fire);
}

void ObbDemo::Update()
{
	const char* objectName[] = { "Marco", "Fire" };
	static int selected = 0;
	ImGui::Combo("Gizmo", &selected, objectName, 2);
	if (selected == 0)
	{
		marco->EnableGizmo();
		fire->DisableGizmo();
	}
	else
	{
		fire->EnableGizmo();
		marco->DisableGizmo();
	}

	bool bObb = Collider::Obb(marco->GetSprite(), fire->GetSprite());
	bObb ? marco->GetCollider()->SetHit() : marco->GetCollider()->SetMiss();
	bObb ? fire->GetCollider()->SetHit() : fire->GetCollider()->SetMiss();

	/*if (bObb)
	{
		wstring str1 = to_wstring(marco->Position().x) + to_wstring(marco->Position().y);
		wstring str2 = to_wstring(fire->Position().x) + to_wstring(fire->Position().y);
		wstring message = str1 + L"\n" + str2;
		MessageBox(Hwnd, message.c_str(), L"OBB", MB_OK);
	}*/

	marco->Update();
	fire->Update();
}

void ObbDemo::Render()
{
	marco->Render();
	fire->Render();
}
