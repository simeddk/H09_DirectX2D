#include "stdafx.h"
#include "Device.h"

ID3D11Buffer* vertexBuffer = nullptr;
ID3D11InputLayout* inputLayout = nullptr;
ID3D11RasterizerState* rasterizerState = nullptr;

struct Vertex
{
	D3DXVECTOR3 Position;
	D3DXCOLOR Color;
};

void InitScene()
{
	//Vertex Data
	Vertex vertices[6];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.f);
	vertices[5].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.f);

	for (int i = 0 ; i < 6; i++)
		vertices[i].Color = D3DXCOLOR(1, 1, 1, 1);

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

	//Create InputLayout
	{
		D3D11_INPUT_ELEMENT_DESC desc[] = 
		{ 
			//[0]
			{
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},

			//[1]
			{
				"COLOR",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				12,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			}
		};
		

		HRESULT result = Device->CreateInputLayout
		(
			desc,
			2,
			VsBlob->GetBufferPointer(),
			VsBlob->GetBufferSize(),
			&inputLayout
		);
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
	vertexBuffer->Release();
	inputLayout->Release();
}


bool bWireFrameMode = false;
void Update()
{
	if (GetAsyncKeyState(VK_F1) & 0x0001)
		bWireFrameMode = !bWireFrameMode;
}

void Render()
{
	D3DXCOLOR clearColor = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
	DeviceContext->ClearRenderTargetView(RTV, clearColor);
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DeviceContext->IASetInputLayout(inputLayout);

		DeviceContext->RSSetState(bWireFrameMode ? rasterizerState : nullptr);

		DeviceContext->Draw(6, 0);
	}
	SwapChain->Present(0, 0);

}