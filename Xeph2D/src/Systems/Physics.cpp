#include "Systems/Physics.h"

#include "GameObject.h"
#include "Components/Rigidbody.h"
#include "Systems/Time.h"

using namespace Xeph2D;

Physics::Physics()
{
	m_world.SetContactListener(&m_listener);
}

void Physics::Update()
{
	Get().m_world.Step(Time::DeltaTime(), Get().m_velIter, Get().m_posIter);
	for (Rigidbody* rb : Get().m_rigidbodies)
		rb->UpdateTransform();
	Get().CallCollisionCallbacks();
}

void Xeph2D::Physics::RegisterRigidbody(Rigidbody* rb)
{
	m_rigidbodies.push_back(rb);
}

void Xeph2D::Physics::UnregisterRigidbody(Rigidbody* rb)
{
	for (auto it = m_rigidbodies.begin(); it != m_rigidbodies.end(); it++)
	{
		if (*it == rb)
		{
			m_rigidbodies.erase(it);
			return;
		}
	}
}

void Physics::CallCollisionCallbacks()
{
	for (CollisionPair& cp : Get().m_collisionPairs)
	{
		if (cp.isTrigger && cp.isExit)
		{
			cp.a->onTriggerExit.Invoke(cp.b);
			cp.b->onTriggerExit.Invoke(cp.a);
		}
		else if (cp.isTrigger)
		{
			cp.a->onTriggerEnter.Invoke(cp.b);
			cp.b->onTriggerEnter.Invoke(cp.a);
		}
		else if (cp.isExit)
		{
			cp.a->onCollisionExit.Invoke(cp.b);
			cp.b->onCollisionExit.Invoke(cp.a);
		}
		else
		{
			cp.a->onCollisionEnter.Invoke(cp.b);
			cp.b->onCollisionEnter.Invoke(cp.a);
		}
	}
	m_collisionPairs.clear();
}

void Xeph2D::Physics::AddCollisionPair(Rigidbody* a, Rigidbody* b, bool isTrigger, bool isExit)
{
	Get().m_collisionPairs.push_back({ a, b, isTrigger, isExit });
}

void Xeph2D::CollisionListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	Rigidbody* objA = (Rigidbody*)fixtureA->GetUserData().pointer;
	Rigidbody* objB = (Rigidbody*)fixtureB->GetUserData().pointer;

	// Check if one of the fixtures is a sensor
	if (fixtureA->IsSensor() || fixtureB->IsSensor())
	{
		Physics::Get().AddCollisionPair(objA, objB, true);
		return;
	}

	Physics::Get().AddCollisionPair(objA, objB);
}

void Xeph2D::CollisionListener::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	Rigidbody* objA = (Rigidbody*)fixtureA->GetUserData().pointer;
	Rigidbody* objB = (Rigidbody*)fixtureB->GetUserData().pointer;

	// Check if one of the fixtures is a sensor
	if (fixtureA->IsSensor() || fixtureB->IsSensor())
	{
		Physics::Get().AddCollisionPair(objA, objB, true, true);
		return;
	}

	Physics::Get().AddCollisionPair(objA, objB, false, true);
}
