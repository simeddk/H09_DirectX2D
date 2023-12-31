#include "stdafx.h"
#include "System/Device.h"

#include "Scene/Scene.h"
#include "Scene/Scene01Demo.h"
#include "Scene/Scene02Demo.h"
#include "Scene/AabbDemo.h"
#include "Scene/ObbDemo.h"
#include "Scene/ScreenSpaceDemo.h"
#include "Scene/MouseDemo.h"
#include "Scene/SpawnDemo.h"
#include "Scene/FileDemo.h"
#include "Scene/FileDialogDemo.h"

vector<Scene*> scenes;

void InitScene()
{
	scenes.push_back(new FileDialogDemo());
}

void DestroyScene()
{
	for (Scene* scene : scenes)
		SafeDelete(scene);
}


void Update()
{
	for (Scene* scene : scenes)
		scene->Update();
}

void Render()
{
	for (Scene* scene : scenes)
		scene->Render();
}

void PostRender()
{
	//FPS
	RECT rect = { 0, 0, 800, 200 };
	wstring text = L"FPS : " + to_wstring(ImGui::GetIO().Framerate);
	DirectWirte::RenderText(text, rect);
}
