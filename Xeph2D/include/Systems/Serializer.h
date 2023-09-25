#pragma once
#include "Systems/Debug.h"

#include <string>
#include <unordered_map>
#include <any>

#include "Serializer.macros.h"

namespace Xeph2D
{
    class Serializer final
    {
    public:
        enum class DataType {
            UInt8, UInt16, UInt32, UInt64, Int8, Int16, Int32, Int64,
            Float, Double, Bool, Char, String, Vector2, Color, Transform, Pointer
        };

    private:
        Serializer() {}
        static Serializer& Get();

        struct VarEntry
        {
            DataType type;
            std::any data;
            void* ptr;

            template <typename T>
            T DataAs()
            {
                try
                {
                    return std::any_cast<T>(data);
                }
                catch (const std::bad_any_cast& e)
                {
                    Debug::LogErr("Serializer Entry -> Bad type cast from %s to %s", data.type().name(), typeid(T).name());
                    return T();
                }
            }
        };

        using VarMap = std::unordered_map<std::string, VarEntry>;
        using ObjMap = std::unordered_map<uint32_t, VarMap>;

        ObjMap _manifest;

    public:
        ~Serializer() = default;
        Serializer(const Serializer& other) = delete;
        Serializer(const Serializer&& other) = delete;
        Serializer operator=(const Serializer& other) = delete;
        Serializer operator=(const Serializer&& other) = delete;

        static void Register(const uint32_t instID, DataType type, void* ptr, const std::string& name);
        static void Register(const uint32_t instID, const uint32_t typeID, DataType type, void* ptr, const std::string& name);
        static void SaveToFile() { Get()._SaveToFile(); };

    private:
        void DataImport(VarEntry& iter, void*& ptr);
        void DataExport(VarEntry& iter, void*& ptr);

        void _SaveToFile();
        std::string DataStr(VarEntry& var) const;
    };

}