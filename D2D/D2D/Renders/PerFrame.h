#pragma once

class PerFrame
{
public:
	PerFrame(Shader* shader);
	~PerFrame();

	void Update();
	void Render();

private:
	struct Desc
	{
		Matrix View;
		Matrix Projection;
	} desc;

private:
	Shader* shader = nullptr;

	ID3D11Buffer* constantBuffer = nullptr;
	ID3DX11EffectConstantBuffer* sConstantBuffer;

};
