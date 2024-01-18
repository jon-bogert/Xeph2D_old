#ifdef _EDITOR
#include "Editor/EditorWindows/ScriptManager.h"
#include "Editor/Editor.h"

#include "Structs.h"
#include "Utility.h"

#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <iomanip>
#include <filesystem>

#pragma warning (disable : 4996)

#define TAB "    "

using namespace Xeph2D::Edit;

void ScriptManager::Initialize()
{
	name = "Script Manager";
	isOpen = false;

	std::ifstream file(m_dataFilePath + m_dataFileName);
	if (!file.is_open())
		return;

	std::string line;
	uint32_t id;
	std::string name;
	std::string path;
	while (std::getline(file, line))
	{
		std::stringstream linestream(line);
		std::string cell;
		std::getline(linestream, cell, ' ');
		std::stringstream idstream;
		idstream << std::hex << cell;
		idstream >> id;
		std::getline(linestream, name, ' ');
		std::getline(linestream, path, '\n');
		m_manifest[id] = Entry(name, path);
	}

	file.close();
}

void ScriptManager::OnGUI()
{
	std::vector<uint32_t> ids;
	std::vector<std::string> names;

	for (auto& script : m_manifest)
	{
		if (script.second.path == "<engine>")
			continue;
		ids.push_back(script.first);
		names.push_back(script.second.name);
	}
	if (ImGui::ListBox("##HItems", &m_editSelection, Utility::CStrVect(names).data(), names.size()))
	{
		m_isRemoving = false;
	}

	ImGui::NewLine();
	if (ImGui::Button("Add##Script"))
	{
		Editor::GetScriptCreator()->Open();
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove##Script"))
	{
		if (m_editSelection >= 0)
		{
			m_isRemoving = true;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Edit##Script"))
	{
		if (m_editSelection >= 0)
		{
			m_isEditing = true;
		}
	}

	if (m_isRemoving)
	{
		ImGui::SetNextWindowPos({ (ImGui::GetMainViewport()->Size.x - ImGui::GetWindowWidth()) * 0.5f, (ImGui::GetMainViewport()->Size.y - ImGui::GetWindowHeight()) * 0.5f });
		ImGui::Begin("Confirm Remove##Script", &m_isRemoving,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDecoration);

		ImGui::Text(("Are you sure you want to remove " + names[m_editSelection] + "?").c_str());
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, "Notes: ");
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, " - Your current scene will also be saved.");
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, " - Remember to remove the script file references\nfrom your Visual Studio Project.");
		if (ImGui::Button("Confirm##RemoveScript"))
		{
			
			std::filesystem::remove(m_manifest[ids[m_editSelection]].path + names[m_editSelection] + ".h");
			std::filesystem::remove(m_manifest[ids[m_editSelection]].path + names[m_editSelection] + ".cpp");
			m_manifest.erase(ids[m_editSelection]);
			Serializer::RemoveAllComponents(ids[m_editSelection]);
			Serializer::SaveToFile(SceneManager::GetCurrentName());
			Save();
			m_editSelection = -1;
			m_isRemoving = false;
			Editor::RebuildProject();
			Editor::Close();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			m_isRemoving = false;
		}
		ImGui::End();
	}

	if (m_isEditing)
	{
		ImGui::SetNextWindowPos({ (ImGui::GetMainViewport()->Size.x - ImGui::GetWindowWidth()) * 0.5f, (ImGui::GetMainViewport()->Size.y - ImGui::GetWindowHeight()) * 0.5f });
		ImGui::Begin("Edit##Script", &m_isRemoving,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDecoration);

		if (InputSystem::GetMouseDown(Mouse::Button::Left) && !ImGui::IsWindowHovered())
		{
			m_isEditing = false;
			strcpy(m_nameBuffer, "");
			strcpy(m_pathBuffer, "");
		}

		ImGui::Text(("Editing: " + names[m_editSelection]).c_str());
		std::stringstream id;
		id << "Component ID: " << std::setw(8) << std::setfill('0') << std::hex << ids[m_editSelection];
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, id.str().c_str());
		ImGui::NewLine();
		ImGui::Text("Name:");
		ImGui::InputText("##Name", m_nameBuffer, 256);
		ImGui::NewLine();
		ImGui::Text("Path:");
		ImGui::SameLine();
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.f }, "(optional)");
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.f }, "Assets/Scripts/");
		ImGui::SameLine();
		ImGui::InputText("##Path", m_pathBuffer, 1024);
		ImGui::NewLine();
		if (ImGui::Button("Save##Script"))
		{
			bool wasChanged = EditScript(ids[m_editSelection]);
			if (wasChanged)
			{
				Save();
				m_isEditing = false;
				strcpy(m_nameBuffer, "");
				strcpy(m_pathBuffer, "");
				m_editSelection = -1;
				Editor::RebuildProject();
				Editor::Close();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel##Script"))
		{
			m_isEditing = false;
			strcpy(m_nameBuffer, "");
			strcpy(m_pathBuffer, "");
		}
		ImGui::End();
	}
}

void ScriptManager::Save()
{
	if (!std::filesystem::exists(m_dataFilePath))
		std::filesystem::create_directories(m_dataFilePath);

	std::ofstream file(m_dataFilePath + m_dataFileName);
	for (auto& script : m_manifest)
	{
		file << std::setw(8) << std::setfill('0') << std::hex << script.first << ' '
			<< script.second.name << ' ' << script.second.path << std::endl;
	}
	file.close();
	if (!std::filesystem::exists(m_scriptManifestPath))
		std::filesystem::create_directories(m_scriptManifestPath);

	file.open(m_scriptManifestPath + m_scriptManifestName);

	file << "#pragma once\n\n";
	file << "//ScriptManifest.h is Auto-Generated and managed by the Xeph2D Editor.\n//Use the Editor to add or remove scripts\n\n";
	file << "#include <Xeph2D.h>\n\n";
	file << "#include <memory>\n#include <unordered_map>\n#include <cstdint>\n\n";
	for (auto& script : m_manifest)
	{
		if (script.second.path == "<engine>")
			continue;

		file << "#include \"" << script.second.path.substr(15) << script.second.name << ".h\"\n"; //substr to remove "Assets/Scripts/
	}

	file << "\n#define __X2D_REGISTER_COMP_NAMES &__RegisterComponentNames\n";
	file << "#define __X2D_POPULATE_COMP_PTR &__PopulateComponentPtr\n\n";
	file << "namespace Xeph2D\n{\n";
	file << TAB << "std::unordered_map<uint32_t, std::string> __RegisterComponentNames()\n    {\n";
	file << TAB << TAB << "return{\n";
	size_t index = 0;
	for (auto& script : m_manifest)
	{
		file << TAB << TAB << "{0x" << std::setw(8) << std::setfill('0') << std::hex << script.first << ",\"" << script.second.name << "\"}";
		if (++index == m_manifest.size())
			file << "};\n    };\n\n";
		else
			file << ",\n";
	}
	file << TAB << "void __PopulateComponentPtr(std::unique_ptr<Component>& ptr, uint32_t compID)\n    {\n";
	file << TAB << TAB << "switch (compID)\n        {\n";
	for (auto& script : m_manifest)
	{
		file << TAB << TAB << "case 0x" << std::setw(8) << std::setfill('0') << std::hex << script.first << ": ";
		file << "ptr = std::make_unique<" << script.second.name << ">(); break;\n";
	}
	file << TAB << TAB << "}\n" << TAB << "}\n}\n";

	file.close();
}

void Xeph2D::Edit::ScriptManager::CreateNew(const std::string& name, std::string& path)
{
	if (path != "")
	{
		if (path.back() != '/' && path.back() != '\\')
			path.push_back('/');

		for (size_t i = 0; i < path.length(); ++i)
		{
			if (path[i] == '\\')
				path[i] = '/';
		}
	}

	Inspector* insp = Editor::GetInspectorWindow();
	uint32_t newID;
	bool isUsed = true;
	while (isUsed)
	{
		newID = Math::Random::UInt32();
		isUsed = insp->CompNamesContains(newID);
	}

	m_manifest[newID] = { name, path };

	GenerateNewFiles(name, path, newID);
	Save();
}

void ScriptManager::GenerateNewFiles(const std::string& name, const std::string& path, uint32_t newID)
{
	if (!std::filesystem::exists(path))
		std::filesystem::create_directories(path);

	std::stringstream id;
	id << std::setw(8) << std::setfill('0') << std::hex << newID;

	std::ifstream fileIn("template/comp_h.template");
	std::ofstream fileOut(path + name + ".h");
	std::string line;
	std::regex pattern;
	while (std::getline(fileIn, line))
	{
		pattern = "@NAME@";
		line = std::regex_replace(line, pattern, name);
		pattern = "@ID@";
		line = std::regex_replace(line, pattern, "0x" + id.str());

		fileOut << line << std::endl;
	}

	fileIn.close();
	fileOut.close();
	fileIn.open("template/comp_cpp.template");
	fileOut.open(path + name + ".cpp");
	while (std::getline(fileIn, line))
	{
		pattern = "@NAME@";
		line = std::regex_replace(line, pattern, name);
		pattern = "@ID@";
		line = std::regex_replace(line, pattern, "0x" + id.str());

		fileOut << line << std::endl;
	}

	fileIn.close();
	fileOut.close();
}

bool Xeph2D::Edit::ScriptManager::EditScript(uint32_t id)
{
	std::string path("Assets/Scripts/");
	path.append(m_pathBuffer);
	std::string newName(m_nameBuffer);

	if (path != "")
	{
		if (path.back() != '/' && path.back() != '\\')
			path.push_back('/');

		for (size_t i = 0; i < path.length(); ++i)
		{
			if (path[i] == '\\')
				path[i] = '/';
		}
	}

	bool doChangePath = (path != m_manifest[id].path);
	bool doChangeName = (newName != m_manifest[id].name);

	if (!doChangeName && !doChangeName)
		return false;

	std::ifstream headerIn(m_manifest[id].path + m_manifest[id].name + ".h");
	std::ifstream cppIn(m_manifest[id].path + m_manifest[id].name + ".cpp");
	std::stringstream headerTemp;
	std::stringstream cppTemp;
	headerTemp << headerIn.rdbuf();
	cppTemp << cppIn.rdbuf();
	headerIn.close();
	cppIn.close();
	std::filesystem::remove(m_manifest[id].path + m_manifest[id].name + ".h");
	std::filesystem::remove(m_manifest[id].path + m_manifest[id].name + ".cpp");

	if (!std::filesystem::exists(path))
		std::filesystem::create_directories(path);
	std::ofstream out(path + newName + ".h");
	std::string line;
	std::regex pattern(m_manifest[id].name);
	while (std::getline(headerTemp, line))
	{
		line = std::regex_replace(line, pattern, newName);
		out << line << std::endl;
	}
	out.close();
	out.open(path + newName + ".cpp");
	while (std::getline(cppTemp, line))
	{
		if (line == "#include \"" + m_manifest[id].path.substr(15) + m_manifest[id].name + ".h\"")
		{
			out << "#include \"" + path.substr(15) + newName + ".h\"" << std::endl;
			continue;
		}
		line = std::regex_replace(line, pattern, newName);
		out << line << std::endl;
	}
	out.close();

	m_manifest[id].path = path;
	m_manifest[id].name = newName;

	return true;
}

#endif //_EDITOR
