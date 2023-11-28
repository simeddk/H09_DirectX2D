#pragma once

#include "Scene.h"

class AabbDemo : public Scene
{
public:
	AabbDemo();
	~AabbDemo();

	void Update() override;
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

private:
	Shader* shader = nullptr;

	class Marco* marco = nullptr;
	class Marker* marker = nullptr;
	class Fire* fire = nullptr;

	Matrix volumeMatrix;
	Collider* volumeCollider = nullptr;

	Gizmo* gizmo = nullptr;
};