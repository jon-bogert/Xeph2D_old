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

        Physics();
        static Physics& Get() { static Physics instance; return instance; }

        struct CollisionPair
        {
            Rigidbody* a;
            Rigidbody* b;
            bool isTrigger;
            bool isExit;
        };

        b2Vec2 _gravity = { 0.f, -10.f };
        b2World _world = b2World(_gravity);
        std::vector<Rigidbody*> _rigidbodies;

        CollisionListener _listener;
        std::vector<CollisionPair> _collisionPairs;

    public:
        ~Physics() = default;
        Physics(const Physics& other) = delete;
        Physics(const Physics&& other) = delete;
        Physics operator=(const Physics& other) = delete;
        Physics operator=(const Physics&& other) = delete;

        static void Update(const float& deltaTime, int velIter, int posItr);

    public:
        void RegisterRigidbody(Rigidbody* rb);
        void UnregisterRigidbody(Rigidbody* rb);
        void CallCollisionCallbacks();
        void AddCollisionPair(Rigidbody* a, Rigidbody* b, bool isTrigger = false, bool isExit = false);
    };

}