#pragma once

#include "Scene.h"

class ObbDemo : public Scene
{
public:
	ObbDemo();
	~ObbDemo();

	void Update() override;
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

private:
	Shader* shader = nullptr;

	class Marco* marco = nullptr;
	class Fire* fire = nullptr;
};