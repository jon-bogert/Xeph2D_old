#ifdef _EDITOR
#include "Editor/EditorWindow.h"

void Xeph2D::Edit::EditorWindow::UpdateValues()
{
	_isFocused = ImGui::IsWindowFocused();
	_isHovered = ImGui::IsWindowHovered();
}

const char* Xeph2D::Edit::EditorWindow::GetName() const
{
	return name.c_str();
}

//ImGuiWindowFlags Xeph2D::Edit::EditorWindow::GetFlags() const
//{
//	return flags;
//}

bool Xeph2D::Edit::EditorWindow::IsOpen() const
{
	return isOpen;
}

void Xeph2D::Edit::EditorWindow::Close()
{
	isOpen = false;
}

void Xeph2D::Edit::EditorWindow::Open()
{
	isOpen = true;
}

bool Xeph2D::Edit::EditorWindow::IsFocused() const
{
	return _isFocused;
}

bool Xeph2D::Edit::EditorWindow::IsHovered() const
{
	return _isHovered;
}

#endif //_EDITOR
