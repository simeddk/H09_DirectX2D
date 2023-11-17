#include "stdafx.h"
#include "System/Device.h"
#include "Objects/Background.h"
#include "Objects/Marco.h"

Shader* shader = nullptr;
Background* background = nullptr;
Marco* marco = nullptr;
Sprite* sprite = nullptr;

void InitScene()
{
	shader = new Shader(L"04_Sprite.fx");
	PerFrame::Get()->SetShader(shader);

	background = new Background(shader);
	marco = new Marco(shader, Vector2(200, 170), Vector2(2.5f, 2.5f));
	sprite = new Sprite(shader, L"Mario.png");
	sprite->Position(400, 400);
	sprite->Scale(1.5f, 1.5f);
}

void DestroyScene()
{
	SafeDelete(shader);
	SafeDelete(background);
	SafeDelete(marco);
	SafeDelete(sprite);
}


void Update()
{
	//Rotation Test(Manual)
	static int value = 2;
	{
		ImGui::RadioButton("Pitch(x)", &value, 0);	ImGui::SameLine();
		ImGui::RadioButton("Yaw(y)", &value, 1);	ImGui::SameLine();
		ImGui::RadioButton("Roll(z)", &value, 2);

		Vector3 rotation = sprite->RotationDegree();

		if (Key->Press(VK_LEFT))
		{
			switch (value)
			{
			case 0: rotation.x -= 400.f * Time::Delta(); break; //Pitch(x)
			case 1: rotation.y -= 400.f * Time::Delta(); break; //Yaw(y)
			case 2: rotation.z -= 400.f * Time::Delta(); break; //Roll(z)
			}
		}
		else if (Key->Press(VK_RIGHT))
		{
			switch (value)
			{
			case 0: rotation.x += 400.f * Time::Delta(); break; //Pitch(x)
			case 1: rotation.y += 400.f * Time::Delta(); break; //Yaw(y)
			case 2: rotation.z += 400.f * Time::Delta(); break; //Roll(z)
			}
		}

		sprite->RotationDegree(rotation);
	}

	//Rotation Test(Auto)
	{
		static bool bAutoRotation = true;
		ImGui::Checkbox("Auto Rotation", &bAutoRotation);

		if (bAutoRotation)
		{
			switch (value)
			{
			case 0: sprite->Rotation(sinf(Time::Get()->Running()) * Math::PI, 0, 0); break; //Pitch(x)
			case 1: sprite->Rotation(0, sinf(Time::Get()->Running()) * Math::PI, 0); break; //Yaw(y)
			case 2: sprite->Rotation(0, 0, sinf(Time::Get()->Running()) * Math::PI);;break; //Roll(z)
			}
		}
	}

	background->Update();
	marco->Update();
	sprite->Update();
}

void Render()
{
	//Draw Debug Value
	if (ImGui::CollapsingHeader("Rotation(Degree)", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::LabelText
		(
			"Degree",
			"%.2f, %.2f, %.2f",
			sprite->RotationDegree().x,
			sprite->RotationDegree().y,
			sprite->RotationDegree().z
		);
	}

	if (ImGui::CollapsingHeader("World Matrix", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::LabelText("_11, _12, _13", "%.2f, %.2f, %.2f", sprite->GetWorldMatrix()._11, sprite->GetWorldMatrix()._12, sprite->GetWorldMatrix()._13);
		ImGui::LabelText("_21, _22, _23", "%.2f, %.2f, %.2f", sprite->GetWorldMatrix()._21, sprite->GetWorldMatrix()._22, sprite->GetWorldMatrix()._23);
		ImGui::LabelText("_31, _32, _33", "%.2f, %.2f, %.2f", sprite->GetWorldMatrix()._31, sprite->GetWorldMatrix()._32, sprite->GetWorldMatrix()._33);
	}

	if (ImGui::CollapsingHeader("Direction", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::LabelText("Right", "%.2f, %.2f, %.2f",	sprite->Right().x,		sprite->Right().y,		sprite->Right().z);
		ImGui::LabelText("Up", "%.2f, %.2f, %.2f",		sprite->Up().x,			sprite->Up().y,			sprite->Up().z);
		ImGui::LabelText("Forward", "%.2f, %.2f, %.2f", sprite->Forward().x,	sprite->Forward().y,	sprite->Forward().z);
	}


	background->Render();
	marco->Render();
	sprite->Render();
}