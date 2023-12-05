#include "stdafx.h"
#include "FileDialogDemo.h"
#include "Objects/Rect.h"
#include "Objects/Marco.h"
#include "Objects/Marker.h"
#include "Utilities/Xml.h"
#include "Utilities/BinaryFile.h"

FileDialogDemo::FileDialogDemo()
{
	shader = new Shader(L"04_Sprite.fx");
	perFrame = new PerFrame(shader);

	background = new Sprite(shader, L"Background/SonicStage.png");
}

FileDialogDemo::~FileDialogDemo()
{
	SafeDelete(shader);
	SafeDelete(perFrame);
	SafeDelete(background);
	
	for (auto marker : markers)
		SafeDelete(marker);
}

void FileDialogDemo::Update()
{
	Vector2 wPosition = Mouse->Position_World();

	//Spawn Marker
	{
		if (Mouse->Press(LButton))
			markers.push_back(new Marker(shader, wPosition));

		if (Key->Down(VK_ESCAPE))
			markers.clear();
	}

	//Save & Load Markers
	{
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::CollapsingHeader("Markers", ImGuiTreeNodeFlags_DefaultOpen))
		{
			function<void(wstring)> onOpenCompleted;
			onOpenCompleted = bind(&FileDialogDemo::OnOpenCompleted, this, placeholders::_1);

			if (ImGui::Button("Save Bin"))
				SaveAsBinary();
			ImGui::SameLine();

			if (ImGui::Button("Load Bin"))
				LoadAsBinary();

			if (ImGui::Button("Save"))
			{
				function<void(wstring)> onSaveCompleted;
				onSaveCompleted = bind(&FileDialogDemo::OnSaveCompleted, this, placeholders::_1);

				Path::SaveFileDialog
				(
					L"",
					L"Binary Files(bin)\0*.bin",
					L"../../_Datas/",
					onSaveCompleted,
					Hwnd
				);
			}

			ImGui::SameLine();
			if (ImGui::Button("Load"))
			{
				function<void(wstring)> onOpenCompleted;
				onOpenCompleted = bind(&FileDialogDemo::OnOpenCompleted, this, placeholders::_1);

				Path::OpenFileDialog
				(
					L"",
					L"Binary Files(bin)\0*.bin",
					L"../../_Datas/",
					onOpenCompleted,
					Hwnd
				);
			}
		}
	}

	perFrame->Update();
	background->Update();

	for (auto marker : markers)
		marker->Update();
}

void FileDialogDemo::Render()
{
	perFrame->Render();
	background->Render();

	for (auto marker : markers)
		marker->Render();
}

void FileDialogDemo::SaveAsBinary()
{
	BinaryWriter* w = new BinaryWriter(L"../../_Datas/Markers.bin");

	w->UInt(markers.size());

	for (size_t i = 0; i < markers.size(); i++)
	{
		w->UInt(i);
		w->Vector2(markers[i]->Position());
	}

	SafeDelete(w);
}

void FileDialogDemo::LoadAsBinary()
{
	if (Path::ExistFile(L"../../_Datas/Markers.bin") == false)
	{
		MessageBox(Hwnd, L"파일 없음...", L"Error", MB_OK);
		return;
	}

	markers.clear();

	Performance performace;
	performace.Start();

	BinaryReader* r = new BinaryReader(L"../../_Datas/Markers.bin");

	UINT count = r->UInt();
	for (size_t i = 0; i < count; i++)
	{
		UINT index = r->UInt();
		Vector2 position = r->Vector2();

		markers.push_back(new Marker(shader, position));
	}

	SafeDelete(r);

	float elapsed = performace.End();
	MessageBox(Hwnd, to_wstring(elapsed).c_str(), L"Bin Loaded", MB_OK);


}

void FileDialogDemo::OnSaveCompleted(wstring path)
{
	BinaryWriter* w = new BinaryWriter(path + L".bin");

	w->UInt(markers.size());

	for (size_t i = 0; i < markers.size(); i++)
	{
		w->UInt(i);
		w->Vector2(markers[i]->Position());
	}

	SafeDelete(w);

	wstring msg = path + L".bin 파일을 저장했습니다.";
	MessageBox(Hwnd, msg.c_str(), L"저장 완료", MB_OK);
}

void FileDialogDemo::OnOpenCompleted(wstring path)
{
	markers.clear();

	Performance performace;
	performace.Start();

	BinaryReader* r = new BinaryReader(path);

	UINT count = r->UInt();
	for (size_t i = 0; i < count; i++)
	{
		UINT index = r->UInt();
		Vector2 position = r->Vector2();

		markers.push_back(new Marker(shader, position));
	}

	SafeDelete(r);

	float elapsed = performace.End();
	MessageBox(Hwnd, to_wstring(elapsed).c_str(), L"Bin Loaded", MB_OK);
}
