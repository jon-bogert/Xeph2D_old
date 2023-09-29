#pragma once
#ifdef _EDITOR

#include "Editor/EditorWindow.h"

#include <map>

namespace Xeph2D::Edit
{
	class ScriptManager : public EditorWindow
	{
		struct Entry
		{
			std::string name;
			std::string path;

			Entry() = default;
			Entry(const std::string& name, const std::string& path) : name(name), path(path) {}
		};

		std::string _dataFilePath = "settings/";
		std::string _dataFileName = "ScriptManager.x2dset";
		std::string _scriptManifestPath = "Assets/Scripts/.generated/";
		std::string _scriptManifestName = "ScriptManifest.generated.h";

		std::map<uint32_t, Entry> _manifest;

		int _editSelection = -1;

		bool _isRemoving = false;
		bool _isEditing = false;
		char _nameBuffer[256];
		char _pathBuffer[1024];

	public:
		void Initialize() override;
		void OnGUI() override;

		void Save();
		void CreateNew(const std::string& name, std::string& path);

	private:
		void GenerateNewFiles(const std::string& name, const std::string& path, uint32_t newID);
		bool EditScript(uint32_t id);
	};
}

#endif //_EDITOR
