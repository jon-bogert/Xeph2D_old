#pragma once
#include <Xeph2D.h>
#include "../../../ScriptManifest.h"

#define __X2D_LOAD_SCENES &Xeph2D::LoadSceneManafest

namespace Xeph2D
{
	void LoadSceneManifest(SceneManager* mngr, int num, bool init)
	{
		if (init)
		{
			mngr->AddScene("Main");
			mngr->AddScene("Second");
		}
		Scene* scn = mngr->NewScene();
		if (num == 0)
		{
			GameObject* _00000000 = scn->AddGameObject();
			_00000000->AddComponent<SpriteRenderer>();
			_00000000->AddComponent<AudioSource>();
			_00000000->AddComponent<TestScript>();

			GameObject* _00000001 = scn->AddGameObject();
			_00000001->AddComponent<Camera>();
			_00000001->AddComponent<CameraController>();

			GameObject* _00000002 = scn->AddGameObject();
			_00000002->AddComponent<AudioSource>();
			_00000002->AddComponent<MusicManager>();
		}
		else if (num == 1)
		{
			GameObject* _0000003 = scn->AddGameObject();
			_0000003->AddComponent<SpriteRenderer>();
			_0000003->AddComponent<AudioSource>();
			_0000003->AddComponent<TestScript>();
			_0000003->transform.position = { 10, 10 };

			GameObject* _00000004 = scn->AddGameObject();
			_00000004->AddComponent<Camera>();
			_00000004->AddComponent<CameraController>();
		}
	}
}