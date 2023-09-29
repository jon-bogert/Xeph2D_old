#pragma once

#include "GameObject.h"
#include <box2d/box2d.h>

namespace Xeph2D
{
	class Rigidbody;
	class CircleCollider : public Component
	{
		b2CircleShape _shape;

		float _radius = 0.5f;
		Rigidbody* _rigidbody = nullptr;
		bool _isTrigger;

		bool _showCollider = true;

	public:
		COMP_HEADER_STD(0x00000006);

		void Awake() override;
		void DebugDraw() override;
	};
}