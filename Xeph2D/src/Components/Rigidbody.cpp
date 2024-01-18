#include "Components/Rigidbody.h"

#include "Systems/Serializer.h"
#include "Systems/Physics.h"

using namespace Xeph2D;

void Rigidbody::Serializables()
{
	SERIALIZE_DEFAULT;
	SERIALIZE_INT(m_typeAsInt);
	SERIALIZE_BOOL(m_lockRotation);
	m_type = (Type)m_typeAsInt;

}

void Rigidbody::Awake()
{
	b2BodyDef def;
	switch (m_type)
	{
	case Type::Static:
		def.type = b2_staticBody;
		break;
	case Type::Dynamic:
		def.type = b2_dynamicBody;
		break;
	case Type::Kinematic:
		def.type = b2_dynamicBody;
		break;
	}
	def.position.Set(transform->position.x, transform->position.y);
	def.angle = -transform->rotation.GetRad();
	def.fixedRotation = m_lockRotation;
	m_body = Physics::Get().m_world.CreateBody(&def);

	Physics::Get().RegisterRigidbody(this);
}

void Xeph2D::Rigidbody::OnDestroy()
{
	Physics::Get().UnregisterRigidbody(this);
}

Vector2 Xeph2D::Rigidbody::GetPosition() const
{
	return m_body->GetPosition();
}

void Xeph2D::Rigidbody::SetPosition(const Vector2 position)
{
	m_body->SetTransform(position, m_body->GetAngle());
}

Rotation Xeph2D::Rigidbody::GetRotation() const
{
	Rotation rot;
	rot.SetRad(m_body->GetAngle());
	return rot;
}

void Xeph2D::Rigidbody::SetRotation(const Rotation rotation)
{
	m_body->SetTransform(m_body->GetPosition(), rotation.GetRad());
}

void Xeph2D::Rigidbody::SetRotation(const float degrees)
{
	Rotation rotation;
	rotation.SetDeg(degrees);
	m_body->SetTransform(m_body->GetPosition(), rotation.GetRad());
}

Vector2 Xeph2D::Rigidbody::GetVelocity() const
{
	return m_body->GetLinearVelocity();
}

void Xeph2D::Rigidbody::SetVelocity(const Vector2 velocity)
{
	m_body->SetLinearVelocity(velocity);
}

float Xeph2D::Rigidbody::GetAngularVelocity() const
{
	return m_body->GetAngularVelocity() * (180.f / Math::kPi);
}

void Xeph2D::Rigidbody::SetAngularVelocity(const float angularVel)
{
	m_body->SetAngularVelocity(angularVel * Math::kPi / 180.f);
}

void Xeph2D::Rigidbody::AddForce(const Vector2 force)
{
	m_body->ApplyForceToCenter(force, true);
}

void Xeph2D::Rigidbody::AddForceToPoint(const Vector2 force, const Vector2 point)
{
	m_body->ApplyForce(force, point, true);
}

void Xeph2D::Rigidbody::AddImpulse(const Vector2 impulse)
{
	m_body->ApplyLinearImpulseToCenter(impulse, true);
}

void Xeph2D::Rigidbody::AddImpulseToPoint(const Vector2 impulse, const Vector2 point)
{
	m_body->ApplyLinearImpulse(impulse, point, true);
}

void Xeph2D::Rigidbody::SetLockRotation(const float doLock)
{
	m_body->SetFixedRotation(doLock);
}

float Xeph2D::Rigidbody::GetLockRotation() const
{
	return m_body->IsFixedRotation();
}

b2Body* Xeph2D::Rigidbody::__UnWrap()
{
	return m_body;
}

void Xeph2D::Rigidbody::UpdateTransform()
{
	transform->position = m_body->GetPosition();
	transform->rotation.SetRad(-m_body->GetAngle());
}
