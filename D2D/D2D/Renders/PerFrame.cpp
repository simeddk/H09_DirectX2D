#include "stdafx.h"
#include "PerFrame.h"

PerFrame* PerFrame::instance = nullptr;

PerFrame::PerFrame()
{
	camera = new Freedom();
}

PerFrame::~PerFrame()
{
	SafeRelease(constantBuffer);

	SafeDelete(camera);
}

void PerFrame::Create()
{
	assert(instance == nullptr);
	instance = new PerFrame();
}

void PerFrame::Delete()
{
	SafeDelete(instance);
}

PerFrame* PerFrame::Get()
{
	assert(instance != nullptr);
	return instance;
}

void PerFrame::SetShader(Shader* shader)
{
	this->shader = shader;

	//Create Constant Buffer
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.ByteWidth = sizeof(Desc);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		Check(Device->CreateBuffer(&bufferDesc, nullptr, &constantBuffer));
	}

	CheckNull(this->shader);
	sConstantBuffer = this->shader->AsConstantBuffer("CB_PerFrame");
}

void PerFrame::Update()
{
	camera->Update();
	memcpy(&desc.View, camera->View(), sizeof(Matrix));

	Vector2 screenSize = Vector2((FLOAT)Width, (FLOAT)Height);
	D3DXMatrixOrthoOffCenterLH(&desc.Projection, 0, screenSize.x, 0, screenSize.y, -1, 1);
}

void PerFrame::Render()
{
	CheckNull(shader);

	D3D11_MAPPED_SUBRESOURCE subResource;
	DeviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, &desc, sizeof(Desc));
	}
	DeviceContext->Unmap(constantBuffer, 0);

	sConstantBuffer->SetConstantBuffer(constantBuffer);
}
