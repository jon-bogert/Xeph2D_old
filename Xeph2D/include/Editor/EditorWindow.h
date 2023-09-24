#pragma once

#include "../../ImGui/include/imgui.h"

#include <string>

namespace Xeph2D::Edit
{
	class EditorWindow
	{
		friend class Editor;
		bool _isFocused = false;
		bool _isHovered = false;
		void UpdateValues();

	protected:
		std::string name = "<Unnamed>";
		int flags = 0;
		bool isOpen = true;

	public:
		EditorWindow() = default;
		virtual ~EditorWindow() = default;

		virtual void Initialize() {};
		virtual void OnGUI() {};

		const char* GetName() const;
		//ImGuiWindowFlags GetFlags() const;
		bool IsOpen() const;
		void Close();
		void Open();

		bool IsFocused() const;
		bool IsHovered() const;
	};
}