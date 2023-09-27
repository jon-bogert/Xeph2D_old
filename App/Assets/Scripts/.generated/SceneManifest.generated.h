#pragma once
#include <Xeph2D.h>
#include "../../../ScriptManifest.h"

#define __X2D_LOAD_SCENES &Xeph2D::Generated::LoadSceneManifest

namespace Xeph2D::Generated
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
			_00000000->instID = 0x00000000;
			_00000000->AddComponent<SpriteRenderer>();
			_00000000->AddComponent<AudioSource>();
			_00000000->AddComponent<TestScript>();
			_00000000->AddComponent<Rigidbody>();
			_00000000->AddComponent<BoxCollider>();
			GameObject* _00000001 = scn->AddGameObject();
			_00000001->instID = 0x00000001;
			_00000001->AddComponent<Camera>();
			_00000001->AddComponent<CameraController>();
			GameObject* _00000002 = scn->AddGameObject();
			_00000002->instID = 0x00000002;
			_00000002->AddComponent<AudioSource>();
			_00000002->AddComponent<MusicManager>();
		}
		else if (num == 1)
		{
			GameObject* _0000003 = scn->AddGameObject();
			_0000003->instID = 0x00000003;
			_0000003->AddComponent<SpriteRenderer>();
			_0000003->AddComponent<AudioSource>();
			_0000003->AddComponent<TestScript>();
			_0000003->transform.position = { 10, 10 };
			GameObject* _00000004 = scn->AddGameObject();
			_00000004->instID = 0x00000004;
			_00000004->AddComponent<Camera>();
			_00000004->AddComponent<CameraController>();
		}
	}
}