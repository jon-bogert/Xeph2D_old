#include "Editor/EditorWindow.h"

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
