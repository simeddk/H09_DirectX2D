#pragma once

#include "Scene.h"

class Scene01Demo : public Scene
{
public:
	Scene01Demo();
	~Scene01Demo();

	void Update() override;
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

private:
	Shader* shader = nullptr;
	PerFrame* perFrame = nullptr;

	Sprite* cloud[2];
	Sprite* bush[2];
	Sprite* tile;

	Shader* rectShader = nullptr;
	class Rect* rect = nullptr;

	class Marco* marco = nullptr;
};