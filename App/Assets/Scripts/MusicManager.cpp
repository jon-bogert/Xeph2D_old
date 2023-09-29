#include "MusicManager.h"

using namespace Xeph2D;

void MusicManager::Serializables()
{
	SERIALIZE_DEFAULT;
}

void MusicManager::Start()
{
	audio = gameObject->GetComponent<AudioSource>();
	Debug::LogColor(Color::Purple);
	Debug::Log("MusicManager.Start");
	//audio->Play();
}
