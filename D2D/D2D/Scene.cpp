#include "Device.h"

ID3D11Buffer* vertexBuffer = nullptr;
ID3D11Buffer* vertexBuffer2 = nullptr;
ID3D11InputLayout* inputLayout = nullptr;

struct Vertex
{
	D3DXVECTOR3 Position;
	D3DXCOLOR Color;
};

void InitScene()
{
	//Vertex Data
	// -> Polygon1
	Vertex vertices[3];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	vertices[1].Position = D3DXVECTOR3(+0.0f, +0.0f, 0.f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.f);

	vertices[0].Color = D3DXCOLOR(1, 0, 0, 1);
	vertices[1].Color = D3DXCOLOR(0, 1, 0, 1);
	vertices[2].Color = D3DXCOLOR(0, 0, 1, 1);

	// -> Polygon2
	Vertex vertices2[3];
	vertices2[0].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.f);
	vertices2[1].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.f);
	vertices2[2].Position = D3DXVECTOR3(+0.0f, +0.0f, 0.f);

	vertices2[0].Color = D3DXCOLOR(1, 1, 1, 1);
	vertices2[1].Color = D3DXCOLOR(1, 1, 1, 1);
	vertices2[2].Color = D3DXCOLOR(0, 0, 0, 1);

	//Create Vertex Buffer
	// -> VertexBuffer1
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		HRESULT result = Device->CreateBuffer(&desc, &subResource, &vertexBuffer);
		assert(SUCCEEDED(result));
	}

	// -> VertexBuffer2
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices2;

		HRESULT result = Device->CreateBuffer(&desc, &subResource, &vertexBuffer2);
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

}

void DestroyScene()
{
	vertexBuffer->Release();
	inputLayout->Release();
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
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DeviceContext->IASetInputLayout(inputLayout);

		DeviceContext->Draw(3, 0);

		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer2, &stride, &offset);
		DeviceContext->Draw(3, 0);

	}
	SwapChain->Present(0, 0);

}