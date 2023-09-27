#include "Components/BoxCollider.h"
#include "Systems/Serializer.h"
#include "Systems/Physics.h"

#include "Components/Rigidbody.h"

using namespace Xeph2D;

void BoxCollider::Serializables()
{
	SERIALIZE_DEFAULT;
	SERIALIZE_VEC2(_dimensions);
	SERIALIZE_BOOL(_isTrigger);
	SERIALIZE_BOOL(_showCollider);
}

void Xeph2D::BoxCollider::Awake()
{
	_rigidbody = gameObject->GetComponent<Rigidbody>();
	if (!_rigidbody)
		Debug::LogErr("BoxCollider -> No Rigidbody found on GameObject %s", gameObject->name.c_str());

	_shape.SetAsBox(_dimensions.x * 0.5f, _dimensions.y * 0.5f);
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

void Xeph2D::BoxCollider::DebugDraw()
{
	if (_showCollider)
	{
		Debug::DrawBoxOutline(transform->position, _dimensions, transform->rotation.GetDeg(), Color::LightGreen);
	}
}
