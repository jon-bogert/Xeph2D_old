#pragma once
#ifdef _EDITOR
#include <Editor/EditorWindow.h>

namespace Xeph2D::Edit
{
	class Hierarchy : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;

	private:
		int m_selectionIndex = -1;
	};
}
#endif //_EDITOR