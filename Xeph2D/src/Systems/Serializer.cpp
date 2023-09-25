#include "Systems/Serializer.h"

#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>

using namespace Xeph2D;

Serializer& Serializer::Get()
{
	static Serializer instance;
	return instance;
}

void Xeph2D::Serializer::Register(const uint32_t instID, const uint32_t typeID, DataType type, void* ptr, const std::string& name)
{
	std::stringstream nameFull;
	nameFull << std::setw(8) << std::setfill('0') << std::hex << typeID << name;
	Register(instID, type, ptr, nameFull.str());
}

void Xeph2D::Serializer::Register(const uint32_t instID, DataType type, void* ptr, const std::string& name)
{
	Serializer& s = Get();
	auto obj = s._manifest.find(instID);

	//Already object
	if (obj != s._manifest.end())
	{
		auto field = obj->second.find(name);
		if (field != obj->second.end()) // Field was found
		{
			if (field->second.type != type) // Bad Type
			{
				Debug::LogWarn("Serializer::Register -> Missmatched type on Register, Changing to new type");
				field->second.type = type;
				Get().DataImport(field->second, ptr);
				field->second.ptr = ptr;
				return;
			}
			//Good Type
			Get().DataExport(field->second, ptr);
			field->second.ptr = ptr;
			return;
		}
		// No Field
		auto& inst = obj->second[name];
		inst.type = type;
		Get().DataImport(inst, ptr);
		inst.ptr = ptr;
		return;
	}
	// No Object
	auto& inst = s._manifest[instID][name];
	inst.type = type;
	Get().DataImport(inst, ptr);
	inst.ptr = ptr;
}

Xeph2D::Serializer::VarMap* Xeph2D::Serializer::GetDataFromInstance(uint32_t instID)
{
	auto it = Get()._manifest.find(instID);
	if (it == Get()._manifest.end())
	{
		std::stringstream idStr;
		idStr << std::hex << std::setfill('0') << instID;
		Debug::LogErr("Serializer::GetDataFromInstance -> Could not find object with instance id: %s", idStr.str().c_str());
		return nullptr;
	}

	return &it->second;
}

void Xeph2D::Serializer::DataImport(VarEntry& iter, void*& ptr)
{
	switch (iter.type)
	{
	case DataType::Int:
		iter.data = *(int*)ptr;
		break;
	case DataType::Float:
		iter.data = *(float*)ptr;
		break;
	case DataType::Bool:
		iter.data = *(bool*)ptr;
		break;
	case DataType::Char:
		iter.data = *(char*)ptr;
		break;
	case DataType::String:
		iter.data = *(std::string*)ptr;
		break;
	case DataType::Vector2:
		iter.data = *(Vector2*)ptr;
		break;
	case DataType::Color:
		iter.data = *(Color*)ptr;
		break;
	case DataType::Transform:
		iter.data = *(Transform*)ptr;
		break;
	default:
		Debug::LogErr("Serializer::DataImport -> Enum type not supported");
		break;
	}
}

void Xeph2D::Serializer::DataExport(VarEntry& iter, void*& ptr)
{
	Transform t;
	switch (iter.type)
	{
	case DataType::Int:
		*(int*)ptr = iter.DataAs<int>();
		break;
	case DataType::Float:
		*(float*)ptr = iter.DataAs<float>();
		break;
	case DataType::Bool:
		*(bool*)ptr = iter.DataAs<bool>();
		break;
	case DataType::Char:
		*(char*)ptr = iter.DataAs<char>();
		break;
	case DataType::String:
		*(std::string*)ptr = iter.DataAs<std::string>();
		break;
	case DataType::Vector2:
		*(Vector2*)ptr = iter.DataAs<Vector2>();
		break;
	case DataType::Color:
		*(Color*)ptr = iter.DataAs<Color>();
		break;
	case DataType::Transform:
		t = iter.DataAs<Transform>();
		*(Transform*)ptr = iter.DataAs<Transform>();
		break;
	default:
		Debug::LogErr("Serializer::DataExport -> Enum type not supported");
		break;
	}
}

std::string Xeph2D::Serializer::DataStr(VarEntry& var) const
{
	Vector2 v2{};
	Color c{};
	Transform t{};
	switch (var.type)
	{
	case DataType::Int:
		return std::to_string(var.DataAs<int>());
	case DataType::Float:
		return std::to_string(var.DataAs<float>());
	case DataType::Bool:
		return (var.DataAs<bool>()) ? "true" : "false";
	case DataType::Char:
		return std::to_string(var.DataAs<char>());
	case DataType::String:
		return var.DataAs<std::string>();
	case DataType::Vector2:
		v2 = var.DataAs<Vector2>();
		return std::to_string(v2.x) + "," + std::to_string(v2.y);
	case DataType::Color:
		c = var.DataAs<Color>();
		return std::to_string(c.r) + "," + std::to_string(c.g) + "," + std::to_string(c.b) + "," + std::to_string(c.a);
	case DataType::Transform:
		t = var.DataAs<Transform>();
		return std::to_string(t.position.x) + "," + std::to_string(t.position.y) + "," +
			std::to_string(t.rotation.GetDeg()) + "," +
			std::to_string(t.scale.x) + "," + std::to_string(t.scale.y);
	default:
		Debug::LogErr("Serializer::DataStr -> Enum type not supported");
		return "";
	}
}

void Xeph2D::Serializer::DataParse(VarEntry& entry, std::string& data)
{
	Vector2 v2{};
	Color c{};
	Transform t{};
	std::stringstream datastream(data);
	std::string cell;
	switch (entry.type)
	{
	case DataType::Int:
		entry.data = (int)std::stoi(data);
		break;
	case DataType::Float:
		entry.data = (float)std::stof(data);
		break;
	case DataType::Bool:
		entry.data = (data == "true");
		break;
	case DataType::Char:
		entry.data = data[0];
		break;
	case DataType::String:
		entry.data = data;
		break;
	case DataType::Vector2:
		std::getline(datastream, cell, ',');
		v2.x = std::stof(cell);
		std::getline(datastream, cell, ',');
		v2.y = std::stof(cell);
		entry.data = v2;
		break;
	case DataType::Color:
		std::getline(datastream, cell, ',');
		c.r = std::stof(cell);
		std::getline(datastream, cell, ',');
		c.g = std::stof(cell);
		std::getline(datastream, cell, ',');
		c.b = std::stof(cell);
		std::getline(datastream, cell, ',');
		c.a = std::stof(cell);
		entry.data = c;
		break;
	case DataType::Transform:
		std::getline(datastream, cell, ',');
		t.position.x = std::stof(cell);
		std::getline(datastream, cell, ',');
		t.position.y = std::stof(cell);
		std::getline(datastream, cell, ',');
		t.rotation.SetDeg(std::stof(cell));
		std::getline(datastream, cell, ',');
		t.scale.x = std::stof(cell);
		std::getline(datastream, cell, ',');
		t.scale.y = std::stof(cell);
		entry.data = t;
		break;
	default:
		Debug::LogErr("Serializer::DataStr -> Enum type not supported");
		break;
	}
}

void Xeph2D::Serializer::_SaveToFile(const std::string& scene)
{
	if (!std::filesystem::exists("Assets/Scenes"))
		std::filesystem::create_directories("Assets/Scenes");
	std::ofstream file("Assets/Scenes/" + scene + ".x2dsc");
	for (auto& obj : _manifest)
	{
		file << "inst=" << std::setw(8) << std::setfill('0') << std::hex << obj.first << std::dec << std::endl;
		for (auto& field : obj.second)
		{
			file << "    " << (int)field.second.type << ' ' << field.first << '=' << DataStr(field.second) << std::endl;
		}
	}
	file.close();
}

void Xeph2D::Serializer::_LoadFromFile(const std::string& scene)
{
	std::ifstream file("Assets/Scenes/" + scene + ".x2dsc");
	if (!file.is_open())
	{
		Debug::LogWarn("Serializer::LoadFromFile -> No file to load: %s", (scene + ".x2dsc").c_str());
		return;
	}

	std::string line;
	uint32_t inst = 0;
	while (std::getline(file, line))
	{
		if (line.substr(0, 5) == "inst=")
		{
			std::stringstream id;
			id << std::hex << line.substr(5);
			id >> inst;
			continue;
		}
		if (line.substr(0, 4) != "    ")
		{
			Debug::LogErr("Serializer::LoadFromFile -> Bad Formatting: %s", line.c_str());
			continue;
		}
		std::stringstream linestream(line.substr(4));
		std::string cell;
		VarEntry entry;
		std::string key;
		std::getline(linestream, cell, ' ');
		entry.type = static_cast<Serializer::DataType>(std::stoi(cell));
		std::getline(linestream, key, '=');
		std::getline(linestream, cell, '\n');
		DataParse(entry, cell);
		_manifest[inst][key] = entry;
	}
	file.close();
}
