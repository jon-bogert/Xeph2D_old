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
	if (ImGui::ListBox("##HItems", &_selectionIndex, Utility::CStrVect(itemNames).data(), itemNames.size()))
	{
		Editor::GetInspectorWindow()->SetGameObject(objects[_selectionIndex]);
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
		if (_selectionIndex >= 0)
		{
			Serializer::Get().EditorRemoveGameObject(objects[_selectionIndex]);
			SceneManager::GetCurrentScene()->Destroy(objects[_selectionIndex]);
			Editor::GetInspectorWindow()->SetGameObject(nullptr);
			Editor::SetHasSaved(false);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("^##Hier"))
	{
		if (_selectionIndex > 0)
		{
			SceneManager::GetCurrentScene()->MoveUp(_selectionIndex);
			Editor::SetHasSaved(false);
			--_selectionIndex;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("v##Hier"))
	{
		if (_selectionIndex < objects.size() - 1 && _selectionIndex >= 0)
		{
			SceneManager::GetCurrentScene()->MoveDown(_selectionIndex);
			Editor::SetHasSaved(false);
			++_selectionIndex;
		}
	}
}

#endif //_EDITOR