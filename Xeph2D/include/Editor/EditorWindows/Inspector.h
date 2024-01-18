#pragma once
#include <Editor/EditorWindow.h>
#include <Systems/Serializer.h>

namespace Xeph2D
{
	class GameObject;
}

namespace Xeph2D::Edit
{
	class Inspector : public EditorWindow
	{
	public:
		struct VarEntry
		{
			std::string displayName;
			Serializer::EdVarEntry* serialized = nullptr;
			void* instanced = nullptr;
		};

		void Initialize() override;
		void OnGUI() override;

		void SetGameObject(GameObject* obj);
		void RegisterComponentNames(std::function<std::unordered_map<uint32_t, std::string>(void)> callback);
		bool CompNamesContains(uint32_t id);

	private:
		std::string Var2DisplayName(std::string varName);

		void DrawVar(VarEntry& entry);
		void DrawInt(VarEntry& entry);
		void DrawFloat(VarEntry& entry);
		void DrawBool(VarEntry& entry);
		void DrawChar(VarEntry& entry);
		void DrawString(VarEntry& entry);
		void DrawVec2(VarEntry& entry);
		void DrawColor(VarEntry& entry);
		void DrawTransform(VarEntry& entry);

		void ShowEdit();

		GameObject* m_currObject = nullptr;
		Serializer::EdObject* m_objectInfo = nullptr;
		std::unordered_map<uint32_t, std::string> m_compNames;

		bool m_showEdit = false;
		bool m_showAdd = false;
		char m_editSearchBuff[256];
		int m_editSelection = -1;
	};
}