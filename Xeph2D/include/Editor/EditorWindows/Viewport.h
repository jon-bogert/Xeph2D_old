#pragma once
#include <Editor/EditorWindow.h>

namespace Xeph2D::Edit
{
	class Viewport : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;
	};
}