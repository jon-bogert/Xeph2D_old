#pragma once
#ifdef _EDITOR
#include <Editor/EditorWindow.h>

namespace Xeph2D::Edit
{
	class Hierarchy : public EditorWindow
	{
		int _selectionIndex = -1;

	public:
		void Initialize() override;
		void OnGUI() override;
	};
}
#endif //_EDITOR