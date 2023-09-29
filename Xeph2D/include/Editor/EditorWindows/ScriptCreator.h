#pragma once
#ifdef _EDITOR

#include "Editor/EditorWindow.h"

namespace Xeph2D::Edit
{
	class ScriptCreator : public EditorWindow
	{
		char _nameBuffer[256];
		char _pathBuffer[1024];

		int _defaultFlags = 0;
		bool _isConfirmed = false;

	public:
		void Initialize() override;
		void OnGUI() override;
	};
}

#endif //_EDITOR