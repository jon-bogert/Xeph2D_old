#include "Components/Rigidbody.h"

#include "Systems/Serializer.h"
#include "Systems/Physics.h"

using namespace Xeph2D;

void Rigidbody::Serializables()
{
	SERIALIZE_DEFAULT;
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
	_body = Physics::Get()._world.CreateBody(&def);

	Physics::Get().RegisterRigidbody(this);
}

void Xeph2D::Rigidbody::OnDestroy()
{
	Physics::Get().UnregisterRigidbody(this);
}

void Xeph2D::Rigidbody::UpdateTransform()
{
	transform->position = _body->GetPosition();
	transform->rotation.SetRad(-_body->GetAngle());
}
