#include "Components/AudioSource.h"
#include "Systems/AssetManager.h"
#include "Systems/Serializer.h"

using namespace Xeph2D;
#define __CALL(func) if  (m_data) { if (IsStreamed()) { m_data->GetStream()->func(); return; } m_sound->func(); }
#define __CALL_GET(func) if  (m_data) { if (IsStreamed()) { return m_data->GetStream()->func(); } return m_sound->func(); }
#define __CALL_SET(func, val) if  (m_data) { if (IsStreamed()) { m_data->GetStream()->func(val); return; } m_sound->func(val); }

void AudioSource::Serializables()
{
	SERIALIZE_DEFAULT;
	SERIALIZE_STRING(m_audioDataKey);
}

void Xeph2D::AudioSource::EditorInit()
{
	Awake();
}

void Xeph2D::AudioSource::Awake()
{
	LoadAudioByTag(m_audioDataKey);
}

void Xeph2D::AudioSource::LoadAudioByTag(const std::string& tag)
{
	AudioData* data = AssetManager::GetAudioData(tag);
	m_data = data;
	if (data == nullptr)
	{
		Debug::LogErr("AudioSource %s tried to get audio data '%s'", gameObject->name.c_str(), tag.c_str());
		return;
	}
	if (IsStreamed())
		return;

	SoundSetup();
}

void Xeph2D::AudioSource::Play()	{ __CALL(play) }
void Xeph2D::AudioSource::Pause()	{ __CALL(pause) }
void Xeph2D::AudioSource::Stop()	{ __CALL(stop) }

AudioStatus Xeph2D::AudioSource::Status()
{
	sf::SoundSource::Status status;
	if (IsStreamed())
		status = m_data->GetStream()->getStatus();
	else
		status = m_sound->getStatus();

	return static_cast<AudioStatus>(status);
}

bool Xeph2D::AudioSource::IsStreamed()
{
	if (!m_data)
		return true;
	return m_data->GetIsStreamed();
}

void Xeph2D::AudioSource::SoundSetup()
{
	m_sound = std::make_unique<sf::Sound>();
	m_sound->setBuffer(*m_data->GetBuffer());
}