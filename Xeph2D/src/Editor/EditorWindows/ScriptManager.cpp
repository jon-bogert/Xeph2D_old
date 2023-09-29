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

#define TAB "    "

using namespace Xeph2D::Edit;

void ScriptManager::Initialize()
{
	name = "Script Manager";
	isOpen = false;

	std::ifstream file(_dataFilePath + _dataFileName);
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
		_manifest[id] = Entry(name, path);
	}

	file.close();
}

void ScriptManager::OnGUI()
{
	//for (auto& script : _manifest)
	//{
	//	if (script.second.path == "<engine>")
	//		continue;
	//	std::stringstream idStr;
	//	idStr << std::setw(8) << std::setfill('0') << std::hex << script.first;
	//	ImGui::Text((idStr.str() + ": " + script.second.name + " " + script.second.path).c_str());
	//}

	std::vector<uint32_t> ids;
	std::vector<std::string> names;

	for (auto& script : _manifest)
	{
		if (script.second.path == "<engine>")
			continue;
		ids.push_back(script.first);
		names.push_back(script.second.name);
	}
	if (ImGui::ListBox("##HItems", &_editSelection, Utility::CStrVect(names).data(), names.size()))
	{
		_isRemoving = false;
	}

	ImGui::NewLine();
	if (ImGui::Button("Add##Script"))
	{
		Editor::GetScriptCreator()->Open();
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove##Script"))
	{
		if (_editSelection >= 0)
		{
			_isRemoving = true;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Edit##Script"))
	{

	}

	if (_isRemoving)
	{
		ImGui::SetNextWindowPos({ (ImGui::GetMainViewport()->Size.x - ImGui::GetWindowWidth()) * 0.5f, (ImGui::GetMainViewport()->Size.y - ImGui::GetWindowHeight()) * 0.5f });
		ImGui::Begin("Confirm Remove##Script", &_isRemoving,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDecoration);

		ImGui::Text(("Are you sure you want to remove " + names[_editSelection] + "?").c_str());
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, "Notes: ");
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, " - Your current scene will also be saved.");
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, " - Remember to remove the script file references\nfrom your Visual Studio Project.");
		if (ImGui::Button("Confirm##RemoveScript"))
		{
			
			std::filesystem::remove(_manifest[ids[_editSelection]].path + names[_editSelection] + ".h");
			std::filesystem::remove(_manifest[ids[_editSelection]].path + names[_editSelection] + ".cpp");
			_manifest.erase(ids[_editSelection]);
			Serializer::RemoveAllComponents(ids[_editSelection]);
			Serializer::SaveToFile(SceneManager::GetCurrentName());
			Save();
			_editSelection = -1;
			_isRemoving = false;
			Editor::Close();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			_isRemoving = false;
		}
		ImGui::End();
	}
}

void ScriptManager::Save()
{
	if (!std::filesystem::exists(_dataFilePath))
		std::filesystem::create_directories(_dataFilePath);

	std::ofstream file(_dataFilePath + _dataFileName);
	for (auto& script : _manifest)
	{
		file << std::setw(8) << std::setfill('0') << std::hex << script.first << ' '
			<< script.second.name << ' ' << script.second.path << std::endl;
	}
	file.close();
	if (!std::filesystem::exists(_scriptManifestPath))
		std::filesystem::create_directories(_scriptManifestPath);

	file.open(_scriptManifestPath + _scriptManifestName);

	file << "#pragma once\n\n";
	file << "//ScriptManifest.h is Auto-Generated and managed by the Xeph2D Editor.\n//Use the Editor to add or remove scripts\n\n";
	file << "#include <Xeph2D.h>\n\n";
	file << "#include <memory>\n#include <unordered_map>\n#include <cstdint>\n\n";
	for (auto& script : _manifest)
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
	for (auto& script : _manifest)
	{
		file << TAB << TAB << "{0x" << std::setw(8) << std::setfill('0') << std::hex << script.first << ",\"" << script.second.name << "\"}";
		if (++index == _manifest.size())
			file << "};\n    };\n\n";
		else
			file << ",\n";
	}
	file << TAB << "void __PopulateComponentPtr(std::unique_ptr<Component>& ptr, uint32_t compID)\n    {\n";
	file << TAB << TAB << "switch (compID)\n        {\n";
	for (auto& script : _manifest)
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

	_manifest[newID] = { name, path };

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

#endif //_EDITOR
