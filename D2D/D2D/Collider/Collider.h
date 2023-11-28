#pragma once

class Collider
{
private:
	struct ObbDesc;

public:
	Collider();
	~Collider();

	void Update(Matrix& world);
	void Render();

	void SetHit() { bHitted = true; }
	void SetMiss() { bHitted = false; }
	bool IsHitted() { return bHitted; }

public:
	static bool Aabb(Matrix world, Vector2 position);
	static bool Aabb(Matrix world1, Matrix world2);

public:
	static bool Obb(Sprite* a, Sprite* b);

private:
	static void CreateObb(ObbDesc* out, Vector2& half, Matrix& transform);

private:
	struct Vertex
	{
		Vector3 Position;
	};

	struct ObbDesc
	{
		Vector2 HalfSize;

		Vector2 Right;
		Vector2 Up;

		Vector2 RightLength;
		Vector2 UpLength;
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