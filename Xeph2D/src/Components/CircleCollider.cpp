#include "Components/CircleCollider.h"

#include "Systems/Serializer.h"
#include "Systems/Physics.h"

#include "Components/Rigidbody.h"

#include <cmath>

using namespace Xeph2D;

void CircleCollider::Serializables()
{
	SERIALIZE_DEFAULT;
	SERIALIZE_FLOAT(_radius);
	SERIALIZE_BOOL(_isTrigger);
	SERIALIZE_BOOL(_showCollider);
}
void CircleCollider::Awake()
{
	_rigidbody = gameObject->GetComponent<Rigidbody>();
	if (!_rigidbody)
		Debug::LogErr("BoxCollider -> No Rigidbody found on GameObject %s", gameObject->name.c_str());

	_shape.m_radius = _radius;
	b2FixtureDef fixDef;
	fixDef.shape = &_shape;

	PhysicsMaterial material{}; // TODO - Assign in inspector

	fixDef.friction = material.friction;
	fixDef.density = material.density;
	fixDef.restitution = material.restitution;

	fixDef.isSensor = _isTrigger;
	fixDef.userData.pointer = (uintptr_t)_rigidbody;

	_rigidbody->_body->CreateFixture(&fixDef);
}

void CircleCollider::DebugDraw()
{
	if (_showCollider)
	{
		Debug::DrawCircleOutline(transform->position, _radius, Color::LightGreen);
		Vector2 rPoint = {
			transform->position.x + cosf(transform->rotation.GetRad()) * _radius,
			transform->position.y + -sinf(transform->rotation.GetRad()) * _radius,
		};
		Debug::DrawLine(transform->position, rPoint, Color::LightGreen);
	}
}
