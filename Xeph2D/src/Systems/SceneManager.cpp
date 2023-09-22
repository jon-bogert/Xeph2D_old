#include "Systems/SceneManager.h"

using namespace Xeph2D;

SceneManager& Xeph2D::SceneManager::Get()
{
    static SceneManager instance;
    return instance;
}
