#include "stdafx.h"
#include "Collider.h"

Collider::Collider()
{
	D3DXMatrixIdentity(&world);

	shader = new Shader(L"06_Collider.fx");
	sWorld = shader->AsMatrix("World");
	sColor = shader->AsVector("Color");

	perFrame = new PerFrame(shader);

	Vertex vertices[5];
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.f);
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.f);
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.f);
	vertices[4].Position = Vector3(-0.5f, -0.5f, 0.f);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(Vertex) * 5;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = vertices;

	Check(Device->CreateBuffer(&desc, &subResource, &vertexBuffer));
}

Collider::~Collider()
{
	SafeRelease(vertexBuffer);

	SafeDelete(shader);
	SafeDelete(perFrame);
}

void Collider::Update(Matrix& world)
{
	this->world = world;

	perFrame->Update();
}

void Collider::Render()
{
	perFrame->Render();

	sWorld->SetMatrix(world);
	sColor->SetFloatVector(bHitted ? Color(1, 0, 0, 1) : Color(0, 1, 0, 0));

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	shader->Draw(0, 0, 5);

	//UnresolvedMergeConflict. float4(X), matrix(O)
}
