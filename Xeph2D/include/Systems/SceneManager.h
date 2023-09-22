#pragma once
#include "Scene.h"

#include <functional>

namespace Xeph2D
{
	class SceneManager final
	{
		SceneManager() {}
		static SceneManager& Get();

		std::vector<std::unique_ptr<Scene>> _scenes;
		int _currIndex = -1;

	public:
		~SceneManager() = default;
		SceneManager(const SceneManager& other) = delete;
		SceneManager(const SceneManager&& other) = delete;
		SceneManager operator=(const SceneManager& other) = delete;
		SceneManager operator=(const SceneManager&& other) = delete;

		static void Initialize(std::function<void(SceneManager*)> loadProcess);

		static Scene* AddScene(const std::string& name);
		static void LoadScene(const std::string& name);
		static void LoadScene(const int index);
		static int GetSceneIndex(const std::string& name);
		static Scene* GetScene(const std::string& name);
		static Scene* GetScene(const int index);

		static Scene* GetCurrentScene();
		static int GetCurrentIndex();

		static void Startup();
		static void HandleSceneChange();
		static void Shutdown();

		static void EarlyUpdate();
		static void Update();
		static void LateUpdate();
	};
}