#pragma once

#include "GameObject.h"
#include <box2d/box2d.h>

namespace Xeph2D
{
	class Rigidbody;
	class BoxCollider : public Component
	{
		b2PolygonShape _shape;

		Vector2 _dimensions = { 1.f, 1.f };
		Rigidbody* _rigidbody = nullptr;
		bool _isTrigger;

		bool _showCollider = true;

	public:
		COMP_HEADER_STD(5);

		void Awake() override;
		void DebugDraw() override;
	};
}