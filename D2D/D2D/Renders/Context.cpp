#include "stdafx.h"
#include "Context.h"
#include "Viewer/IFocus.h"

Context* Context::instance = nullptr;

Context::Context()
{
	camera = new Freedom();

	D3DXMatrixIdentity(&projection);
}

Context::~Context()
{
	SafeDelete(camera);
}

void Context::Create()
{
	assert(instance == nullptr);
	instance = new Context();
}

void Context::Delete()
{
	SafeDelete(instance);
}

Context* Context::Get()
{
	assert(instance != nullptr);
	return instance;
}

void Context::Update()
{
	camera->Update();

	static bool bOrtho = true;
	ImGui::Checkbox("Ortho", &bOrtho);

	if (bOrtho)
	{
		D3DXMatrixOrthoOffCenterLH
		(
			&projection,
			(float)Width * -0.5f,	//left
			(float)Width * +0.5f,	//right
			(float)Height * -0.5f,	//bottom
			(float)Height * +0.5f,	//top
			-1 + D3DX_16F_EPSILON,	//znear
			1 + D3DX_16F_EPSILON	//zfar
		);
	}
	else
	{
		static float fov = 0.5f;
		static float n = 0.f;
		static float f = 255.f;

		ImGui::SliderFloat("FOV", &fov, 0.001f, 2.f);
		ImGui::SliderFloat("Depth", &camera->Depth(), -100, -1000);

		float aspect = (float)Width / (float)Height;

		D3DXMatrixPerspectiveFovLH
		(
			&projection,
			Math::PI * fov,
			aspect,
			n, f
		);
	}
}

const Matrix& Context::View()
{
	return camera->View();
}

const Matrix& Context::Projection()
{
	return projection;
}

void Context::SetFollowCamera(IFocus* focusObject)
{
	SafeDelete(camera);

	camera = new Follow(focusObject);
}

void Context::SetFreedomCamera()
{
	SafeDelete(camera);

	camera = new Freedom();
}
