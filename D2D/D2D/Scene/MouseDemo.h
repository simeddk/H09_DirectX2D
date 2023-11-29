#pragma once

#include "Scene.h"

class MouseDemo : public Scene
{
public:
	MouseDemo();
	~MouseDemo();

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