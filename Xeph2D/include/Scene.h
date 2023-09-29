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

		void Destroy(GameObject* obj);

		std::vector<GameObject*> GetAllObjects();
#ifdef _EDITOR
		void MoveUp(size_t index);
		void MoveDown(size_t index);
#endif //_EDITOR
	};
}