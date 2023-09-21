#include "AudioSource.h"
#include "Systems/Debug.h"

using namespace Xeph2D;
using MusicPtr = std::shared_ptr<sf::Music>;
using SoundBufferPtr = std::shared_ptr<sf::SoundBuffer>;

void Xeph2D::AudioData::LoadAssetData(const std::string& filepath)
{
	_filepath = filepath;
	if (_isStreamed)
	{
		_source = std::make_shared<sf::Music>();
		sf::Music* source = std::any_cast<MusicPtr>(_source).get();
		source->openFromFile(filepath);
		return;
	}

	_source = std::make_shared<sf::SoundBuffer>();
	sf::SoundBuffer* source = std::any_cast<SoundBufferPtr>(_source).get(); 
	source->loadFromFile(filepath);
}

void Xeph2D::AudioData::LoadAssetData(const std::string& filepath, bool isStreamed)
{
	_isStreamed = isStreamed;
	LoadAssetData(filepath);
}

void Xeph2D::AudioData::SetIsStreamed(const bool isStreamed)
{
	if (_isStreamed == isStreamed)
		return;

	_isStreamed = isStreamed;
	LoadAssetData(_filepath);
}

bool Xeph2D::AudioData::GetIsStreamed() const
{
	return _isStreamed;
}

sf::SoundBuffer* Xeph2D::AudioData::GetBuffer() const
{
	if (_isStreamed)
	{
		Debug::LogErr("AudioSource.GetBuffer -> audio source is streamed");
		return nullptr;
	}
	return std::any_cast<SoundBufferPtr>(_source).get();
}

sf::Music* Xeph2D::AudioData::GetStream() const
{
	if (!_isStreamed)
	{
		Debug::LogErr("AudioSource.GetStream -> audio source is not streamed");
		return nullptr;
	}
	return std::any_cast<MusicPtr>(_source).get();
}
