#pragma once

#include "GameObject.h"

namespace Xeph2D
{
	class Camera : public Component
	{
	public:
		COMP_HEADER_STD(0x00000001);

		void Awake() override;
		void OnDestroy() override;
	};
}