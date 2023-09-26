#ifdef _EDITOR
#include "Editor/EditorWindows/Viewport.h"
#include "Systems/WindowManager.h"
#include <imgui-SFML.h>

using namespace Xeph2D;

void Xeph2D::Edit::Viewport::Initialize()
{
	name = "Viewport";
}

void Xeph2D::Edit::Viewport::OnGUI()
{
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - WindowManager::__Viewport()->getSize().x) * 0.5f);
	ImGui::SetCursorPosY((ImGui::GetWindowSize().y - WindowManager::__Viewport()->getSize().y + 20) * 0.5f);

	ImVec2 windowPos = ImGui::GetWindowPos(); // get the position of the current window
	ImVec2 mousePos = ImGui::GetMousePos(); // get the global mouse position
	_mouseInViewport = ImVec2(mousePos.x - windowPos.x - ImGui::GetCursorPosX(), mousePos.y - windowPos.y - ImGui::GetCursorPosY()); // compute the mouse position relative to the window
	
	ImGui::Image(*WindowManager::__Viewport());

	if (Vector2(ImGui::GetWindowSize()) != _size)
	{
		_size = ImGui::GetWindowSize();
		WindowManager::__UpdateViewportSize(_size - Vector2(10.f, 30.f));
	}
}

Vector2 Xeph2D::Edit::Viewport::GetMousePos() const
{
	return _mouseInViewport;
}

#endif //_EDITOR