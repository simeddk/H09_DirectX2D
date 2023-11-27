#include "stdafx.h"
#include "System/Device.h"

#include "Scene/Scene.h"
#include "Scene/Scene01Demo.h"
#include "Scene/Scene02Demo.h"

vector<Scene*> scenes;
UINT sceneIndex = 0;

void InitScene()
{
	scenes.push_back(new Scene01Demo());
	scenes.push_back(new Scene02Demo());
}

void DestroyScene()
{
	for (Scene* scene : scenes)
		SafeDelete(scene);
}


void Update()
{
	const char* sceneList[] = 
	{
		typeid((Scene01Demo*)scenes[0]).name(),
		typeid((Scene02Demo*)scenes[1]).name()
	};
	ImGui::Combo("Scene", (int*)&sceneIndex, sceneList, 2);
	scenes[sceneIndex]->Update();

	//for (Scene* scene : scenes)
	//	scene->Update();
}

void Render()
{
	//for (Scene* scene : scenes)
	//	scene->Render();

	scenes[sceneIndex]->Render();
}

void PostRender()
{
	//FPS
	RECT rect = { 0, 0, 800, 200 };
	wstring text = L"FPS : " + to_wstring(ImGui::GetIO().Framerate);
	DirectWirte::RenderText(text, rect);
}
