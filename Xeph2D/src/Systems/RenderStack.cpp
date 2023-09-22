#include "Systems/RenderStack.h"
#include "Systems/WindowManager.h"
#include "Systems/Debug.h"

using namespace Xeph2D;

RenderStack& RenderStack::Get()
{
	static RenderStack instance;
	return instance;
}

void RenderStack::AddSprite(const GameObject* gameObject, sf::Sprite* sprite, int order)
{
	Item item;
	item.order = order;
	item.object = gameObject;
	item.type = Item::Type::Sprite;
	item.data = static_cast<void*>(sprite);

	Get()._stack.push_back(item);
	std::sort(Get()._stack.begin(), Get()._stack.end(), [](const Item& i0, const Item& i1) { return i0.order < i1.order; });
}

void RenderStack::Execute()
{
	for (Callback& callback : Get()._drawCallbacks)
	{
		callback.func();
	}

	WindowManager::Begin();
	for (Item& i : Get()._stack)
	{
		switch (i.type)
		{
		case Item::Type::Sprite:
			WindowManager::DrawSprite(i.object, static_cast<sf::Sprite*>(i.data));
			break;
		default:
			Debug::LogErr("RenderStack::Dump -> Type Enum not included");
			break;
		}
	}
	//TODO - Draw UI
	Debug::DrawToWindow();
	WindowManager::End();
	Get()._stack.clear();
}

void Xeph2D::RenderStack::SubscribeDrawCall(void* component, std::function<void(void)> drawCall)
{
	Get()._drawCallbacks.push_back({ component, drawCall });
}

void Xeph2D::RenderStack::UnubscribeDrawCall(void* component)
{
	//std::remove_if(Get()._drawCallbacks.begin(), Get()._drawCallbacks.end(),
	//	[=](const Callback& x) { return component == x.comp; });
	for (auto it = Get()._drawCallbacks.begin(); it != Get()._drawCallbacks.end();)
	{
		if (it->comp == component)
			it = Get()._drawCallbacks.erase(it);
		else
			it++;
	}
}
