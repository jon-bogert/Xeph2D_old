#pragma once
#include "GameObject.h"

#include <box2d/box2d.h>

#include <functional>
#include <vector>

namespace Xeph2D
{
	class Rigidbody : public Component
	{
		friend class Physics;
		friend class BoxCollider;
	public:
		enum Type { Static, Dynamic, Kinematic };

	private:
		Type _type = Type::Static;
		b2Body* _body = nullptr;
		int _typeAsInt = 0;

	public:
		COMP_HEADER_STD(4);
		class EventCallback
		{
			friend class Physics;
			std::vector<std::function<void(Rigidbody*)>> _callbacks;
			void Invoke(Rigidbody* rigidbody)
			{
				for (std::function<void(Rigidbody*)>& cb : _callbacks)
				{
					cb(rigidbody);
				}
			}
		public:
			void Subscribe(std::function<void(Rigidbody*)> callback)
			{
				_callbacks.push_back(callback);
			}
			void UnsubscribeAll()
			{
				_callbacks.clear();
			}
		};

		EventCallback onTriggerEnter;
		EventCallback onTriggerExit;
		EventCallback onCollisionEnter;
		EventCallback onCollisionExit;

		void Awake() override;
		void OnDestroy() override;

	private:
		void UpdateTransform();
	};
}