#pragma once
#include <memory>
#include <vector>

#include "Component.h"

namespace Xeph2D
{
	class GameObject
	{
	public:
		uint32_t instID = 0;
		Transform transform = {};

	private:
		GameObject* _parent = nullptr;
		//std::vector<std::unique_ptr<GameObject>> children 
		std::vector<std::unique_ptr<Component>> _components;

	public:
		template <typename _Component>
		_Component* AddComponent();
		template <typename _Component>
		_Component* GetComponent();

		void EditorInit();
		void EditorShutdown();

		void Awake();
		void Start();
		void OnEnable();
		void EarlyUpdate();
		void Update();
		void LateUpdate();
		void DebugDraw();
		void OnDisable();
		void OnDestroy();

		GameObject* GetParent() const;
		void SetParent(GameObject* parent);
	};

	template<typename _Component>
	inline _Component* GameObject::AddComponent()
	{
		static_assert(std::is_base_of_v<Component, _Component>, "GameObject ->  Cannot add component not of type Xeph2D::Component");
		Component* newComp = _components.emplace_back(std::make_unique<_Component>()).get();
		newComp->Register(this);
		return reinterpret_cast<_Component*>(newComp);
	}
	template<typename _Component>
	inline _Component* GameObject::GetComponent()
	{
		static_assert(std::is_base_of_v<Component, _Component>, "GameObject ->  Cannot add component not of type Xeph2D::Component");
		
		for (auto& comp : _components)
		{
			if (comp->GetTypeID() == _Component::typeID)
				return (_Component*)comp.get();
		}
		return nullptr;
	}
}