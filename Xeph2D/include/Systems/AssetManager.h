#pragma once
#include <string>
#include <unordered_map>

#include <SFML.hpp>
#include "AudioData.h"

namespace Xeph2D
{
	class AssetManager final
	{
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

#ifdef _EDITOR
		std::unique_ptr<unsigned char[]> m_emptyImageData;
#endif //_EDITOR
		std::unordered_map<std::string, sf::Texture> m_textures;
		std::unordered_map<std::string, AudioData> m_audioSources;
		AssetManager();
		static AssetManager& Get();

		const std::string k_assetPath = "Assets/";
	};
}