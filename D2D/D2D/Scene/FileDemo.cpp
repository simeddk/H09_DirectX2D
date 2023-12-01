#include "stdafx.h"
#include "FileDemo.h"
#include "Objects/Rect.h"
#include "Objects/Marco.h"
#include "Objects/Marker.h"
#include "Utilities/Xml.h"

FileDemo::FileDemo()
{
	shader = new Shader(L"04_Sprite.fx");
	perFrame = new PerFrame(shader);

	background = new Sprite(shader, L"Background/SonicStage.png");
	
	rectShader = new Shader(L"05_Rect.fx");
	rect = new Rect(rectShader, Vector2(0, 0), Vector2(100, 100));

	rectPosition = rect->Position();
	rectColor = rect->Color();
}

FileDemo::~FileDemo()
{
	SafeDelete(shader);
	SafeDelete(perFrame);
	SafeDelete(background);

	SafeDelete(rectShader);
	SafeDelete(rect);

	for (auto marker : markers)
		SafeDelete(marker);
}

void FileDemo::Update()
{
	Vector2 wPosition = Mouse->Position_World();

	//Rect Move
	{
		bool bPicked = true;
		bPicked &= Collider::Aabb(rect->World(), wPosition);
		bPicked ? rect->GetCollider()->SetHit() : rect->GetCollider()->SetMiss();

		bPicked &= Mouse->Press(MButton);
		if (bPicked)
			rect->Position(wPosition);
	}

	//Rect Color & Save & Load
	{
		if (ImGui::CollapsingHeader("Rect", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::LabelText("Position", "%.2f, %.2f", rect->Position().x, rect->Position().y);
			
			ImGui::ColorEdit3("Color", rectColor);
			rect->Color(rectColor);

			if (ImGui::Button("Save As CSV"))
				SaveAsCSV();
			ImGui::SameLine();

			if (ImGui::Button("Load As CSV"))
				LoadAsCSV();
		}
	}

	//Spawn Marker
	{
		if (Mouse->Down(LButton))
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
			if (ImGui::Button("Save Markers"))
				SaveMarkers();
			ImGui::SameLine();

			if (ImGui::Button("Load Markers"))
				LoadMarkers();
		}
	}

	//Save & Load Background(as XML)
	{
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::CollapsingHeader("Background", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Button("Save Xml"))
				SaveAsXml();
			ImGui::SameLine();

			if (ImGui::Button("Load Xml"))
				LoadAsXml();
		}
	}

	perFrame->Update();
	background->Update();

	rect->Update();

	for (auto marker : markers)
		marker->Update();
}

void FileDemo::Render()
{
	perFrame->Render();
	background->Render();

	rect->Render();

	for (auto marker : markers)
		marker->Render();
}

void FileDemo::SaveAsCSV()
{
	Path::CreateFolders("../../_Datas/");

	FILE* buffer = nullptr;
	fopen_s(&buffer, "../../_Datas/Rect.csv", "w");
	
	fprintf
	(
		buffer,
		"%f,%f,%f,%f,%f",
		rect->Position().x,
		rect->Position().y,
		rect->Color().r,
		rect->Color().g,
		rect->Color().b
	);

	fclose(buffer);
}

void FileDemo::LoadAsCSV()
{
	FILE* buffer = nullptr;
	fopen_s(&buffer, "../../_Datas/Rect.csv", "r");

	fscanf_s
	(
		buffer,
		"%f,%f,%f,%f,%f",
		&rectPosition.x,
		&rectPosition.y,
		&rectColor.r,
		&rectColor.g,
		&rectColor.b
	);

	fclose(buffer);

	rect->Position(rectPosition);
	rect->Color(rectColor);
}

void FileDemo::SaveMarkers()
{
	Path::CreateFolders("../../_Datas/");

	FILE* buffer = nullptr;
	fopen_s(&buffer, "../../_Datas/Markers.txt", "w");

	fprintf(buffer, "%d\n", markers.size());

	for (int i = 0; i < markers.size(); i++)
	{
		fprintf
		(
			buffer,
			"%d, %.2f, %.2f\n",
			i,
			markers[i]->Position().x,
			markers[i]->Position().y
		);
	}

	fclose(buffer);
}

void FileDemo::LoadMarkers()
{
	if (Path::ExistFile("../../_Datas/Markers.txt") == false)
	{
		MessageBox(Hwnd, L"파일을 찾을 수 없습니다.", L"파일 로드 오류", MB_OK);
		return;
	}

	markers.clear();

	FILE* buffer = nullptr;
	fopen_s(&buffer, "../../_Datas/Markers.txt", "r");

	int count;
	fscanf_s(buffer, "%d", &count);

	for (int i = 0; i < count; i++)
	{
		int index;
		Vector2 position;

		fscanf_s
		(
			buffer,
			"%d,%f,%f",
			&index,
			&position.x,
			&position.y
		);

		markers.push_back(new Marker(shader, position));
	}

	fclose(buffer);

	rect->Position(rectPosition);
	rect->Color(rectColor);
}

void FileDemo::SaveAsXml()
{
	Xml::XMLDocument* document = new Xml::XMLDocument();
	Xml::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	Xml::XMLElement* root = document->NewElement("Background");
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);

	Xml::XMLElement* node = document->NewElement("Texture");
	root->LinkEndChild(node);

	Xml::XMLElement* element = nullptr;
	
	element = document->NewElement("Name");
	wstring onlyFilePath = background->GetTextureFile();
	String::Replace(&onlyFilePath, L"../../_Textures/Background/", L"");
	element->SetText(String::ToString(onlyFilePath).c_str());
	node->LinkEndChild(element);

	element = document->NewElement("Width");
	element->SetText(background->GetTextureSize().x);
	node->LinkEndChild(element);

	element = document->NewElement("Height");
	element->SetText(background->GetTextureSize().y);
	node->LinkEndChild(element);

	document->SaveFile("../../_Datas/Background.xml");
	SafeDelete(document);
}

void FileDemo::LoadAsXml()
{
	Xml::XMLDocument* document = new Xml::XMLDocument();
	Xml::XMLError error = document->LoadFile("../../_Datas/Background.xml");
	assert(error == Xml::XML_SUCCESS);

	Xml::XMLElement* root = document->FirstChildElement();
	Xml::XMLElement* texture = root->FirstChildElement();

	Xml::XMLElement* element = texture->FirstChildElement();
	string name = element->GetText();

	element = element->NextSiblingElement();
	int width = element->IntText();

	element = element->NextSiblingElement();
	int height = element->IntText();

	SafeDelete(document);

	wstring msg = L"FileName : ";
	msg += String::ToWString(name);
	msg += L"\n";

	msg += L"Width : ";
	msg += to_wstring(width);
	msg += L"\n";

	msg += L"Height : ";
	msg += to_wstring(height);

	MessageBox(Hwnd, msg.c_str(), L"Background Information", MB_OK);
}
