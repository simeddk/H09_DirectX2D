#pragma once

class Collider
{
public:
	Collider();
	~Collider();

	void Update(Matrix& world);
	void Render();

	void SetHit() { bHitted = true; }
	void SetMiss() { bHitted = false; }
	bool IsHitted() { return bHitted; }

private:
	struct Vertex
	{
		Vector3 Position;
	};

private:
	Shader* shader;

	ID3D11Buffer* vertexBuffer;

	Matrix world;
	ID3DX11EffectMatrixVariable* sWorld;

	bool bHitted = false;
	ID3DX11EffectVectorVariable* sColor;

	PerFrame* perFrame;
};