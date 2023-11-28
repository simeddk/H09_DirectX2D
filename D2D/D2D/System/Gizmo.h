#pragma once

class Gizmo
{
public:
	void Set(class Sprite* sprite, Vector2* outScale, Vector3* outRotation, Vector2* outPosition, int id = 0);

private:
	ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE mode = ImGuizmo::WORLD;
};