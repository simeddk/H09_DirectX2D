#pragma once

//-------------------------------------------------------------------------------
// Sprite
//-------------------------------------------------------------------------------
class Sprite
{
public: 
	Sprite(Shader* shader, wstring textureFile);
	Sprite(Shader* shader, wstring textureFile, float cropEndX, float cropEndY);
	Sprite(Shader* shader, wstring textureFile, float cropStartX, float cropStartY, float cropEndX, float cropEndY);
	~Sprite();

private:
	void Initialize(Shader* shader, wstring textureFile, float cropStartX, float cropStartY, float cropEndX, float cropEndY);
	void UpdateWorld();

public:
	void Update();
	void Render();

	bool IsUpdated() { return bUpdated; }

	void Pass(UINT pass) { this->pass = pass; }
	Shader* GetShader() { return shader; }
	Vector2 GetTextureSize() { return textureSize; }
	Matrix World() { return world; }

	wstring GetTextureFile() { return textureFile; }

public:
	void Position(float x, float y);
	void Position(Vector2 vec);
	Vector2 Position() { return position; }

	void Scale(float x, float y);
	void Scale(Vector2 vec);
	Vector2 Scale() { return scale; }

	void Rotation(float x, float y, float z);
	void Rotation(Vector3& vec);
	Vector3 Rotation() { return rotation; }

	void RotationDegree(float x, float y, float z);
	void RotationDegree(Vector3& vec);
	Vector3 RotationDegree();

	Matrix& GetWorldMatrix() { return world; }

	Vector3& Right();
	Vector3& Up();
	Vector3& Forward();

private:
	struct Vertex
	{
		Vector3 Position;
		Vector2 Uv;
	};

private:
	Shader* shader;
	UINT pass = 0;

	wstring textureFile;
	ID3D11ShaderResourceView* srv;
	ID3DX11EffectShaderResourceVariable* sSrv;
	Vector2 textureSize;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	Vector2 position = Vector2(0, 0);
	Vector2 scale = Vector2(1, 1);
	Vector3 rotation = Vector3(0, 0, 0);

	Matrix world;
	ID3DX11EffectMatrixVariable* sWorld;

	bool bUpdated = false;
};

//-------------------------------------------------------------------------------
// Sprite SRV Manager
//-------------------------------------------------------------------------------
class SpriteSRVManager
{
private:
	friend class Sprite;

private:
	static ID3D11ShaderResourceView* GetSRV(wstring textureFile);
	static void RemoveSRV(wstring textureFile);

private:
	struct SRVDesc
	{
		UINT ReferenceCount = 0;
		ID3D11ShaderResourceView* SRV = nullptr;
	};

private:
	static map<wstring, SRVDesc> srvMap;
};