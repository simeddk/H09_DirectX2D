#pragma once

#include "Scene.h"

class FileDialogDemo : public Scene
{
public:
	FileDialogDemo();
	~FileDialogDemo();

	void Update() override;
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

private:
	void SaveAsBinary();
	void LoadAsBinary();
	
	void OnOpenCompleted(wstring path);

private:
	Shader* shader = nullptr;
	PerFrame* perFrame = nullptr;

	Sprite* background = nullptr;
	vector<class Marker*> markers;
};