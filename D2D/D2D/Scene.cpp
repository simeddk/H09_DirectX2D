#include "stdafx.h"
#include "Device.h"

Shader* shader = nullptr;

ID3D11Buffer* vertexBuffer = nullptr;
ID3D11Buffer* indexBuffer = nullptr;
ID3D11RasterizerState* rasterizerState = nullptr;

struct Vertex
{
	D3DXVECTOR3 Position;
	D3DXCOLOR Color;
};

Vertex vertices[6];
UINT indices[12];

void InitScene()
{
	shader = new Shader(L"01_First.fx");

	//Vertex Data
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.f);
	vertices[4].Position = D3DXVECTOR3(+0.75f, -0.75f, 0.f);
	vertices[5].Position = D3DXVECTOR3(+0.75f, +0.75f, 0.f);

	vertices[0].Color = D3DXCOLOR(1, 0, 0, 1);
	vertices[1].Color = D3DXCOLOR(1, 0, 0, 1);
	vertices[2].Color = D3DXCOLOR(1, 0, 0, 1);
	vertices[3].Color = D3DXCOLOR(1, 0, 0, 1);
	vertices[4].Color = D3DXCOLOR(0, 1, 0, 1);
	vertices[5].Color = D3DXCOLOR(0, 1, 0, 1);

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 6;
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

		indices[6] = 2;
		indices[7] = 3;
		indices[8] = 4;
		indices[9] = 4;
		indices[10] = 3;
		indices[11] = 5;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * 12;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		HRESULT result = Device->CreateBuffer(&desc, &subResource, &indexBuffer);
		assert(SUCCEEDED(result));
	}

	//Create Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_BACK;
		HRESULT result = Device->CreateRasterizerState(&desc, &rasterizerState);
		assert(SUCCEEDED(result));
	}
}

void DestroyScene()
{
	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);
	SafeRelease(rasterizerState);
}


bool bWireFrameMode = false;
float color[3];
void Update()
{
	//ImGui Test
	ImGui::ColorEdit3("Color", color);
	shader->AsVector("Color")->SetFloatVector(color);
	ImGui::Text("R : %f, G : %f, B : %f", color[0], color[1], color[2]);

	//WireFrame On/Off
	if (GetAsyncKeyState(VK_F1) & 0x0001)
		bWireFrameMode = !bWireFrameMode;

	//Move
	if (Key->Press('A'))
	{
		for (int i = 0; i < 4; i++)
			vertices[i].Position.x -= 1e-4f;
	}
	else if (Key->Press('D'))
	{
		for (int i = 0; i < 4; i++)
			vertices[i].Position.x += 1e-4f;
	}

	if (Key->Press('S'))
	{
		for (int i = 0; i < 4; i++)
			vertices[i].Position.y -= 1e-4f;
	}
	else if (Key->Press('W'))
	{
		for (int i = 0; i < 4; i++)
			vertices[i].Position.y += 1e-4f;
	}

	//UpdateSubresource(GPU, 속도가 느림)
	DeviceContext->UpdateSubresource
	(
		vertexBuffer,
		0,
		nullptr,
		vertices,
		sizeof(Vertex) * 6,
		0
	);

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

		DeviceContext->RSSetState(bWireFrameMode ? rasterizerState : nullptr);

		shader->DrawIndexed(0, 0, 12);
	}
	ImGui::Render();
	SwapChain->Present(0, 0);

}