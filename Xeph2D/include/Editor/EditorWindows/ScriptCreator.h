#pragma once
#ifdef _EDITOR

#include "Editor/EditorWindow.h"

namespace Xeph2D::Edit
{
	class ScriptCreator : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;

	private:
		char m_nameBuffer[256];
		char m_pathBuffer[1024];

		int m_defaultFlags = 0;
		bool m_isConfirmed = false;
	};
}

#endif //_EDITOR