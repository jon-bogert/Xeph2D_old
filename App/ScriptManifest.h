#pragma once

#include "Assets/Scripts/CameraController.h"
#include "Assets/Scripts/MusicManager.h"
#include "Assets/Scripts/TestScript.h"

#ifdef _EDITOR
#include <unordered_map>
#include <string>
#define __X2D_REGISTER_COMP_NAMES &__RegisterComponentNames
inline std::unordered_map<uint32_t, std::string> __RegisterComponentNames()
{
	std::unordered_map<uint32_t, std::string> names;

	names[1] = "Camera";
	names[2] = "SpriteRenderer";
	names[3] = "AudioSource";

	//--
	names[25] = "TestScript";
	names[26] = "CameraController";
	names[27] = "MusicManager";

	return names;
}
#endif _EDITOR