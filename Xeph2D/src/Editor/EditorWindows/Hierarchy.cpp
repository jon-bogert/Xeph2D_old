#ifdef _EDITOR
#include "Editor/EditorWindows/Hierarchy.h"

#include "Systems/Debug.h"
#include "Systems/SceneManager.h"
#include "Systems/Serializer.h"

#include "Editor/Editor.h"

#include "Utility.h"

#include <string>
#include <vector>

using namespace Xeph2D;
using namespace Xeph2D::Edit;

void Hierarchy::Initialize()
{
	name = "Hierarchy";
}

void Hierarchy::OnGUI()
{
	std::string sceneName = SceneManager::GetCurrentName();
	if (!Editor::GetHasSaved())
		sceneName += " - (not saved)";
	ImGui::Text(sceneName.c_str());
	std::vector<GameObject*> objects = SceneManager::GetCurrentScene()->GetAllObjects();
	std::vector<std::string> itemNames;
	for (GameObject*& obj : objects)
		itemNames.push_back(obj->name);
	if (ImGui::ListBox("##HItems", &m_selectionIndex, Utility::CStrVect(itemNames).data(), itemNames.size()))
	{
		Editor::GetInspectorWindow()->SetGameObject(objects[m_selectionIndex]);
	}
	
	if (ImGui::Button("+##Hier"))
	{
		GameObject* obj = SceneManager::GetCurrentScene()->AddGameObject();
		Serializer::Get().EditorAddGameObject(obj);
		Editor::SetHasSaved(false);
	}
	ImGui::SameLine();
	if (ImGui::Button("-##Hier"))
	{
		if (m_selectionIndex >= 0)
		{
			Serializer::Get().EditorRemoveGameObject(objects[m_selectionIndex]);
			SceneManager::GetCurrentScene()->Destroy(objects[m_selectionIndex]);
			Editor::GetInspectorWindow()->SetGameObject(nullptr);
			Editor::SetHasSaved(false);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("^##Hier"))
	{
		if (m_selectionIndex > 0)
		{
			SceneManager::GetCurrentScene()->MoveUp(m_selectionIndex);
			Editor::SetHasSaved(false);
			--m_selectionIndex;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("v##Hier"))
	{
		if (m_selectionIndex < objects.size() - 1 && m_selectionIndex >= 0)
		{
			SceneManager::GetCurrentScene()->MoveDown(m_selectionIndex);
			Editor::SetHasSaved(false);
			++m_selectionIndex;
		}
	}
}

#endif //_EDITOR