#include "stdafx.h"
#include "Rect.h"

Rect::Rect(Shader* shader)
	: shader(shader)
	, position(0, 0)
	, scale(1, 1)
	, color(1, 1, 1, 1)
	, pass(0)
{
	Initialize(shader);
}

Rect::Rect(Shader* shader, Vector2 position, Vector2 scale, D3DXCOLOR color)
	: shader(shader)
	, position(position)
	, scale(scale)
	, color(color)
	, pass(0)
{
	Initialize(shader);
}

void Rect::Initialize(Shader* shader)
{
	perFrame = new PerFrame(shader);
	collider = new Collider();

	CreateBuffer();

	D3DXMatrixIdentity(&world);

	sWorld = shader->AsMatrix("World");
	sColor = shader->AsVector("Color");
}


void Rect::CreateBuffer()
{
	//Vertex Data
	Vertex vertices[4];

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.f);
	vertices[3].Position = Vector3(+0.5f, +0.5f, 0.f);

	vertices[0].Uv = Vector2(0, 1);
	vertices[1].Uv = Vector2(0, 0);
	vertices[2].Uv = Vector2(1, 1);
	vertices[3].Uv = Vector2(1, 0);

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		HRESULT result = Device->CreateBuffer(&desc, &subResource, &vertexBuffer);
		Check(result);
	}

	//Index Data
	UINT indices[] = { 0, 1, 2, 2, 1, 3 };

	//Create Index Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		HRESULT result = Device->CreateBuffer(&desc, &subResource, &indexBuffer);
		Check(result);
	}
}

Rect::~Rect()
{
	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);
	
	SafeDelete(perFrame);
	SafeDelete(collider);
}

void Rect::Update()
{
	perFrame->Update();
	collider->Update(world);
}

void Rect::Render()
{
	perFrame->Render();

	ApplyTo();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, pass, 6);

	collider->Render();
}

void Rect::ApplyTo()
{
	Matrix S, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, 1.f);
	D3DXMatrixTranslation(&T, position.x, position.y, 0.f);
	world = S * T;

	sWorld->SetMatrix(world);
	sColor->SetFloatVector(color);
}

void Rect::Position(float x, float y)
{
	Position(Vector2(x, y));
}

void Rect::Position(Vector2& vec)
{
	position = vec;
}

void Rect::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}

void Rect::Scale(Vector2& vec)
{
	scale = vec;
}

void Rect::Color(float r, float g, float b, float a)
{
	Color(D3DXCOLOR(r, g, b, a));
}

void Rect::Color(D3DXCOLOR& vec)
{
	color = vec;
}
