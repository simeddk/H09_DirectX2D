#include "stdafx.h"
#include "System/Device.h"

Shader* shader = nullptr;
Matrix V, P;

struct Vertex
{
	Vector3 Position;
	Vector2 Uv;
} vertices[6];

ID3D11Buffer* vertexBuffer = nullptr;
ID3D11ShaderResourceView* srv = nullptr;

void InitScene()
{
	shader = new Shader(L"03_Texture.fx");

	//Vertex Data
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.f);
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.f);
	vertices[4].Position = Vector3(-0.5f, +0.5f, 0.f);
	vertices[5].Position = Vector3(+0.5f, +0.5f, 0.f);

	vertices[0].Uv = Vector2(0, 1);
	vertices[1].Uv = Vector2(0, 0);
	vertices[2].Uv = Vector2(1, 1);
	vertices[3].Uv = Vector2(1, 1);
	vertices[4].Uv = Vector2(0, 0);
	vertices[5].Uv = Vector2(1, 0);

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(Device->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	//Create SRV
	{
		Check(D3DX11CreateShaderResourceViewFromFile
		(
			Device,
			//L"../../_Textures/Distortion.png",
			L"../../_Textures/ScopeDog.png",
			nullptr,
			nullptr,
			&srv,
			nullptr
		));
	}
}

void DestroyScene()
{
	SafeDelete(shader);
	
	SafeRelease(vertexBuffer);
	SafeRelease(srv);
}


void Update()
{
	//Transient Camera
	{
		Vector3 eye = Vector3(0, 0, 0);
		Vector3 at = Vector3(0, 0, 1);
		Vector3 up = Vector3(0, 1, 0);
		D3DXMatrixLookAtLH(&V, &eye, &(eye + at), &up);
		D3DXMatrixOrthoOffCenterLH(&P, 0, (FLOAT)Width, 0, (FLOAT)Height, -1, 1);
	}

	
}

void Render()
{
	D3DXCOLOR clearColor = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
	DeviceContext->ClearRenderTargetView(RTV, clearColor);
	{
		//World Matrix
		Matrix S, T, W;
		D3DXMatrixScaling(&S, 380, 380, 1);
		D3DXMatrixTranslation(&T, 500, 350, 0);
		W = S * T;

		//Matrix to Shader(W, V, P)
		shader->AsMatrix("World")->SetMatrix(W);
		shader->AsMatrix("View")->SetMatrix(V);
		shader->AsMatrix("Projection")->SetMatrix(P);

		//SRV to Shader
		shader->AsSRV("DiffuseMap")->SetResource(srv);
		
		//IASet VertexBuffer
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		//Time Parmeter Test
		shader->AsScalar("Time")->SetFloat(Time::Get()->Running());

		//Draw Call
		static int pass = 1;
		ImGui::SliderInt("Pass", &pass, 0, 2);
		shader->Draw(0, pass, 6);
	}
	ImGui::Render();
	SwapChain->Present(0, 0);

}