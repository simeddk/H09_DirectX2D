#include "stdafx.h"
#include "Device.h"
#include "Objects/Rect.h"
#include "Objects/MovableRect.h"

Shader* shader = nullptr;
Matrix V, P;

Rect* rect = nullptr;
Rect* rect2 = nullptr;
MovableRect* movableRect = nullptr;

void InitScene()
{
	shader = new Shader(L"02_WorldMatrix.fx");

	rect = new Rect(shader);
	rect->Position(200, 500);
	rect->Scale(100, 100);
	rect->Color(1, 0, 0);

	rect2 = new Rect(shader, Vector2(500, 500), Vector2(50, 50), Color(0, 0, 1, 1));

	movableRect = new MovableRect(shader);
	movableRect->Scale(100, 100);
	movableRect->Position(Width * 0.5f, movableRect->Scale().y * 0.5f);
}

void DestroyScene()
{
	SafeDelete(shader);
	SafeDelete(rect);
	SafeDelete(rect2);
	SafeDelete(movableRect);
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

	//Rect Test
	{
		rect->Update(V, P);
		rect2->Update(V, P);

		if (Key->Press('D'))
			movableRect->MoveRight();
		else if (Key->Press('A'))
			movableRect->MoveLeft();

		if (Key->Down(VK_SPACE))
			movableRect->StartJump();
		else if (Key->Up(VK_SPACE))
			movableRect->EndJump();

		movableRect->Update(V, P);
	}

	//ImGui Test
	{
		float moveSpeed = movableRect->MoveSpeed();
		ImGui::SliderFloat("MoveSpeed", &moveSpeed, 0.0001f, 0.5f);
		movableRect->MoveSpeed(moveSpeed);

		ImGui::Text("FPS : %.2f",  ImGui::GetIO().Framerate);

		ImGui::Text("dt : %f", Time::Delta());

		static float runningTime;
		runningTime += Time::Delta();
		ImGui::Text("RunningTime : %f", runningTime);
	}

}

void Render()
{
	D3DXCOLOR clearColor = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
	DeviceContext->ClearRenderTargetView(RTV, clearColor);
	{
		rect->Render();
		rect2->Render();
		movableRect->Render();
	}
	ImGui::Render();
	SwapChain->Present(0, 0);

}