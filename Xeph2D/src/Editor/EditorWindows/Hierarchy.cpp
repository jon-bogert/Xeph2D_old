#ifdef _EDITOR
#include "Editor/EditorWindows/Hierarchy.h"

#include "Systems/Debug.h"
#include "Systems/SceneManager.h"
#include "Systems/Serializer.h"

#include "Editor/Editor.h"

#include <string>
#include <vector>

using namespace Xeph2D;
using namespace Xeph2D::Edit;

namespace
{
	std::vector<const char*> CStrVect(const std::vector<std::string>& input)
	{
		std::vector<const char*> inputCStr;
		for (const std::string& item : input)
		{
			inputCStr.push_back(item.c_str());
		}
		return inputCStr;
	}
}

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
	if (ImGui::ListBox("##HItems", &_selectionIndex, CStrVect(itemNames).data(), itemNames.size()))
	{
		Editor::GetInspectorWindow()->SetGameObject(objects[_selectionIndex]);
	}
	
	if (ImGui::Button("+##Hier"))
	{
		Debug::Log("Add GameObject");
	}
	ImGui::SameLine();
	if (ImGui::Button("-##Hier"))
	{
		Debug::Log("Remove GameObject");
	}
	ImGui::SameLine();
	if (ImGui::Button("^##Hier"))
	{
		Debug::Log("GameObject Up");
	}
	ImGui::SameLine();
	if (ImGui::Button("v##Hier"))
	{
		Debug::Log("GameObject Down");
	}
}

#endif //_EDITOR