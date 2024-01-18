#include "Systems/AssetManager.h"
#include "Systems/Debug.h"

#ifdef _EDITOR
#include "../res/no_image_png.h"
#endif //_EDITOR

using namespace Xeph2D;

Xeph2D::AssetManager::AssetManager()
{
#ifdef _EDITOR
	size_t size = 0;
	res::no_image_png(m_emptyImageData, size);
	sf::Texture tex;
	tex.loadFromMemory(m_emptyImageData.get(), size);
	m_textures["__no-image"] = tex;
#endif //_EDITOR
}

AssetManager& AssetManager::Get()
{
	static AssetManager instance;
	return instance;
}

sf::Texture* Xeph2D::AssetManager::LoadTextureFromFile(const std::string& filename, std::string tag)
{
	std::string fullpath = Get().k_assetPath + "Textures/" + filename;
	if (tag == "")
		tag = TagFromFileName(filename);

	sf::Texture& tex = Get().m_textures[tag];
	if (!tex.loadFromFile(fullpath))
	{
		Debug::LogErr("Could not open file at path: %s", fullpath.c_str());
		Get().m_textures.erase(tag);
		return nullptr;
	}
	return &tex;
}

AudioData* Xeph2D::AssetManager::LoadAudioDataFromFile(const std::string& filename, bool isStreamed, std::string tag)
{
	std::string fullpath = Get().k_assetPath + "Audio/" + filename;
	if (tag == "")
		tag = TagFromFileName(filename);
	
	AudioData& source = Get().m_audioSources[tag];
	if (!source.LoadAssetData(fullpath, isStreamed))
	{
		Debug::LogErr("Could not open file at path: %s", fullpath.c_str());
		return nullptr;
	}
	return &source;
}

sf::Texture* Xeph2D::AssetManager::GetTexture(const std::string& tag)
{
	auto result = Get().m_textures.find(tag);

	if (result == Get().m_textures.end())
	{
		Debug::LogErr("Could not find texture with tag: %s", tag.c_str());
		return nullptr;
	}

	return &result->second;
}

void Xeph2D::AssetManager::UnloadTexture(const std::string& tag)
{
	auto result = Get().m_textures.find(tag);

	if (result == Get().m_textures.end())
	{
		Debug::LogErr("Could not unload texture with tag: %s", tag.c_str());
		return;
	}

	Get().m_textures.erase(result);
}

void Xeph2D::AssetManager::UnloadAllTextures()
{
	Get().m_textures.clear();
}

AudioData* Xeph2D::AssetManager::GetAudioData(const std::string& tag)
{
	auto result = Get().m_audioSources.find(tag);

	if (result == Get().m_audioSources.end())
	{
		Debug::LogErr("Could not find audio source with tag: %s", tag.c_str());
		return nullptr;
	}

	return &result->second;
}

void Xeph2D::AssetManager::UnloadAudioData(const std::string& tag)
{
	auto result = Get().m_audioSources.find(tag);

	if (result == Get().m_audioSources.end())
	{
		Debug::LogErr("Could not find audio source with tag: %s", tag.c_str());
		return;
	}

	Get().m_audioSources.erase(result);
}

void Xeph2D::AssetManager::UnloadAllAudioData(bool includeStreams)
{
	if (includeStreams)
	{
		Get().m_audioSources.clear();
		return;
	}
	
	for (auto it = Get().m_audioSources.begin(); it != Get().m_audioSources.end();)
	{
		if (it->second.GetIsStreamed())
			it++;
		else
			it = Get().m_audioSources.erase(it);
	}
}

void Xeph2D::AssetManager::UnloadAll()
{
	UnloadAllTextures();
	UnloadAllAudioData();
}

std::string Xeph2D::AssetManager::TagFromFileName(const std::string& filename)
{
	std::string tag;
	for (int i = 0; i < filename.size(); ++i)
	{
		if (filename[i] == '.')
		{
			tag = filename.substr(0, i);
		}
	}
	return tag;
}
