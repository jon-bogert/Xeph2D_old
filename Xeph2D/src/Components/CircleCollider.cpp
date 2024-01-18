#include "Components/CircleCollider.h"

#include "Systems/Serializer.h"
#include "Systems/Physics.h"

#include "Components/Rigidbody.h"

#include <cmath>

using namespace Xeph2D;

void CircleCollider::Serializables()
{
	SERIALIZE_DEFAULT;
	SERIALIZE_FLOAT(m_radius);
	SERIALIZE_BOOL(m_isTrigger);
	SERIALIZE_BOOL(m_showCollider);
}
void CircleCollider::Awake()
{
	m_rigidbody = gameObject->GetComponent<Rigidbody>();
	if (!m_rigidbody)
		Debug::LogErr("BoxCollider -> No Rigidbody found on GameObject %s", gameObject->name.c_str());

	m_shape.m_radius = m_radius;
	b2FixtureDef fixDef;
	fixDef.shape = &m_shape;

	PhysicsMaterial material{}; // TODO - Assign in inspector

	fixDef.friction = material.friction;
	fixDef.density = material.density;
	fixDef.restitution = material.restitution;

	fixDef.isSensor = m_isTrigger;
	fixDef.userData.pointer = (uintptr_t)m_rigidbody;

	m_rigidbody->m_body->CreateFixture(&fixDef);
}

void CircleCollider::DebugDraw()
{
	if (m_showCollider)
	{
		Debug::DrawCircleOutline(transform->position, m_radius, Color::LightGreen);
		Vector2 rPoint = {
			transform->position.x + cosf(transform->rotation.GetRad()) * m_radius,
			transform->position.y + -sinf(transform->rotation.GetRad()) * m_radius,
		};
		Debug::DrawLine(transform->position, rPoint, Color::LightGreen);
	}
}
