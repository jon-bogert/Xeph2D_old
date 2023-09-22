#include "MusicManager.h"

using namespace Xeph2D;

void MusicManager::Start()
{
	audio = gameObject->GetComponent<AudioSource>();
	audio->LoadAudioByTag("music");
	//audio->Play();
}
