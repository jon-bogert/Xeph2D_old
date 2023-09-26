#pragma once
#include <string>
#include <unordered_map>

#include <SFML.hpp>
#include "AudioData.h"

namespace Xeph2D
{
	class AssetManager final
	{
#ifdef _EDITOR
		std::unique_ptr<unsigned char[]> _emptyImageData;
#endif //_EDITOR
		std::unordered_map<std::string, sf::Texture> _textures;
		std::unordered_map<std::string, AudioData> _audioSources;
		AssetManager();
		static AssetManager& Get();

		const std::string assetPath = "Assets/";

	public:
		~AssetManager() = default;
		AssetManager(const AssetManager& other) = delete;
		AssetManager(const AssetManager&& other) = delete;
		AssetManager operator=(const AssetManager& other) = delete;
		AssetManager operator=(const AssetManager&& other) = delete;

		static sf::Texture* LoadTextureFromFile(const std::string& filename, std::string tag = "");
		//static void LoadTextureFromData(const void* data); // TODO
		static AudioData* LoadAudioDataFromFile(const std::string& filename, bool isStreamed = true, std::string tag = "");
		
		static sf::Texture* GetTexture(const std::string& tag);
		static void UnloadTexture(const std::string& tag);
		static void UnloadAllTextures();
		
		static AudioData* GetAudioData(const std::string& tag);
		static void UnloadAudioData(const std::string& tag);
		static void UnloadAllAudioData(bool includeStreams = false);

		static void UnloadAll();
	private:
		static std::string TagFromFileName(const std::string& filename);
	};
}