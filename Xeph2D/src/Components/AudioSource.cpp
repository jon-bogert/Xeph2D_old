#include "Components/AudioSource.h"
#include "Systems/AssetManager.h"
#include "Systems/Serializer.h"

using namespace Xeph2D;
#define __CALL(func) if  (_data) { if (IsStreamed()) { _data->GetStream()->func(); return; } _sound->func(); }
#define __CALL_GET(func) if  (_data) { if (IsStreamed()) { return _data->GetStream()->func(); } return _sound->func(); }
#define __CALL_SET(func, val) if  (_data) { if (IsStreamed()) { _data->GetStream()->func(val); return; } _sound->func(val); }

void AudioSource::Serializables()
{
	SERIALIZE_DEFAULT;
	SERIALIZE_STRING(_dataKey);
}

void Xeph2D::AudioSource::EditorInit()
{
	Awake();
}

void Xeph2D::AudioSource::Awake()
{
	LoadAudioByTag(_dataKey);
}

void Xeph2D::AudioSource::LoadAudioByTag(const std::string& tag)
{
	AudioData* data = AssetManager::GetAudioData(tag);
	_data = data;
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
		status = _data->GetStream()->getStatus();
	else
		status = _sound->getStatus();

	return static_cast<AudioStatus>(status);
}

bool Xeph2D::AudioSource::IsStreamed()
{
	if (!_data)
		return true;
	return _data->GetIsStreamed();
}

void Xeph2D::AudioSource::SoundSetup()
{
	_sound = std::make_unique<sf::Sound>();
	_sound->setBuffer(*_data->GetBuffer());
}