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

	public:
		void Initialize() override;
		void OnGUI() override;

		void Save();
	};
}

#endif //_EDITOR
