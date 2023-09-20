#pragma once
#include <string>
#include <unordered_map>

#include <SFML.hpp>

namespace Xeph2D
{
	class AssetManager final
	{
		std::unordered_map<std::string, sf::Texture> _textures;
		AssetManager() {}
		static AssetManager& Get();

		const std::string assetPath = "Assets/";

	public:
		~AssetManager() = default;
		AssetManager(const AssetManager& other) = delete;
		AssetManager(const AssetManager&& other) = delete;
		AssetManager operator=(const AssetManager& other) = delete;
		AssetManager operator=(const AssetManager&& other) = delete;

		static sf::Texture& LoadTextureFromFile(const std::string& filename, std::string tag = "");
		//static void LoadTextureFromData(const void* data); // TODO

		static sf::Texture& GetTexture(const std::string& tag);
		static void UnloadTexture(const std::string tag);
		static void UnloadTextures();

		static void UnloadAll();
	};
}