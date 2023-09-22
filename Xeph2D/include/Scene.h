#pragma once
#include "GameObject.h"

namespace Xeph2D
{
	class Scene
	{
	protected:
		std::vector<std::unique_ptr<GameObject>> _gameObjects;
		std::vector<GameObject*> _rootObjects;

	public:
		Scene() = default;
		virtual ~Scene() = default;

		GameObject* AddGameObject(GameObject* parent = nullptr);

		void Awake();
		void Start();
		void OnEnable();
		void EarlyUpdate();
		void Update();
		void LateUpdate();
		void OnDisable();
		void OnDestroy();
	};
}