#include "Components/Rigidbody.h"

#include "Systems/Serializer.h"
#include "Systems/Physics.h"

using namespace Xeph2D;

void Rigidbody::Serializables()
{
	SERIALIZE_DEFAULT;
	SERIALIZE_INT(_typeAsInt);
	SERIALIZE_BOOL(_lockRotation);
	_type = (Type)_typeAsInt;

}

void Rigidbody::Awake()
{
	b2BodyDef def;
	switch (_type)
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
	def.fixedRotation = _lockRotation;
	_body = Physics::Get()._world.CreateBody(&def);

	Physics::Get().RegisterRigidbody(this);
}

void Xeph2D::Rigidbody::OnDestroy()
{
	Physics::Get().UnregisterRigidbody(this);
}

Vector2 Xeph2D::Rigidbody::GetPosition() const
{
	return _body->GetPosition();
}

void Xeph2D::Rigidbody::SetPosition(const Vector2 position)
{
	_body->SetTransform(position, _body->GetAngle());
}

Rotation Xeph2D::Rigidbody::GetRotation() const
{
	Rotation rot;
	rot.SetRad(_body->GetAngle());
	return rot;
}

void Xeph2D::Rigidbody::SetRotation(const Rotation rotation)
{
	_body->SetTransform(_body->GetPosition(), rotation.GetRad());
}

void Xeph2D::Rigidbody::SetRotation(const float degrees)
{
	Rotation rotation;
	rotation.SetDeg(degrees);
	_body->SetTransform(_body->GetPosition(), rotation.GetRad());
}

Vector2 Xeph2D::Rigidbody::GetVelocity() const
{
	return _body->GetLinearVelocity();
}

void Xeph2D::Rigidbody::SetVelocity(const Vector2 velocity)
{
	_body->SetLinearVelocity(velocity);
}

float Xeph2D::Rigidbody::GetAngularVelocity() const
{
	return _body->GetAngularVelocity() * (180.f / Math::kPi);
}

void Xeph2D::Rigidbody::SetAngularVelocity(const float angularVel)
{
	_body->SetAngularVelocity(angularVel * Math::kPi / 180.f);
}

void Xeph2D::Rigidbody::AddForce(const Vector2 force)
{
	_body->ApplyForceToCenter(force, true);
}

void Xeph2D::Rigidbody::AddForceToPoint(const Vector2 force, const Vector2 point)
{
	_body->ApplyForce(force, point, true);
}

void Xeph2D::Rigidbody::AddImpulse(const Vector2 impulse)
{
	_body->ApplyLinearImpulseToCenter(impulse, true);
}

void Xeph2D::Rigidbody::AddImpulseToPoint(const Vector2 impulse, const Vector2 point)
{
	_body->ApplyLinearImpulse(impulse, point, true);
}

void Xeph2D::Rigidbody::SetLockRotation(const float doLock)
{
	_body->SetFixedRotation(doLock);
}

float Xeph2D::Rigidbody::GetLockRotation() const
{
	return _body->IsFixedRotation();
}

b2Body* Xeph2D::Rigidbody::__UnWrap()
{
	return _body;
}

void Xeph2D::Rigidbody::UpdateTransform()
{
	transform->position = _body->GetPosition();
	transform->rotation.SetRad(-_body->GetAngle());
}
