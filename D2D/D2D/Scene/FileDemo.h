#pragma once

#include "Scene.h"

class FileDemo : public Scene
{
public:
	FileDemo();
	~FileDemo();

	void Update() override;
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

private:
	void SaveAsCSV();
	void LoadAsCSV();

	void  SaveMarkers();
	void  LoadMarkers();
	

private:
	Shader* shader = nullptr;
	PerFrame* perFrame = nullptr;

	Sprite* background = nullptr;

	class Rect* rect = nullptr;
	Shader* rectShader = nullptr;

	vector<class Marker*> markers;

	Vector2 rectPosition;
	Color rectColor;
};