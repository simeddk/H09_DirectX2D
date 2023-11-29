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
