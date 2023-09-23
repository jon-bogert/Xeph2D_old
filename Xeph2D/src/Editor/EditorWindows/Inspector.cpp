#ifdef _EDITOR
#include "Editor/EditorWindows/Inspector.h"

void Xeph2D::Edit::Inspector::Initialize()
{
	name = "Inspector";
}

void Xeph2D::Edit::Inspector::OnGUI()
{
	ImGui::Text("Transform");
	ImVec2 v2; float f = 0.f;
	ImGui::DragFloat2("Position", &v2.x);
	ImGui::DragFloat("Rotation", &f);
	ImGui::DragFloat2("Scale", &v2.x);
}
#endif //_EDITOR
