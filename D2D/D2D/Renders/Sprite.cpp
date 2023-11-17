#include "stdafx.h"
#include "Sprite.h"

//-------------------------------------------------------------------------------
// Sprite
//-------------------------------------------------------------------------------
Sprite::Sprite(Shader* shader, wstring textureFile)
{
	Initialize(shader, textureFile, 0, 0, 0, 0);
}

Sprite::Sprite(Shader* shader, wstring textureFile, float cropEndX, float cropEndY)
{
	Initialize(shader, textureFile, 0, 0, cropEndX, cropEndY);
}

Sprite::Sprite(Shader* shader, wstring textureFile, float cropStartX, float cropStartY, float cropEndX, float cropEndY)
{
	Initialize(shader, textureFile, cropStartX, cropStartY, cropEndX, cropEndY);
}


void Sprite::Initialize(Shader* shader, wstring textureFile, float cropStartX, float cropStartY, float cropEndX, float cropEndY)
{
	//Set Memeber Variable
	this->shader = shader;
	this->textureFile = L"../../_Textures/" + textureFile;

	//Get SRV
	srv = SpriteSRVManager::GetSRV(this->textureFile);
 	sSrv = shader->AsSRV("DiffuseMap");

	//Get Image Info
	D3DX11_IMAGE_INFO info;
	Check(D3DX11GetImageInfoFromFile(this->textureFile.c_str(), nullptr, &info, nullptr));

	//Make UV Coordinate
	cropStartX = (cropStartX > 0) ? (cropStartX / (float)info.Width) : (0.f);
	cropStartY = (cropStartY > 0) ? (cropStartY / (float)info.Height) : (0.f);
	cropEndX = (cropEndX > 0) ? (cropEndX / (float)info.Width) : (1.f);
	cropEndY = (cropEndY > 0) ? (cropEndY / (float)info.Height) : (1.f);

	//Vertex Data
	Vertex vertices[4];
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.f);
	vertices[3].Position = Vector3(+0.5f, +0.5f, 0.f);

	vertices[0].Uv = Vector2(cropStartX,	cropEndY);
	vertices[1].Uv = Vector2(cropStartX,	cropStartY);
	vertices[2].Uv = Vector2(cropEndX,		cropEndY);
	vertices[3].Uv = Vector2(cropEndX,		cropStartY);

	//Save Cropped Texture Size
	float width = (cropEndX > 0) ? (cropEndX * (float)info.Width) : ((float)info.Width);
	width -= cropStartX * (float)info.Width;

	float height = (cropEndY > 0) ? (cropEndY * (float)info.Height) : ((float)info.Height);
	height -= cropStartY * (float)info.Height;

	textureSize = Vector2(width, height);

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(Device->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	//Create Index Buffer
	{
		UINT indices[] = { 0, 1, 2, 2, 1, 3 };

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		Check(Device->CreateBuffer(&desc, &subResource, &indexBuffer));
	}

	//Make World
	sWorld = this->shader->AsMatrix("World");
	UpdateWorld();
}

Sprite::~Sprite()
{
	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);

	SpriteSRVManager::RemoveSRV(textureFile);
}

void Sprite::Update()
{
	
}

void Sprite::Render()
{
	//Transform
	sWorld->SetMatrix(world);

	//DiffuseMap
	sSrv->SetResource(srv);

	//Draw Call
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, pass, 6);
}

void Sprite::Position(float x, float y)
{
	Position(Vector2(x, y));
}

void Sprite::Position(Vector2 vec)
{
	position = vec;

	UpdateWorld();
}

void Sprite::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}

void Sprite::Scale(Vector2 vec)
{
	scale = vec;

	UpdateWorld();
}

void Sprite::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}

void Sprite::Rotation(Vector3& vec)
{
	rotation = vec;

	UpdateWorld();
}

void Sprite::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void Sprite::RotationDegree(Vector3& vec)
{
	vec.x = Math::ToRadian(vec.x);
	vec.y = Math::ToRadian(vec.y);
	vec.z = Math::ToRadian(vec.z);

	Rotation(vec);
}

Vector3 Sprite::RotationDegree()
{
	Vector3 degree;
	degree.x = Math::ToDegree(rotation.x);
	degree.y = Math::ToDegree(rotation.y);
	degree.z = Math::ToDegree(rotation.z);

	return degree;
}

Vector3& Sprite::Right()
{
	Vector3 right;
	D3DXVec3Normalize(&right, &Vector3(world._11, world._12, world._13));
	return right;
}

Vector3& Sprite::Up()
{
	Vector3 up;
	D3DXVec3Normalize(&up, &Vector3(world._21, world._22, world._23));
	return up;
}

Vector3& Sprite::Forward()
{
	Vector3 forward;
	D3DXVec3Normalize(&forward, &Vector3(world._31, world._32, world._33));
	return forward;
}

void Sprite::UpdateWorld()
{
	Matrix S, R, T;
	D3DXMatrixScaling(&S, textureSize.x * scale.x, textureSize.y * scale.y, 1.f);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, 1.f);

	world = S * R * T;

	sWorld->SetMatrix(world);
}

//-------------------------------------------------------------------------------
// Sprite SRV Manager
//-------------------------------------------------------------------------------
map<wstring, SpriteSRVManager::SRVDesc> SpriteSRVManager::srvMap;

ID3D11ShaderResourceView* SpriteSRVManager::GetSRV(wstring textureFile)
{
	//Alread Created SRV
	if (srvMap.count(textureFile) > 0)
	{
		srvMap[textureFile].ReferenceCount++;

		return srvMap[textureFile].SRV;
	}

	//First Created
	ID3D11ShaderResourceView* srv;
	Check(D3DX11CreateShaderResourceViewFromFile
	(
		Device,
		textureFile.c_str(),
		nullptr,
		nullptr,
		&srv,
		nullptr
	));

	SRVDesc desc;
	desc.ReferenceCount++;
	desc.SRV = srv;

	srvMap[textureFile] = desc;

	return desc.SRV;
}

void SpriteSRVManager::RemoveSRV(wstring textureFile)
{
	UINT count = srvMap.count(textureFile);
	assert(count > 0);

	srvMap[textureFile].ReferenceCount--;

	if (srvMap[textureFile].ReferenceCount < 1)
	{
		SafeRelease(srvMap[textureFile].SRV);
		srvMap.erase(textureFile);
	}
}
