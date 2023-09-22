#include "Systems/SceneManager.h"
#include "Systems/WindowManager.h"

using namespace Xeph2D;

#define __CURRSCENE Get()._scenes[Get()._currIndex]

SceneManager& SceneManager::Get()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::Initialize(std::function<void(SceneManager*)> loadProcess)
{
    loadProcess(&Get());
}

Scene* SceneManager::AddScene(const std::string& name)
{
    Scene* scene = Get()._scenes.emplace_back(std::make_unique<Scene>()).get();
    if (!name.empty())
        scene->name = name;
    return scene;
}

void SceneManager::LoadScene(const std::string& name)
{
    LoadScene(GetSceneIndex(name));
}

void SceneManager::LoadScene(const int index)
{
}

int SceneManager::GetSceneIndex(const std::string& name)
{
    for (int i = 0; i < Get()._scenes.size(); ++i)
    {
        if (Get()._scenes[i]->name == name)
            return i;
    }
    return -1;
}

Scene* SceneManager::GetScene(const std::string& name)
{
    return GetScene(GetSceneIndex(name));
}

Scene* SceneManager::GetScene(const int index)
{
    return Get()._scenes[index].get();
}

Scene* Xeph2D::SceneManager::GetCurrentScene()
{
    if (Get()._currIndex < 0)
        return nullptr;
    return Get()._scenes[Get()._currIndex].get();
}

int Xeph2D::SceneManager::GetCurrentIndex()
{
    return Get()._currIndex;
}

void Xeph2D::SceneManager::HandleSceneChange()
{
}

void Xeph2D::SceneManager::Startup()
{
    if (Get()._scenes.size() <= 0)
    {
        WindowManager::Close();
        return;
    }
    Get()._currIndex = 0;

    __CURRSCENE->Awake();
    __CURRSCENE->Start();
    __CURRSCENE->OnEnable();
}

void Xeph2D::SceneManager::EarlyUpdate()    { __CURRSCENE->EarlyUpdate(); }
void Xeph2D::SceneManager::Update()         { __CURRSCENE->Update(); }
void Xeph2D::SceneManager::LateUpdate()     { __CURRSCENE->LateUpdate(); }

void Xeph2D::SceneManager::Shutdown()
{
    __CURRSCENE->OnDisable();
    __CURRSCENE->OnDestroy();
}
