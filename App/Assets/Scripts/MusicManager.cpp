#include "MusicManager.h"

using namespace Xeph2D;

void MusicManager::Serializables()
{
	SERIALIZE_DEFAULT;
}

void MusicManager::Start()
{
	audio = gameObject->GetComponent<AudioSource>();
	audio->LoadAudioByTag("music");
	//audio->Play();
}
