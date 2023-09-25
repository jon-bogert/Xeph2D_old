#pragma once

#define SERIALIZE_UINT8(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::UInt8, &var, #var)
#define SERIALIZE_UINT16(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::UInt16, &var, #var)
#define SERIALIZE_UINT32(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::UInt32, &var, #var)
#define SERIALIZE_UINT64(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::UInt64, &var, #var)
#define SERIALIZE_INT8(var)		Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Int8, &var, #var)
#define SERIALIZE_INT16(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Int16, &var, #var)
#define SERIALIZE_INT32(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Int32, &var, #var)
#define SERIALIZE_INT64(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Int64, &var, #var)
#define SERIALIZE_FLOAT(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Float, &var, #var)
#define SERIALIZE_DOUBLE(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Double, &var, #var)
#define SERIALIZE_BOOL(var)		Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Bool, &var, #var)
#define SERIALIZE_CHAR(var)		Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Char, &var, #var)
#define SERIALIZE_STRING(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::String, &var, #var)
#define SERIALIZE_VEC2(var)		Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Vector2, &var, #var)
#define SERIALIZE_COLOR(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Color, &var, #var)
#define SERIALIZE_TRANSF(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Transform, var, #var)
#define SERIALIZE_PTR(var)		Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Pointer, &var, #var)

#define SERIALIZE_INT(var) SERIALIZE_INT32(var)

#define SERIALIZE_DEFAULT