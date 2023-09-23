#ifdef _EDITOR
#include "Editor/EditorWindows/Viewport.h"
#include "Systems/WindowManager.h"
#include <imgui-SFML.h>

void Xeph2D::Edit::Viewport::Initialize()
{
	name = "Viewport";
}

void Xeph2D::Edit::Viewport::OnGUI()
{
	ImGui::Image(WindowManager::__Viewport()->getTexture());
}

#endif //_EDITOR