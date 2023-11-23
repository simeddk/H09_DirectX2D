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

	Vector2 screenSize = Vector2((FLOAT)Width, (FLOAT)Height);
	D3DXMatrixOrthoOffCenterLH(&projection, 0, screenSize.x, 0, screenSize.y, -1, 1);
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
