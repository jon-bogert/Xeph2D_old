#ifdef _EDITOR
#include "Editor/EditorWindows/ScriptCreator.h"
#include "Editor/Editor.h"

using namespace Xeph2D::Edit;

void ScriptCreator::Initialize()
{
	isOpen = false;
	name = "Create New Script";
	m_defaultFlags |=
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove;
	flags = m_defaultFlags;
}

void ScriptCreator::OnGUI()
{
	ImGui::SetWindowPos({ (ImGui::GetMainViewport()->Size.x - ImGui::GetWindowWidth()) * 0.5f, (ImGui::GetMainViewport()->Size.y - ImGui::GetWindowHeight()) * 0.5f });
	if (m_isConfirmed)
	{
		ImGui::Text("Please Close the Editor and Recompile\nfor changes to take effect.");
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.f }, "(Remember to add new Script files to\nyour Visual Studio Project)");
		ImGui::NewLine();
		if (ImGui::Button("Close Now"))
		{
			isOpen = false;
			Editor::Close();
		}
		ImGui::SameLine();
		if (ImGui::Button("Close Later"))
		{
			m_isConfirmed = false;
			flags = m_defaultFlags;
		}
		return;
	}
	ImGui::Text("Component Name:");
	ImGui::InputText("##Name", m_nameBuffer, 256);
	ImGui::NewLine();
	ImGui::Text("Path:");
	ImGui::SameLine();
	ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.f }, "(optional)");
	ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.f }, "Assets/Scripts/");
	ImGui::SameLine();
	ImGui::InputText("##Path", m_pathBuffer, 1024);
	ImGui::NewLine();
	if (ImGui::Button("Create"))
	{
		if (strcmp(m_nameBuffer, "") != 0)
		{
			m_isConfirmed = true;
			flags |= ImGuiWindowFlags_NoDecoration;
			std::string pathFull = std::string("Assets/Scripts/") + std::string(m_pathBuffer);
			Editor::GetScriptManager()->CreateNew(m_nameBuffer, pathFull);
			Editor::RebuildProject();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
	{
		isOpen = false;
	}
}

#endif //_EDITOR
