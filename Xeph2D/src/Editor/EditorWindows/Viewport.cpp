#include "Editor/EditorWindows/Viewport.h"

void Xeph2D::Edit::Viewport::Initialize()
{
	name = "Viewport";
}

void Xeph2D::Edit::Viewport::OnGUI()
{
	ImGui::Text("Hello Viewport");
}