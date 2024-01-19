#include "Systems/Serializer.h"
#include "GameObject.h"
#include "Utility.h"

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


void Xeph2D::Serializer::RegisterGameObject(const uint32_t instID, GameObject* object)
{
	Serializer& s = Get();
	auto obj = s.m_manifest.find(instID);

	//Object not found
	if (obj == s.m_manifest.end())
	{
		VarEntry tmpEntry;
		tmpEntry.type = DataType::String;
		tmpEntry.data = s.m_manifest[instID].name = object->name;
#ifdef _EDITOR
		Get().EditorAdd(instID, NULL, "name", tmpEntry, &object->name);
#endif // _EDITOR
		tmpEntry.type = DataType::Transform;
		tmpEntry.data = s.m_manifest[instID].transform = object->transform;
#ifdef _EDITOR
		Get().EditorAdd(instID, NULL, "transform", tmpEntry, &object->name);
#endif // _EDITOR
		return;
	}

	object->name = obj->second.name;
	object->transform = obj->second.transform;
}

void Xeph2D::Serializer::Register(const uint32_t instID, const uint32_t compID, DataType type, void* ptr, const std::string& name)
{
	Serializer& s = Get();
	auto obj = s.m_manifest.find(instID);

	//Object not found
	if (obj == s.m_manifest.end())
	{
		//Is Component Value
		VarEntry& inst = s.m_manifest[instID].components[compID][name];
		inst.type = type;
		Get().DataImport(inst, ptr);
#ifdef _EDITOR
		Get().EditorAdd(instID, compID, name, inst, ptr);
#endif //_EDITOR
		return;
	}

	//Create Component if not found
	if (obj->second.components.find(compID) == obj->second.components.end())
		obj->second.components[compID];

	auto field = obj->second.components[compID].find(name);
	if (field != obj->second.components[compID].end()) // Field was found
	{
		if (field->second.type != type) // Bad Type
		{
			Debug::LogWarn("Serializer::Register -> Missmatched type on Register, Changing to new type");
			field->second.type = type;
			Get().DataImport(field->second, ptr); // second is VarEntry, first is field name; obj.first is object id
#ifdef _EDITOR
			Get().EditorAdd(instID, compID, field->first, field->second, ptr);
#endif //_EDITOR
			return;
		}
		//Good Type
		Get().DataExport(field->second, ptr);
#ifdef _EDITOR
		Get().EditorAdd(instID, compID, field->first, field->second, ptr);
#endif //_EDITOR
		return;
	}
	// No Field
	VarEntry& inst = obj->second.components[compID][name];
	inst.type = type;
	Get().DataImport(inst, ptr);
#ifdef _EDITOR
	Get().EditorAdd(instID, compID, name, inst, ptr);
#endif //_EDITOR
	return;

}

#ifdef _EDITOR
void Xeph2D::Serializer::EditorAdd(uint32_t instID, uint32_t compID, const std::string& fieldName, const VarEntry& entry, void* ptr)
{
	if (compID == 0x00000000) // Game Object Variable
	{
		EdVarEntry& newEntry = m_editorManifest[instID].go_variables.emplace_back();
		newEntry.type = entry.type;
		newEntry.data = entry.data;
		newEntry.name = fieldName;
		newEntry.ptr = ptr;
		return;
	}
	//Component Variable

	EdComponent* comp = nullptr;
	for (EdComponent& x : m_editorManifest[instID].components)
	{
		if (x.id == compID)
		{
			comp = &x;
			break;
		}
	}
	if (comp == nullptr)
	{
		comp = &m_editorManifest[instID].components.emplace_back();
		comp->id = compID;
	}

	EdVarEntry& newEntry = comp->variables.emplace_back();
	newEntry.type = entry.type;
	newEntry.data = entry.data;
	newEntry.name = fieldName;
	newEntry.ptr = ptr;
}

void Xeph2D::Serializer::EditorAddGameObject(GameObject* obj)
{
	uint32_t newID;
	bool isUsed = true;
	while (isUsed)
	{
		newID = Math::Random::UInt32();
		isUsed = (m_editorManifest.find(newID) != m_editorManifest.end());
	}
	obj->instID = newID;
	m_editorManifest[newID];
	obj->Serializables();
}

void Xeph2D::Serializer::EditorRemoveGameObject(GameObject* obj)
{
	m_editorManifest.erase(obj->instID);
}

Xeph2D::Serializer::EdObject* Xeph2D::Serializer::GetDataFromInstance(uint32_t instID)
{
	auto it = Get().m_editorManifest.find(instID);
	if (it == Get().m_editorManifest.end())
	{
		std::stringstream idStr;
		idStr << std::hex << std::setfill('0') << instID;
		Debug::LogErr("Serializer::GetDataFromInstance -> Could not find object with instance id: %s", idStr.str().c_str());
		return nullptr;
	}

	return &it->second;
}
void Xeph2D::Serializer::RemoveAllComponents(uint32_t id)
{
	for (auto& obj : Get().m_editorManifest)
	{
		for (auto it = obj.second.components.begin(); it != obj.second.components.end();)
		{
			if (it->id == id)
			{
				it = obj.second.components.erase(it);
			}
			else
				it++;
		}
	}
}
#endif //_EDITOR

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

void Xeph2D::Serializer::YAMLSave(EdVarEntry& entry, YAML::Node& content)
{
	switch (entry.type)
	{
	case DataType::Int:
		content["type"] = "int";
		content["data"] = std::any_cast<int>(entry.data);
		break;
	case DataType::Float:
		content["type"] = "float";
		content["data"] = std::any_cast<float>(entry.data);
		break;
	case DataType::Bool:
		content["type"] = "bool";
		content["data"] = std::any_cast<bool>(entry.data);
		break;
	case DataType::Char:
		content["type"] = "char";
		content["data"] = std::any_cast<char>(entry.data);
		break;
	case DataType::String:
		content["type"] = "string";
		content["data"] = std::any_cast<std::string>(entry.data);
		break;
	case DataType::Vector2:
		content["type"] = "vector2";
		content["data"]["x"] = std::any_cast<Vector2>(entry.data).x;
		content["data"]["y"] = std::any_cast<Vector2>(entry.data).y;
		break;
	case DataType::Color:
		content["type"] = "color";
		content["data"]["r"] = std::any_cast<Color>(entry.data).r;
		content["data"]["g"] = std::any_cast<Color>(entry.data).g;
		content["data"]["b"] = std::any_cast<Color>(entry.data).b;
		content["data"]["a"] = std::any_cast<Color>(entry.data).a;
		break;
	case DataType::Transform:
		content["type"] = "transform";
		content["data"]["pos"]["x"] = std::any_cast<Transform>(entry.data).position.x;
		content["data"]["pos"]["y"] = std::any_cast<Transform>(entry.data).position.y;
		content["data"]["rot"] = std::any_cast<Transform>(entry.data).rotation.GetDeg();
		content["data"]["sca"]["x"] = std::any_cast<Transform>(entry.data).scale.x;
		content["data"]["sca"]["y"] = std::any_cast<Transform>(entry.data).scale.y;
		break;
	default:
		Debug::LogErr("Serializer::DataStr -> Enum type not supported");
		break;
	}
}

#ifdef _EDITOR
void Xeph2D::Serializer::_SaveToFile(const std::string& scene)
{
	YAML::Node sceneInfo;
	//Add scene information here

	for (auto& obj : m_editorManifest)
	{
		YAML::Node objInfo;
		objInfo["inst"] = Utility::ToHex32String(obj.first);
		for (auto& goField : obj.second.go_variables)
		{
			if (goField.type == DataType::String)
				objInfo[goField.name] = std::any_cast<std::string>(goField.data);
			else if (goField.type == DataType::Transform)
			{
				objInfo[goField.name]["pos"]["x"] = std::any_cast<Transform>(goField.data).position.x;
				objInfo[goField.name]["pos"]["y"] = std::any_cast<Transform>(goField.data).position.y;
				objInfo[goField.name]["rot"] = std::any_cast<Transform>(goField.data).rotation.GetDeg();
				objInfo[goField.name]["sca"]["x"] = std::any_cast<Transform>(goField.data).scale.x;
				objInfo[goField.name]["sca"]["y"] = std::any_cast<Transform>(goField.data).scale.y;
			}
		}
		for (auto& comp : obj.second.components)
		{
			YAML::Node compInfo;
			compInfo["id"] = Utility::ToHex32String(comp.id);
			for (auto& compField : comp.variables)
			{
				YAML::Node fieldInfo;
				fieldInfo["field"] = compField.name;
				YAMLSave(compField, fieldInfo);
				compInfo["variables"].push_back(fieldInfo);
			}
			objInfo["components"].push_back(compInfo);
		}
		sceneInfo["objects"].push_back(objInfo);
	}

	if (!std::filesystem::exists("Assets/Scenes"))
		std::filesystem::create_directories("Assets/Scenes");
	std::ofstream file("Assets/Scenes/" + scene + ".yaml");
	file << sceneInfo;
	file.close();
}
#endif //_EDITOR

void Xeph2D::Serializer::YAMLLoad(YAML::const_iterator& field, VarEntry& entry)
{
	std::string type = (*field)["type"].as<std::string>();
	if (type == "int")
	{
		entry.type = DataType::Int;
		entry.data = (*field)["data"].as<int>();
	}
	else if (type == "float")
	{
		entry.type = DataType::Float;
		entry.data = (*field)["data"].as<float>();
	}
	else if (type == "bool")
	{
		entry.type = DataType::Bool;
		entry.data = (*field)["data"].as<bool>();
	}
	else if (type == "char")
	{
		entry.type = DataType::Char;
		entry.data = (*field)["data"].as<char>();
	}
	else if (type == "string")
	{
		entry.type = DataType::String;
		entry.data = (*field)["data"].as<std::string>();
	}
	else if (type == "vector2")
	{
		entry.type = DataType::Vector2;
		Vector2 data;
		data.x = (*field)["data"]["x"].as<float>();
		data.y = (*field)["data"]["y"].as<float>();
		entry.data = data;
	}
	else if (type == "Color")
	{
		entry.type = DataType::Color;
		Color data;
		data.r = (*field)["data"]["r"].as<float>();
		data.g = (*field)["data"]["g"].as<float>();
		data.b = (*field)["data"]["b"].as<float>();
		data.a = (*field)["data"]["a"].as<float>();
		entry.data = data;
	}
	else if (type == "transform")
	{
		entry.type = DataType::Transform;
		Transform data;
		data.position.x = (*field)["data"]["pos"]["x"].as<float>();
		data.position.y = (*field)["data"]["pos"]["y"].as<float>();
		data.rotation.SetDeg((*field)["data"]["rot"].as<float>());
		data.scale.x = (*field)["data"]["sca"]["x"].as<float>();
		data.scale.y = (*field)["data"]["sca"]["y"].as<float>();
		entry.data = data;
	}
}

void Xeph2D::Serializer::_LoadFromFile(const std::string& scene)
{
	YAML::Node content;
	if (!std::filesystem::exists("Assets/Scenes/" + scene + ".yaml"))
	{
		Debug::LogErr(("No File located at " + std::string("Assets/Scenes/") + scene + ".x2dsc").c_str());
		return;
	}
	try
	{
		content = YAML::LoadFile("Assets/Scenes/" + scene + ".yaml");
	}
	catch (std::exception e)
	{
		Debug::LogErr("Error while reading scene %s -- %s", ("Assets/Scenes/" + scene + ".yaml").c_str(), e.what());
		return;
	}

	//Load Scene Info;
	for (YAML::const_iterator obj = content["objects"].begin(); obj != content["objects"].end(); ++obj)
	{
		ObjInfo objInfo;

		uint32_t inst = Utility::FromHex32String((*obj)["inst"].as<std::string>());
		objInfo.name = (*obj)["name"].as<std::string>();

		Transform transform;
		transform.position.x = (*obj)["transform"]["pos"]["x"].as<float>();
		transform.position.y = (*obj)["transform"]["pos"]["y"].as<float>();
		transform.rotation.SetDeg((*obj)["transform"]["rot"].as<float>());
		transform.scale.x = (*obj)["transform"]["sca"]["x"].as<float>();
		transform.scale.y = (*obj)["transform"]["sca"]["y"].as<float>();

		objInfo.transform = transform;

		for (YAML::const_iterator comp = (*obj)["components"].begin(); comp != (*obj)["components"].end(); ++comp)
		{
			uint32_t id = Utility::FromHex32String((*comp)["id"].as<std::string>());
			VarMap varMap;
			for (YAML::const_iterator field = (*comp)["variables"].begin(); field != (*comp)["variables"].end(); ++field)
			{
				VarEntry entry;
				std::string key = (*field)["field"].as<std::string>();
				YAMLLoad(field, entry);
				varMap[key] = entry;
			}
			objInfo.components[id] = varMap;
		}
		m_manifest[inst] = objInfo;
	}
}

std::vector<Xeph2D::Serializer::IDInfo> Xeph2D::Serializer::GetIDInfo()
{
	std::vector<IDInfo> result;
	for (const std::pair<uint32_t, ObjInfo>& obj : Get().m_manifest)
	{
		IDInfo& o = result.emplace_back();
		o.gameObject = obj.first;
		for (const std::pair<uint32_t, VarMap>& comp : obj.second.components)
		{
			o.components.push_back(comp.first);
		}
	}
	return result;
}
