#pragma once
#include "GameObject.h"

#include <box2d/box2d.h>

#include <functional>
#include <vector>

#define RIGIDBODY_CALLBACK(func) std::bind(&func, this, std::placeholders::_1)

namespace Xeph2D
{
	class Rigidbody : public Component
	{
		friend class Physics;
		friend class BoxCollider;
		friend class CircleCollider;
	public:
		enum Type { Static, Dynamic, Kinematic };

		COMP_HEADER_STD(0x00000004);
		class EventCallback
		{
			friend class Physics;
			std::vector<std::function<void(Rigidbody*)>> m_callbacks;
			void Invoke(Rigidbody* rigidbody)
			{
				for (std::function<void(Rigidbody*)>& cb : m_callbacks)
				{
					cb(rigidbody);
				}
			}
		public:
			void Subscribe(std::function<void(Rigidbody*)> callback)
			{
				m_callbacks.push_back(callback);
			}
			void UnsubscribeAll()
			{
				m_callbacks.clear();
			}
		};

		EventCallback onTriggerEnter;
		EventCallback onTriggerExit;
		EventCallback onCollisionEnter;
		EventCallback onCollisionExit;

		void Awake() override;
		void OnDestroy() override;

		Vector2 GetPosition() const;
		void SetPosition(const Vector2);

		Rotation GetRotation() const;
		void SetRotation(const Rotation rotation);
		void SetRotation(const float degrees);

		Vector2 GetVelocity() const;
		void SetVelocity(const Vector2 velocity);

		float GetAngularVelocity() const;
		void SetAngularVelocity(const float angularVel);

		void AddForce(const Vector2 force);
		void AddForceToPoint(const Vector2 force, const Vector2 point);
		void AddImpulse(const Vector2 impulse);
		void AddImpulseToPoint(const Vector2 impulse, const Vector2 point);

		void SetLockRotation(const float doLock);
		float GetLockRotation() const;

		b2Body* __UnWrap();

	private:
		void UpdateTransform();

		Type m_type = Type::Static;
		b2Body* m_body = nullptr;
		int m_typeAsInt = 0;
		bool m_lockRotation = false;
	};
}