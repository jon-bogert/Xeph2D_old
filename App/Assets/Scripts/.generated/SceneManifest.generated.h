#pragma once
#include <Xeph2D.h>
#include "../../../ScriptManifest.h"

#define __X2D_LOAD_SCENES &Xeph2D::LoadSceneManafest

namespace Xeph2D
{
	void LoadSceneManifest(SceneManager* mngr)
	{
		Scene* scn = mngr->AddScene("Main");

		GameObject* _0000000 = scn->AddGameObject();
		_0000000->AddComponent<SpriteRenderer>();
		_0000000->AddComponent<AudioSource>();
		_0000000->AddComponent<TestScript>();

		GameObject* _00000001 = scn->AddGameObject();
		_00000001->AddComponent<Camera>();
		_00000001->AddComponent<CameraController>();

		GameObject* _00000002 = scn->AddGameObject();
		_00000002->AddComponent<AudioSource>();
		_00000002->AddComponent<MusicManager>();
	}
}