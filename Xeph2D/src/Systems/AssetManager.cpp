#include "Systems/AssetManager.h"
#include "Systems/Debug.h"

using namespace Xeph2D;

AssetManager& AssetManager::Get()
{
	static AssetManager instance;
	return instance;
}

sf::Texture* Xeph2D::AssetManager::LoadTextureFromFile(const std::string& filename, std::string tag)
{
	std::string fullpath = Get().assetPath + "Textures/" + filename;
	if (tag == "")
		tag = TagFromFileName(filename);

	sf::Texture& tex = Get()._textures[tag];
	if (!tex.loadFromFile(fullpath))
	{
		Debug::LogErr("Could not open file at path: %s", fullpath.c_str());
		return nullptr;
	}
	return &tex;
}

AudioData* Xeph2D::AssetManager::LoadAudioDataFromFile(const std::string& filename, bool isStreamed, std::string tag)
{
	std::string fullpath = Get().assetPath + "Audio/" + filename;
	if (tag == "")
		tag = TagFromFileName(filename);
	
	AudioData& source = Get()._audioSources[tag];
	if (!source.LoadAssetData(fullpath, isStreamed))
	{
		Debug::LogErr("Could not open file at path: %s", fullpath.c_str());
		return nullptr;
	}
	return &source;
}

sf::Texture* Xeph2D::AssetManager::GetTexture(const std::string& tag)
{
	auto result = Get()._textures.find(tag);

	if (result == Get()._textures.end())
	{
		Debug::LogErr("Could not find texture with tag: %s", tag.c_str());
		return nullptr;
	}

	return &result->second;
}

void Xeph2D::AssetManager::UnloadTexture(const std::string& tag)
{
	auto result = Get()._textures.find(tag);

	if (result == Get()._textures.end())
	{
		Debug::LogErr("Could not unload texture with tag: %s", tag.c_str());
		return;
	}

	Get()._textures.erase(result);
}

void Xeph2D::AssetManager::UnloadAllTextures()
{
	Get()._textures.clear();
}

AudioData* Xeph2D::AssetManager::GetAudioData(const std::string& tag)
{
	auto result = Get()._audioSources.find(tag);

	if (result == Get()._audioSources.end())
	{
		Debug::LogErr("Could not find audio source with tag: %s", tag.c_str());
		return nullptr;
	}

	return &result->second;
}

void Xeph2D::AssetManager::UnloadAudioData(const std::string& tag)
{
	auto result = Get()._audioSources.find(tag);

	if (result == Get()._audioSources.end())
	{
		Debug::LogErr("Could not find audio source with tag: %s", tag.c_str());
		return;
	}

	Get()._audioSources.erase(result);
}

void Xeph2D::AssetManager::UnloadAllAudioData(bool includeStreams)
{
	if (includeStreams)
	{
		Get()._audioSources.clear();
		return;
	}
	
	for (auto it = Get()._audioSources.begin(); it != Get()._audioSources.end();)
	{
		if (it->second.GetIsStreamed())
			it++;
		else
			it = Get()._audioSources.erase(it);
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
