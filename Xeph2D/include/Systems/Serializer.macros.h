#pragma once
#define SERIALIZE_INT(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Int, &var, #var)
#define SERIALIZE_FLOAT(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Float, &var, #var)
#define SERIALIZE_BOOL(var)		Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Bool, &var, #var)
#define SERIALIZE_CHAR(var)		Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Char, &var, #var)
#define SERIALIZE_STRING(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::String, &var, #var)
#define SERIALIZE_VEC2(var)		Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Vector2, &var, #var)
#define SERIALIZE_COLOR(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Color, &var, #var)
#define SERIALIZE_TRANSF(var)	Serializer::Register(gameObject->instID, typeID, Serializer::DataType::Transform, var, #var)

#define SERIALIZE_DEFAULT