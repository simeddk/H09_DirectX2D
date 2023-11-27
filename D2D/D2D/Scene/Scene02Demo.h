#pragma once

#include "Scene.h"

class Scene02Demo : public Scene
{
public:
	Scene02Demo();
	~Scene02Demo();

	void Update() override;
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

private:
	Shader* shader = nullptr;
	PerFrame* perFrame = nullptr;

	Sprite* background = nullptr;
};