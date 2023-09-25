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
			}
			//Good Type
			Get().DataExport(field->second, ptr);
			field->second.ptr = ptr;
		}
		// No Field
		auto& inst = obj->second[name];
		inst.type = type;
		Get().DataImport(inst, ptr);
		inst.ptr = ptr;
	}
	// No Object
	auto& inst = s._manifest[instID][name];
	inst.type = type;
	Get().DataImport(inst, ptr);
	inst.ptr = ptr;
}

void Xeph2D::Serializer::DataImport(VarEntry& iter, void*& ptr)
{
	switch (iter.type)
	{
	case DataType::UInt8:
		iter.data = *(uint8_t*)ptr;
		break;
	case DataType::UInt16:
		iter.data = *(uint16_t*)ptr;
		break;
	case DataType::UInt32:
		iter.data = *(uint32_t*)ptr;
		break;
	case DataType::UInt64:
		iter.data = *(uint64_t*)ptr;
		break;
	case DataType::Int8:
		iter.data = *(int8_t*)ptr;
		break;
	case DataType::Int16:
		iter.data = *(int16_t*)ptr;
		break;
	case DataType::Int32:
		iter.data = *(int32_t*)ptr;
		break;
	case DataType::Int64:
		iter.data = *(int64_t*)ptr;
		break;
	case DataType::Float:
		iter.data = *(float*)ptr;
		break;
	case DataType::Double:
		iter.data = *(double*)ptr;
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
	case DataType::Pointer:
		iter.data = *(void**)ptr;
		break;
	default:
		Debug::LogErr("Serializer::DataImport -> Enum type not supported");
		break;
	}
}

void Xeph2D::Serializer::DataExport(VarEntry& iter, void*& ptr)
{
	switch (iter.type)
	{
	case DataType::UInt8:
		*(uint8_t*)ptr = iter.DataAs<uint8_t>();
		break;
	case DataType::UInt16:
		*(uint16_t*)ptr = iter.DataAs<uint16_t>();
		break;
	case DataType::UInt32:
		*(uint32_t*)ptr = iter.DataAs<uint32_t>();
		break;
	case DataType::UInt64:
		*(uint64_t*)ptr = iter.DataAs<uint64_t>();
		break;
	case DataType::Int8:
		*(int8_t*)ptr = iter.DataAs<int8_t>();
		break;
	case DataType::Int16:
		*(int16_t*)ptr = iter.DataAs<int16_t>();
		break;
	case DataType::Int32:
		*(int32_t*)ptr = iter.DataAs<int32_t>();
		break;
	case DataType::Int64:
		*(int64_t*)ptr = iter.DataAs<int64_t>();
		break;
	case DataType::Float:
		*(float*)ptr = iter.DataAs<float>();
		break;
	case DataType::Double:
		*(double*)ptr = iter.DataAs<double>();
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
		*(Transform*)ptr = iter.DataAs<Transform>();
		break;
	case DataType::Pointer:
		*(void**)ptr = iter.DataAs<void*>();
		break;
	default:
		Debug::LogErr("Serializer::DataExport -> Enum type not supported");
		break;
	}
}

void Xeph2D::Serializer::_SaveToFile()
{
	if (!std::filesystem::exists("Assets/Scenes"))
		std::filesystem::create_directories("Assets/Scenes");
	std::ofstream file("Assets/Scenes/scene.txt");
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

std::string Xeph2D::Serializer::DataStr(VarEntry& var) const
{
	Vector2 v2{};
	Color c{};
	Transform t{};
	switch (var.type)
	{
	case DataType::UInt8:
		return std::to_string(var.DataAs<uint8_t>());
	case DataType::UInt16:
		return std::to_string(var.DataAs<uint16_t>());
	case DataType::UInt32:
		return std::to_string(var.DataAs<uint32_t>());
	case DataType::UInt64:
		return std::to_string(var.DataAs<uint64_t>());
	case DataType::Int8:
		return std::to_string(var.DataAs<int8_t>());
	case DataType::Int16:
		return std::to_string(var.DataAs<int16_t>());
	case DataType::Int32:
		return std::to_string(var.DataAs<int32_t>());
	case DataType::Int64:
		return std::to_string(var.DataAs<int64_t>());
	case DataType::Float:
		return std::to_string(var.DataAs<float>());
	case DataType::Double:
		return std::to_string(var.DataAs<double>());
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
