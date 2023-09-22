#pragma once

namespace Xeph2D
{
	class SceneManager final
	{
		SceneManager() {}
		static SceneManager& Get();

	public:
		~SceneManager() = default;
		SceneManager(const SceneManager& other) = delete;
		SceneManager(const SceneManager&& other) = delete;
		SceneManager operator=(const SceneManager& other) = delete;
		SceneManager operator=(const SceneManager&& other) = delete;
	};
}