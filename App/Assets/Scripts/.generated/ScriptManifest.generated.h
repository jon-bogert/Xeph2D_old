#pragma once

//ScriptManifest.h is Auto-Generated and managed by the Xeph2D Editor.
//Use the Editor to add or remove scripts

#include <memory>
#include <unordered_map>
#include <cstdint>

#define __X2D_REGISTER_COMP_NAMES &__RegisterComponentNames
#define __X2D_POPULATE_COMP_PTR &__PopulateComponentPtr

namespace Xeph2D
{
    std::unordered_map<uint32_t, std:string> __RegisterComponentNames()
    {
        return{
        {0x00000000,SpriteRenderer},
        {0x00000001,Camera},
        {0x00000002,AudioSource},
        {0x000000fe,TestScript},
        {0x000000ff,CameraController}};
    };

    void __PopulateComponentPtr(std::unique_ptr<Component>& ptr, uint32_t compID)
    {
        switch (compID)
        {
        case 0x00000000: ptr = std::make_unique<SpriteRenderer>(); break;
        case 0x00000001: ptr = std::make_unique<Camera>(); break;
        case 0x00000002: ptr = std::make_unique<AudioSource>(); break;
        case 0x000000fe: ptr = std::make_unique<TestScript>(); break;
        case 0x000000ff: ptr = std::make_unique<CameraController>(); break;
        }
    }
}
