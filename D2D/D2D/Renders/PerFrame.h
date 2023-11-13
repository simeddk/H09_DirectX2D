#pragma once

class PerFrame
{
private:
	PerFrame();
	~PerFrame();

public:
	static void Create();
	static void Delete();
	static PerFrame* Get();

public:
	void SetShader(Shader* shader);

	void Update();
	void Render();

private:
	struct Desc
	{
		Matrix View;
		Matrix Projection;
	} desc;

private:
	static PerFrame* instance;

private:
	Shader* shader = nullptr;

	ID3D11Buffer* constantBuffer = nullptr;
	ID3DX11EffectConstantBuffer* sConstantBuffer;
};