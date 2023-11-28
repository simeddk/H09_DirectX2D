#include "stdafx.h"
#include "Gizmo.h"
#include "Renders/Sprite.h"

void Gizmo::Set(Sprite* sprite, Vector2* outScale, Vector3* outRotation, Vector2* outPosition, int id)
{
	ImGuizmo::SetOrthographic(true);
	ImGuizmo::BeginFrame();
	ImGuizmo::SetID(id);

	if (Key->Press(VK_LCONTROL))
	{
		if (Key->Down('W'))
			operation = ImGuizmo::TRANSLATE;
		else if (Key->Down('E'))
			operation = ImGuizmo::ROTATE;
		else if (Key->Down('R'))
			operation = ImGuizmo::SCALE;
		else if (Key->Down(VK_OEM_3)) //~
			mode = (ImGuizmo::MODE)!mode;
	}

	ImGuizmo::SetRect(0, 0, (float)Width, (float)Height);

	Matrix V = Context::Get()->View();
	Matrix P = Context::Get()->Projection();
	ImGuizmo::Manipulate(V, P, operation, mode, sprite->GetWorldMatrix());

	Vector3 s, r, t;
	Math::MatrixDecompose(sprite->GetWorldMatrix(), s, r, t);

	*outRotation = r;
	*outPosition = Vector2(t.x, t.y);
	*outScale = Vector2(s.x / sprite->GetTextureSize().x, s.y / sprite->GetTextureSize().y);
}
