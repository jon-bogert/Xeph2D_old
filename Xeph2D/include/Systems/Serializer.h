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
        enum class DataType { Int, Float, Bool, Char, String, Vector2, Color, Transform };
        struct VarEntry
        {
            DataType type;
            std::any data;
            //void* ptr;

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

#ifdef _EDITOR

        struct EdVarEntry : public VarEntry
        {
            std::string name;
            void* ptr;
            template <typename T>
            void RefreshData()
            {
                data = *(T*)ptr;
            }
        };
        struct EdComponent
        {
            uint32_t id;
            std::vector<EdVarEntry> variables;
        };
        struct EdObject
        {
            std::vector<EdVarEntry> go_variables;
            std::vector<EdComponent> components;
        };
        using EdMap = std::unordered_map<uint32_t, EdObject>;
#endif //_EDITOR

    private:
        Serializer() {}
        static Serializer& Get();

        ObjMap _manifest;
#ifdef _EDITOR
        EdMap _editorManifest;
        void EditorAdd(uint32_t instID, const std::string& fieldName, const VarEntry& entry, void* ptr);
#endif //_EDITOR

    public:
        ~Serializer() = default;
        Serializer(const Serializer& other) = delete;
        Serializer(const Serializer&& other) = delete;
        Serializer operator=(const Serializer& other) = delete;
        Serializer operator=(const Serializer&& other) = delete;

        static void Register(const uint32_t instID, DataType type, void* ptr, const std::string& name);
        static void Register(const uint32_t instID, const uint32_t typeID, DataType type, void* ptr, const std::string& name);
        static void LoadFromFile(const std::string& scene) { Get()._LoadFromFile(scene); };
#ifdef _EDITOR
        static void SaveToFile(const std::string& scene) { Get()._SaveToFile(scene); };
        static EdObject* GetDataFromInstance(uint32_t instID);
#endif //_EDITOR

    private:
        void DataImport(VarEntry& iter, void*& ptr);
        void DataExport(VarEntry& iter, void*& ptr);
        std::string DataStr(VarEntry& var) const;
        void DataParse(VarEntry& entry, std::string& data);
#ifdef _EDITOR
        void _SaveToFile(const std::string& scene);
#endif //_EDITOR
        void _LoadFromFile(const std::string& scene);
    };

}