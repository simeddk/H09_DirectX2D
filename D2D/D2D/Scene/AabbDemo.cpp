#include "stdafx.h"
#include "AabbDemo.h"
#include "Objects/Marco.h"
#include "Objects/Marker.h"
#include "Objects/Fire.h"

AabbDemo::AabbDemo()
{
	shader = new Shader(L"04_Sprite.fx");
	
	marco = new Marco(shader, Vector2(100, 170), Vector2(2.5f, 2.5f));
	marker = new Marker(shader, Vector2(250, 250));
	fire = new Fire(shader, Vector2(350, 170), Vector2(1.5f, 1.5f));

	D3DXMatrixIdentity(&volumeMatrix);
	volumeCollider = new Collider();

	gizmo = new Gizmo();
}

AabbDemo::~AabbDemo()
{
	SafeDelete(shader);
	SafeDelete(marco);
	SafeDelete(marker);
	SafeDelete(fire);
	SafeDelete(volumeCollider);
	SafeDelete(gizmo);
}

void AabbDemo::Update()
{
	bool bAabb = Collider::Aabb(marco->GetWorld(), marker->Position());
	ImGui::LabelText("Matrix - Vector", "%s", bAabb ? "Yes Overlapped" : "No Overlapped");
	bAabb ? marco->GetCollider()->SetHit() : marco->GetCollider()->SetMiss();

	//ImGui::SliderFloat2("Fire Position", fire->Position(), -400, 400);
	//ImGui::SliderFloat2("Fire Scale", fire->Scale(), 1.5f, 10.f);
	bool bAabb2 = Collider::Aabb(marco->GetWorld(), fire->GetWorld());
	ImGui::LabelText("Matrix - Matrix", "%s", bAabb2 ? "Yes Overlapped" : "No Overlapped");
	bAabb2 ? fire->GetCollider()->SetHit() : fire->GetCollider()->SetMiss();

	marco->Update();
	marker->Update();
	fire->Update();


	Matrix volumeScale, volumePosition;
	D3DXMatrixScaling(&volumeScale, 200, 40, 1);
	D3DXMatrixTranslation(&volumePosition, 300, 300, 0);
	volumeMatrix = volumeScale * volumePosition;

	volumeCollider->Update(volumeMatrix);

	bool bAabb3 = Collider::Aabb(marco->GetWorld(), volumeMatrix);
	bAabb3 ? fire->Stop() : fire->Play();
	bAabb3 ? volumeCollider->SetHit() : volumeCollider->SetMiss();

	Vector3 r;
	Vector2 s, t;
	gizmo->Set(marco->GetSprite(), &s, &r, &t);
	marco->Position(t);
	marco->Scale(s);
	marco->Rotation(r);
}

void AabbDemo::Render()
{
	marco->Render();
	marker->Render();
	fire->Render();
	volumeCollider->Render();
}
