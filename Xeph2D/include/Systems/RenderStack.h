#pragma once
#include <vector>
#include <functional>
#include <SFML.hpp>
#include "GameObject.h"

namespace Xeph2D
{
	class RenderStack final
	{
		struct Item
		{
			enum class Type { Sprite };
			int order = 0;
			const GameObject* object = nullptr;
			Type type = Type::Sprite;
			void* data = nullptr;

			Item() = default;
			constexpr Item(int order, GameObject* object, Type type, void* data)
				: order(order), object(object), type(type), data(data) {}
		};

		struct Callback
		{
			void* comp;
			std::function<void(void)> func;
		};

		std::vector<Callback> _drawCallbacks;
		std::vector<Item> _stack;

		RenderStack() {}
		static RenderStack& Get();

	public:
		~RenderStack() = default;
		RenderStack(const RenderStack& other) = delete;
		RenderStack(const RenderStack&& other) = delete;
		RenderStack operator=(const RenderStack& other) = delete;
		RenderStack operator=(const RenderStack&& other) = delete;

		static void AddSprite(const GameObject* gameObject, sf::Sprite* sprite, int order = 0);
		static void Draw();

		static void SubscribeDrawCall(void* component, std::function<void(void)> drawCall);
		static void UnubscribeDrawCall(void* component);
	};
}