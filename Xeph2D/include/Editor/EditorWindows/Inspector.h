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
	private:
		GameObject* _currObject = nullptr;
		Serializer::EdObject* _objectInfo = nullptr;
		//std::vector<VarEntry> _varList;

	public:
		void Initialize() override;
		void OnGUI() override;

		void SetGameObject(GameObject* obj);

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
	};
}