#pragma once
#include "Scene.h"

#include <functional>

namespace Xeph2D
{
	class SceneManager final
	{
		SceneManager() {}
		static SceneManager& Get();

		std::vector<std::string> _scenes;
		std::unique_ptr<Scene> _currScene = nullptr;
		int _currIndex = -1;
		int _nextIndex = 0;
		bool _doLoadScene = false;
		std::function<void(SceneManager*, int, bool)> _loadCallback;

	public:
		~SceneManager() = default;
		SceneManager(const SceneManager& other) = delete;
		SceneManager(const SceneManager&& other) = delete;
		SceneManager operator=(const SceneManager& other) = delete;
		SceneManager operator=(const SceneManager&& other) = delete;

		static void Initialize(std::function<void(SceneManager*, int, bool)> loadCallback);
		static Scene* NewScene();

		static void AddScene(const std::string& name);
		static void LoadScene(const std::string& name);
		static void LoadScene(const int index);
		static int GetSceneIndex(const std::string& name);

		static Scene* GetCurrentScene();
		static int GetCurrentIndex();

		static void EditorInit();
		static void EditorShutdown();

		static void Serializables();

		static void Startup();
		static void Update();
		static void DebugDraw();
		static void HandleSceneChange();
		static void Shutdown();
	};
}