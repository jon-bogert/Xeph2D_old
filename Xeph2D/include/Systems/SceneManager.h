#pragma once
#include "Scene.h"
#ifdef _EDITOR
#include "Editor/EditorWindows/Inspector.h"
#endif //_EDITOR

#include <functional>

namespace Xeph2D
{
	class SceneManager final
	{
	public:
		~SceneManager() = default;
		SceneManager(const SceneManager& other) = delete;
		SceneManager(const SceneManager&& other) = delete;
		SceneManager operator=(const SceneManager& other) = delete;
		SceneManager operator=(const SceneManager&& other) = delete;

		static void Initialize(std::function<void(std::unique_ptr<Component>& ptr, uint32_t compID)> scriptCallback);
		static Scene* NewScene();

		static void AddScene(const std::string& name);
		static void LoadScene(const std::string& name);
		static void LoadScene(const int index);
		static int GetSceneIndex(const std::string& name);

		static Scene* GetCurrentScene();
		static int GetCurrentIndex();
		static std::string GetCurrentName();

		static void EditorInit();
		static void EditorShutdown();

		static void Serializables();

		static void Startup();
		static void Update();
		static void DebugDraw();
		static void HandleSceneChange();
		static void Shutdown();

	private:
		void DoSceneLoading();
#ifdef _EDITOR
		friend class Edit::Inspector;
		void __AddComponentByID(GameObject* obj, uint32_t id);
#endif //_EDITOR

		SceneManager() {}
		static SceneManager& Get();

		std::vector<std::string> m_scenes;
		std::unique_ptr<Scene> m_currScene = nullptr;
		int m_currIndex = -1;
		int m_nextIndex = 0;
		bool m_doLoadScene = false;
		std::function<void(std::unique_ptr<Component>& ptr, uint32_t compID)> m_scriptCallback;
	};
}