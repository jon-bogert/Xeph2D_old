#pragma once
#include <Editor/EditorWindow.h>
#include <Xeph2D.h>

namespace Xeph2D::Edit
{
	class Viewport : public EditorWindow
	{
		Xeph2D::Vector2 _size;
		bool _isHovered = false;
		Vector2 _mouseInViewport{};

	public:
		void Initialize() override;
		void OnGUI() override;

		Vector2 GetMousePos() const;
	};
}