#include "Components/BoxCollider.h"
#include "Systems/Serializer.h"
#include "Systems/Physics.h"

#include "Components/Rigidbody.h"

using namespace Xeph2D;

void BoxCollider::Serializables()
{
	SERIALIZE_DEFAULT;
	SERIALIZE_VEC2(m_dimensions);
	SERIALIZE_BOOL(m_isTrigger);
	SERIALIZE_BOOL(m_showCollider);
}

void Xeph2D::BoxCollider::Awake()
{
	m_rigidbody = gameObject->GetComponent<Rigidbody>();
	if (!m_rigidbody)
		Debug::LogErr("BoxCollider -> No Rigidbody found on GameObject %s", gameObject->name.c_str());

	m_shape.SetAsBox(m_dimensions.x * 0.5f, m_dimensions.y * 0.5f);
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

void Xeph2D::BoxCollider::DebugDraw()
{
	if (m_showCollider)
	{
		Debug::DrawBoxOutline(transform->position, m_dimensions, transform->rotation.GetDeg(), Color::LightGreen);
	}
}
