#pragma once
#ifdef _EDITOR

#include "Editor/EditorWindow.h"

#include <map>

namespace Xeph2D::Edit
{
	class ScriptManager : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;

		void Save();
		void CreateNew(const std::string& name, std::string& path);

	private:
		void GenerateNewFiles(const std::string& name, const std::string& path, uint32_t newID);
		bool EditScript(uint32_t id);

		struct Entry
		{
			std::string name;
			std::string path;

			Entry() = default;
			Entry(const std::string& name, const std::string& path) : name(name), path(path) {}
		};

		std::string m_dataFilePath = "settings/";
		std::string m_dataFileName = "ScriptManager.x2dset";
		std::string m_scriptManifestPath = "Assets/Scripts/.generated/";
		std::string m_scriptManifestName = "ScriptManifest.generated.h";

		std::map<uint32_t, Entry> m_manifest;

		int m_editSelection = -1;

		bool m_isRemoving = false;
		bool m_isEditing = false;
		char m_nameBuffer[256];
		char m_pathBuffer[1024];
	};
}

#endif //_EDITOR
