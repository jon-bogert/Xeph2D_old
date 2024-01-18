#pragma once

#include "GameObject.h"
#include <box2d/box2d.h>

namespace Xeph2D
{
	class Rigidbody;
	class BoxCollider : public Component
	{
	public:
		COMP_HEADER_STD(0x00000005);

		void Awake() override;
		void DebugDraw() override;

	private:
		b2PolygonShape m_shape;

		Vector2 m_dimensions = { 1.f, 1.f };
		Rigidbody* m_rigidbody = nullptr;
		bool m_isTrigger;

		bool m_showCollider = true;
	};
}