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
		std::string name = "GameObject";

		template <typename _Component>
		_Component* AddComponent();
		template <typename _Component>
		_Component* GetComponent();

		void RemoveComponent(Component* comp);
		void RemoveComponent(uint32_t compID);

#ifdef _EDITOR
		void MoveUp(size_t index);
		void MoveDown(size_t index);
#endif //_EDITOR

		std::unique_ptr<Component>& __GetNewEmptyComponentPtr();

		void Serializables();

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

	private:
		GameObject* m_parent = nullptr;
		//std::vector<std::unique_ptr<GameObject>> children 
		std::vector<std::unique_ptr<Component>> m_components;
	};

	template<typename _Component>
	inline _Component* GameObject::AddComponent()
	{
		static_assert(std::is_base_of_v<Component, _Component>, "GameObject ->  Cannot add component not of type Xeph2D::Component");
		Component* newComp = m_components.emplace_back(std::make_unique<_Component>()).get();
		newComp->Register(this);
		return reinterpret_cast<_Component*>(newComp);
	}
	template<typename _Component>
	inline _Component* GameObject::GetComponent()
	{
		static_assert(std::is_base_of_v<Component, _Component>, "GameObject ->  Cannot add component not of type Xeph2D::Component");
		
		for (auto& comp : m_components)
		{
			if (comp->GetTypeID() == _Component::typeID)
				return (_Component*)comp.get();
		}
		return nullptr;
	}
}