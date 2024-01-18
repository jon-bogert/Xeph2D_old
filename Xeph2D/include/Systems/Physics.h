#pragma once
#include <box2d/box2d.h>
#include <vector>

namespace Xeph2D
{
    class Rigidbody;
    class CollisionListener : public b2ContactListener
    {
        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
    };

    struct PhysicsMaterial
    {
        float friction = 0.5f;
        float density = 1.f;
        float restitution = 0.f;
    };

    class Physics final
    {
        friend class Rigidbody;
        friend class CollisionListener;

    public:
        ~Physics() = default;
        Physics(const Physics& other) = delete;
        Physics(const Physics&& other) = delete;
        Physics operator=(const Physics& other) = delete;
        Physics operator=(const Physics&& other) = delete;

        static void Update();

    private:
        void RegisterRigidbody(Rigidbody* rb);
        void UnregisterRigidbody(Rigidbody* rb);
        void CallCollisionCallbacks();
        void AddCollisionPair(Rigidbody* a, Rigidbody* b, bool isTrigger = false, bool isExit = false);

        Physics();
        static Physics& Get() { static Physics instance; return instance; }

        struct CollisionPair
        {
            Rigidbody* a;
            Rigidbody* b;
            bool isTrigger;
            bool isExit;
        };

        int m_velIter = 6;
        int m_posIter = 2;

        b2Vec2 m_gravity = { 0.f, -10.f };
        b2World m_world = b2World(m_gravity);
        std::vector<Rigidbody*> m_rigidbodies;

        CollisionListener m_listener;
        std::vector<CollisionPair> m_collisionPairs;
    };

}