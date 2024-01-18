#pragma once

#include "GameObject.h"
#include <box2d/box2d.h>

namespace Xeph2D
{
	class Rigidbody;
	class CircleCollider : public Component
	{
	public:
		COMP_HEADER_STD(0x00000006);

		void Awake() override;
		void DebugDraw() override;

	private:
		b2CircleShape m_shape;

		float m_radius = 0.5f;
		Rigidbody* m_rigidbody = nullptr;
		bool m_isTrigger;

		bool m_showCollider = true;
	};
}