#pragma once
#include <Editor/EditorWindow.h>

namespace Xeph2D::Edit
{
	class Inspector : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;
	};
}