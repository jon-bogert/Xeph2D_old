#include "AudioData.h"
#include "Systems/Debug.h"

using namespace Xeph2D;
using MusicPtr = std::shared_ptr<sf::Music>;
using SoundBufferPtr = std::shared_ptr<sf::SoundBuffer>;

bool AudioData::LoadAssetData(const std::string& filepath)
{
	m_filepath = filepath;
	if (m_isStreamed)
	{
		m_source = std::make_shared<sf::Music>();
		sf::Music* source = std::any_cast<MusicPtr>(m_source).get();
		if (!source->openFromFile(filepath))
			return false;

		return true;
	}

	m_source = std::make_shared<sf::SoundBuffer>();
	sf::SoundBuffer* source = std::any_cast<SoundBufferPtr>(m_source).get(); 
	if (!source->loadFromFile(filepath))
		return false;

	return true;
}

bool AudioData::LoadAssetData(const std::string& filepath, bool isStreamed)
{
	m_isStreamed = isStreamed;
	return LoadAssetData(filepath);
}

void AudioData::SetIsStreamed(const bool isStreamed)
{
	if (m_isStreamed == isStreamed)
		return;

	m_isStreamed = isStreamed;
	LoadAssetData(m_filepath);
}

bool AudioData::GetIsStreamed() const
{
	return m_isStreamed;
}

sf::SoundBuffer* Xeph2D::AudioData::GetBuffer() const
{
	if (m_isStreamed)
	{
		Debug::LogErr("AudioSource.GetBuffer -> audio source is streamed");
		return nullptr;
	}
	return std::any_cast<SoundBufferPtr>(m_source).get();
}

sf::Music* AudioData::GetStream() const
{
	if (!m_isStreamed)
	{
		Debug::LogErr("AudioSource.GetStream -> audio source is not streamed");
		return nullptr;
	}
	return std::any_cast<MusicPtr>(m_source).get();
}
