#include "stdafx.h"
#include "Device.h"

Shader* shader = nullptr;

ID3D11Buffer* vertexBuffer = nullptr;
ID3D11Buffer* indexBuffer = nullptr;

struct Vertex
{
	Vector3 Position = Vector3(0, 0, 0);
	Color Color = D3DXCOLOR(1, 1, 1, 1);
};

Vertex vertices[4];
UINT indices[6];

Matrix W, V, P;

void InitScene()
{
	shader = new Shader(L"02_WorldMatrix.fx");

	//Vertex Data
	vertices[0].Position = Vector3(-100.f, -100.f, 0.f);
	vertices[1].Position = Vector3(-100.f, +100.f, 0.f);
	vertices[2].Position = Vector3(+100.f, -100.f, 0.f);
	vertices[3].Position = Vector3(+100.f, +100.f, 0.f);

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
		assert(SUCCEEDED(result));
	}

	//Create Index Buffer
	{
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 1;
		indices[5] = 3;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		HRESULT result = Device->CreateBuffer(&desc, &subResource, &indexBuffer);
		assert(SUCCEEDED(result));
	}

	//Make Matrix Identity
	D3DXMatrixIdentity(&W);
	D3DXMatrixIdentity(&V);
	D3DXMatrixIdentity(&P);

	//Matix Settings
	// -> World Space
	D3DXMatrixTranslation(&W, 800, 400, 0.f);
	//Todo. 키보드 이동은 내일 해보자...

	// -> View Space
	Vector3 eye = Vector3(0, 0, 0);
	Vector3 at = Vector3(0, 0, 1);
	Vector3 up = Vector3(0, 1, 0);
	D3DXMatrixLookAtLH(&V, &eye, &(eye + at), &up);

	//-> Projection Space
	D3DXMatrixOrthoOffCenterLH(&P, 0, (FLOAT)Width, 0, (FLOAT)Height, -1, 1);

	//Set Matrix Parameter to Shader
	shader->AsMatrix("World")->SetMatrix(W);
	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);
}

void DestroyScene()
{
	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);

	SafeDelete(shader);
}


void Update()
{

}

void Render()
{
	D3DXCOLOR clearColor = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
	DeviceContext->ClearRenderTargetView(RTV, clearColor);
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		shader->DrawIndexed(0, 0, 6);
	}
	ImGui::Render();
	SwapChain->Present(0, 0);

}