#pragma once
#include <Editor/EditorWindow.h>
#include <Xeph2D.h>

namespace Xeph2D::Edit
{
	class Viewport : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;

		Vector2 GetMousePos() const;

	private:
		Xeph2D::Vector2 m_size;
		bool m_isHovered = false;
		Vector2 m_mouseInViewport{};
	};
}