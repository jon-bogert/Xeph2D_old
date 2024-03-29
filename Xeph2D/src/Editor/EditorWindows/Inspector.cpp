#ifdef _EDITOR
#include "Editor/EditorWindows/Inspector.h"
#include "Editor/Editor.h"

#include <GameObject.h>
#include "Utility.h"

#include <regex>

#pragma warning(disable : 4996)

void Xeph2D::Edit::Inspector::Initialize()
{
	name = "Inspector";
}

void Xeph2D::Edit::Inspector::OnGUI()
{
	if (m_objectInfo == nullptr)
		return;

	if (m_showEdit)
	{
		ShowEdit();
		return;
	}

	VarEntry entry;
	//Game Object Variables
	for (Serializer::EdVarEntry& e : m_objectInfo->go_variables)
	{
		entry.displayName = Var2DisplayName(e.name);
		entry.serialized = &e;
		entry.instanced = e.ptr;
		DrawVar(entry);
	}
	for (Serializer::EdComponent& comp : m_objectInfo->components)
	{
		ImGui::NewLine();
		if (ImGui::CollapsingHeader((m_compNames[comp.id] + "##Comp").c_str(),ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (Serializer::EdVarEntry& e : comp.variables)
			{
				entry.displayName = Var2DisplayName(e.name);
				entry.serialized = &e;
				entry.instanced = e.ptr;
				DrawVar(entry);
			}
		}
	}
	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();
	if (ImGui::Button("Edit Components##Insp"))
	{
		m_showEdit = !m_showEdit;
		if (m_showEdit)
			m_editSelection = -1;
	}
}
void Xeph2D::Edit::Inspector::SetGameObject(GameObject* obj)
{
	m_currObject = obj;

	if (m_currObject == nullptr)
	{
		m_objectInfo = nullptr;
		Editor::GetTransformGizmo()->SetCurrentObject(nullptr);
		return;
	}

	Editor::GetTransformGizmo()->SetCurrentObject(obj);

	uint32_t instID = m_currObject->instID;
	m_objectInfo = Serializer::GetDataFromInstance(instID);
	if (m_objectInfo == nullptr)
	{
		Debug::LogErr("Inspector::SetGameObject -> could not get variable data from Serializer, See serializer Error");
	}
}

void Xeph2D::Edit::Inspector::RegisterComponentNames(std::function<std::unordered_map<uint32_t, std::string>(void)> callback)
{
	m_compNames = callback();
}

bool Xeph2D::Edit::Inspector::CompNamesContains(uint32_t id)
{
	return (m_compNames.find(id) != m_compNames.end());
}

#define __CAP_OFFSET 32

std::string Xeph2D::Edit::Inspector::Var2DisplayName(std::string varName)
{
	bool spaceLast = false;
	if (varName.size() <= 2)
		return varName;

	if (varName.substr(0, 2) == "m_")
		varName = varName.substr(2);
	else if (varName[0] == 'm' && varName[1] <= 'Z')
		varName = varName.substr(1);

	while (varName[0] == '_')
		varName = varName.substr(1);

	if (varName[0] >= 'a')
		varName[0] -= __CAP_OFFSET;
	for (size_t i = 1; i < varName.size();)
	{
		if (varName[i] == '_' && varName.size() >= i + 2)
		{
			if (varName[i + 1] == '_')
			{
				varName.erase(i, 1);
				continue;
			}
			varName[i] = ' ';
			if (varName[i + 1] > 'Z')
			{
				varName[i + 1] -= __CAP_OFFSET;
			}
			i += 2;
			spaceLast = true;
			continue;
		}
		else if (varName[i] == '_')
		{
			varName.pop_back();
			continue;
		}

		if (varName[i] <= 'Z' && !spaceLast)
		{
			spaceLast = 0;
			varName.insert(i, 1, ' ');
			i += 2;
			continue;
		}
		else if (varName[i] > 'Z' && spaceLast)
			spaceLast = false;

		++i;
	}

	return varName;
}

void Xeph2D::Edit::Inspector::DrawVar(VarEntry& entry)
{
	switch (entry.serialized->type)
	{
	case Serializer::DataType::Int:
		DrawInt(entry);
		break;
	case Serializer::DataType::Float:
		DrawFloat(entry);
		break;
	case Serializer::DataType::Bool:
		DrawBool(entry);
		break;
	case Serializer::DataType::Char:
		DrawChar(entry);
		break;
	case Serializer::DataType::String:
		DrawString(entry);
		break;
	case Serializer::DataType::Vector2:
		DrawVec2(entry);
		break;
	case Serializer::DataType::Color:
		DrawColor(entry);
		break;
	case Serializer::DataType::Transform:
		DrawTransform(entry);
		break;
	default:
		Debug::LogErr("Inspector::OnGUI -> Data Type not supported");
		break;
	}
}

void Xeph2D::Edit::Inspector::DrawInt(VarEntry& entry)
{
	if (ImGui::InputInt((entry.displayName + "##Insp").c_str(), (int*)entry.instanced))
	{
		entry.serialized->data = *(int*)entry.instanced;
		Editor::SetHasSaved(false);
	}
}

void Xeph2D::Edit::Inspector::DrawFloat(VarEntry& entry)
{
	if (ImGui::InputFloat((entry.displayName + "##Insp").c_str(), (float*)entry.instanced))
	{
		entry.serialized->data = *(float*)entry.instanced;
		Editor::SetHasSaved(false);
	}
}

void Xeph2D::Edit::Inspector::DrawBool(VarEntry& entry)
{
	if (ImGui::Checkbox((entry.displayName + "##Insp").c_str(), (bool*)entry.instanced))
	{
		entry.serialized->data = *(float*)entry.instanced;
		Editor::SetHasSaved(false);
	}
}

void Xeph2D::Edit::Inspector::DrawChar(VarEntry& entry)
{
	char buff = *(char*)entry.instanced;
	if (ImGui::InputText((entry.displayName + "##Insp").c_str(), &buff, 1))
	{
		*(char*)entry.instanced = buff;
		entry.serialized->data = *(float*)entry.instanced;
		Editor::SetHasSaved(false);
	}
}

void Xeph2D::Edit::Inspector::DrawString(VarEntry& entry)
{
	char buff[1024];
	strcpy(buff, (*(std::string*)entry.instanced).c_str());
	if (ImGui::InputText((entry.displayName + "##Insp").c_str(), buff, 1024, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		*(std::string*)entry.instanced = buff;
		entry.serialized->data = *(std::string*)entry.instanced;
		Editor::SetHasSaved(false);
	}
}

void Xeph2D::Edit::Inspector::DrawVec2(VarEntry& entry)
{
	if (ImGui::InputFloat2((entry.displayName + "##Insp").c_str(), (float*)entry.instanced))
	{
		entry.serialized->data = *(Vector2*)entry.instanced;
		Editor::SetHasSaved(false);
	}
}

void Xeph2D::Edit::Inspector::DrawColor(VarEntry& entry)
{
	if (ImGui::ColorPicker4((entry.displayName + "##Insp").c_str(), (float*)entry.instanced))
	{
		entry.serialized->data = *(Color*)entry.instanced;
		Editor::SetHasSaved(false);
	}
}

void Xeph2D::Edit::Inspector::DrawTransform(VarEntry& entry)
{
	ImGui::Text(entry.displayName.c_str());
	if (ImGui::DragFloat2(("Position##" + entry.displayName + "Insp").c_str(), (float*)&((Transform*)entry.instanced)->position))
	{
		entry.serialized->data = *(Transform*)entry.instanced;
		Editor::SetHasSaved(false);
	}
	float rotation = ((Transform*)entry.instanced)->rotation.GetDeg();
	if (ImGui::DragFloat(("Rotation##" + entry.displayName + "Insp").c_str(), &rotation))
	{
		((Transform*)entry.instanced)->rotation.SetDeg(rotation);
		entry.serialized->data = *(Transform*)entry.instanced;
		Editor::SetHasSaved(false);
	}
	if (ImGui::DragFloat2(("Scale##" + entry.displayName + "Insp").c_str(), (float*)&((Transform*)entry.instanced)->scale))
	{
		entry.serialized->data = *(Transform*)entry.instanced;
		Editor::SetHasSaved(false);
	}
}

void Xeph2D::Edit::Inspector::ShowEdit()
{
	std::vector<std::string> itemNames;
	for (const Serializer::EdComponent& c : m_objectInfo->components)
	{
		itemNames.push_back(m_compNames[c.id]);
	}
	if (ImGui::ListBox("##HItems", &m_editSelection, Utility::CStrVect(itemNames).data(), itemNames.size()))
	{
	
	}
	if (ImGui::Button("+##Insp"))
	{
		m_showAdd = !m_showAdd;
	}
	ImGui::SameLine();
	if (ImGui::Button("-##Insp"))
	{
		if (m_editSelection >= 0)
		{
			Serializer::EdComponent& comp = m_objectInfo->components[m_editSelection];
			m_currObject->RemoveComponent(comp.id);
			m_objectInfo->components.erase(m_objectInfo->components.begin() + m_editSelection);
			Editor::SetHasSaved(false);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("^##Insp"))
	{
		if (m_editSelection > 0)
		{
			m_currObject->MoveUp(m_editSelection);
			std::swap(m_objectInfo->components[m_editSelection], m_objectInfo->components[m_editSelection - 1]);
			Editor::SetHasSaved(false);
			--m_editSelection;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("v##Insp"))
	{
		if (m_editSelection < itemNames.size() - 1 && m_editSelection >= 0)
		{
			m_currObject->MoveDown(m_editSelection);
			std::swap(m_objectInfo->components[m_editSelection], m_objectInfo->components[m_editSelection + 1]);
			Editor::SetHasSaved(false);
			++m_editSelection;
		}
	}
	if (m_showAdd)
	{
		ImVec2 nextPos = (Vector2)ImGui::GetCursorPos() + ImGui::GetWindowPos();
		ImGui::SetNextWindowPos(nextPos);
		ImGui::Begin("Add Component##Insp", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		if (InputSystem::GetMouseDown(Mouse::Button::Left) && !ImGui::IsWindowHovered())
			m_showAdd = false;

		ImGui::InputText("##SearchAddComp", m_editSearchBuff, 256);
		for (auto& comp : m_compNames)
		{
			if (!std::regex_search(comp.second, std::regex(m_editSearchBuff)))
				continue;
			if (ImGui::MenuItem((comp.second + "##AddComp").c_str()))
			{
				SceneManager::Get().__AddComponentByID(m_currObject, comp.first);
				Editor::SetHasSaved(false);
				m_showAdd = false;
				break;
			}
		}
		ImGui::End();
	}

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();
	if (ImGui::Button("Show Components##Insp"))
	{
		m_showEdit = !m_showEdit;
	}
}


#endif //_EDITOR
