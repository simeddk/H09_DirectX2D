#include "stdafx.h"
#include "Device.h"
#include "Objects/Rect.h"

Shader* shader = nullptr;
Matrix V, P;

Rect* rect = nullptr;
Rect* rect2 = nullptr;

void InitScene()
{
	shader = new Shader(L"02_WorldMatrix.fx");
	
	rect = new Rect(shader);
	rect->Position(200, 500);
	rect->Scale(100, 100);
	rect->Color(1, 0, 0);

	rect2 = new Rect(shader, Vector2(500, 500), Vector2(50, 50), Color(0, 0, 1, 1));

	
}

void DestroyScene()
{
	SafeDelete(shader);
	SafeDelete(rect);
	SafeDelete(rect2);
}


void Update()
{
	//Transient Camera
	{
		Vector3 eye = Vector3(0, 0, 0);
		Vector3 at = Vector3(0, 0, 1);
		Vector3 up = Vector3(0, 1, 0);
		D3DXMatrixLookAtLH(&V, &eye, &(eye + at), &up);
		D3DXMatrixOrthoOffCenterLH(&P, 0, (FLOAT)Width, 0, (FLOAT)Height, -1, 1);
	}

	rect->Update(V, P);
	rect2->Update(V, P);
}

void Render()
{
	D3DXCOLOR clearColor = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
	DeviceContext->ClearRenderTargetView(RTV, clearColor);
	{
		rect->Render();
		rect2->Render();
	}
	ImGui::Render();
	SwapChain->Present(0, 0);

}