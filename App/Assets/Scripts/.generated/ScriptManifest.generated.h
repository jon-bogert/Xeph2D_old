#pragma once

//ScriptManifest.h is Auto-Generated and managed by the Xeph2D Editor.
//Use the Editor to add or remove scripts

#include <Xeph2D.h>

#include <memory>
#include <unordered_map>
#include <cstdint>

#include "TestScript.h"
#include "CameraController.h"
#include "MusicManager.h"

#define __X2D_REGISTER_COMP_NAMES &__RegisterComponentNames
#define __X2D_POPULATE_COMP_PTR &__PopulateComponentPtr

namespace Xeph2D
{
    std::unordered_map<uint32_t, std::string> __RegisterComponentNames()
    {
        return{
        {0x00000001,"Camera"},
        {0x00000002,"SpriteRenderer"},
        {0x00000003,"AudioSource"},
        {0x00000019,"TestScript"},
        {0x0000001a,"CameraController"},
        {0x0000001b,"MusicManager"}};
    };

    void __PopulateComponentPtr(std::unique_ptr<Component>& ptr, uint32_t compID)
    {
        switch (compID)
        {
        case 0x00000001: ptr = std::make_unique<Camera>(); break;
        case 0x00000002: ptr = std::make_unique<SpriteRenderer>(); break;
        case 0x00000003: ptr = std::make_unique<AudioSource>(); break;
        case 0x00000019: ptr = std::make_unique<TestScript>(); break;
        case 0x0000001a: ptr = std::make_unique<CameraController>(); break;
        case 0x0000001b: ptr = std::make_unique<MusicManager>(); break;
        }
    }
}
