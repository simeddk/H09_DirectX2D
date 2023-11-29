#pragma once

#include "Scene.h"

class ScreenSpaceDemo : public Scene
{
public:
	ScreenSpaceDemo();
	~ScreenSpaceDemo();

	void Update() override;
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

private:
	Shader* shader = nullptr;
	PerFrame* perFrame = nullptr;

	Sprite* background = nullptr;

	class Rect* rect = nullptr;
	Shader* rectShader = nullptr;
};