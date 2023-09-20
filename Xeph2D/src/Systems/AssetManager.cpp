#include "Systems/AssetManager.h"
#include "Systems/Debug.h"

using namespace Xeph2D;

AssetManager& AssetManager::Get()
{
	static AssetManager instance;
	return instance;
}

sf::Texture& Xeph2D::AssetManager::LoadTextureFromFile(const std::string& filename, std::string tag)
{
	std::string fullpath = Get().assetPath + "Textures/" + filename;
	if (tag == "")
	{
		for (int i = 0; i < filename.size(); ++i)
		{
			if (filename[i] == '.')
			{
				tag = filename.substr(0, i);
			}
		}
	}
	sf::Texture& tex = Get()._textures[tag];
	tex.loadFromFile(fullpath);

	return tex;
}

sf::Texture& Xeph2D::AssetManager::GetTexture(const std::string& tag)
{
	auto result = Get()._textures.find(tag);

	if (result == Get()._textures.end())
		Debug::LogErr("Could not find texture with tag: %s", tag.c_str());

	return result->second;
}

void Xeph2D::AssetManager::UnloadTexture(const std::string tag)
{
	auto result = Get()._textures.find(tag);

	if (result == Get()._textures.end())
	{
		Debug::LogErr("Could not unload texture with tag: %s", tag.c_str());
		return;
	}

	Get()._textures.erase(result);
}

void Xeph2D::AssetManager::UnloadTextures()
{
	Get()._textures.clear();
}

void Xeph2D::AssetManager::UnloadAll()
{
	UnloadTextures();
}
