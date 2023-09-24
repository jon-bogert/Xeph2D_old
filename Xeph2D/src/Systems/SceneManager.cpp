#include "Systems/SceneManager.h"
#include "Systems/WindowManager.h"
#include "Systems/Debug.h"

using namespace Xeph2D;

SceneManager& SceneManager::Get()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::Initialize(std::function<void(SceneManager*, int, bool)> loadCallback)
{
    Get()._loadCallback = loadCallback;
}

Scene* Xeph2D::SceneManager::NewScene()
{
    Get()._currScene = std::make_unique<Scene>();
    return Get()._currScene.get();
}

void SceneManager::AddScene(const std::string& name)
{
    Get()._scenes.emplace_back(name);
}

void SceneManager::LoadScene(const std::string& name)
{
    LoadScene(GetSceneIndex(name));
}

void SceneManager::LoadScene(const int index)
{
    Get()._nextIndex = index;
    Get()._doLoadScene = true;
}

int SceneManager::GetSceneIndex(const std::string& name)
{
    for (int i = 0; i < Get()._scenes.size(); ++i)
    {
        if (Get()._scenes[i] == name)
            return i;
    }
    return -1;
}

Scene* Xeph2D::SceneManager::GetCurrentScene()
{
    return Get()._currScene.get();
}

int Xeph2D::SceneManager::GetCurrentIndex()
{
    return Get()._currIndex;
}

void Xeph2D::SceneManager::EditorInit()
{
    Get()._currIndex = 0;
    Get()._loadCallback(&Get(), Get()._currIndex, true);
    Get()._currScene->EditorInit();
}

void Xeph2D::SceneManager::EditorShutdown()
{
    Get()._currScene->EditorShutdown();
}

void Xeph2D::SceneManager::HandleSceneChange()
{
    if (!Get()._doLoadScene)
        return;

    Get()._doLoadScene = false;

    Get()._currScene->OnDisable();
    Get()._currScene->OnDestroy();

    Debug::ClearMonitorBuffer();
    Get()._loadCallback(&Get(), Get()._nextIndex, false);
    Get()._currIndex = Get()._nextIndex;

    Get()._currScene->Awake();
    Get()._currScene->Start();
    Get()._currScene->OnEnable();
}

void Xeph2D::SceneManager::Startup()
{
    Get()._currIndex = 0;
    Get()._loadCallback(&Get(), Get()._currIndex, true);
    if (Get()._scenes.size() <= 0)
    {
        WindowManager::Close();
        return;
    }

    Get()._currScene->Awake();
    Get()._currScene->Start();
    Get()._currScene->OnEnable();
}

void Xeph2D::SceneManager::Update()
{
    Get()._currScene->EarlyUpdate();
    Get()._currScene->Update();
    Get()._currScene->LateUpdate();
}

void Xeph2D::SceneManager::DebugDraw()
{
    Get()._currScene->DebugDraw();
}

void Xeph2D::SceneManager::Shutdown()
{
    if (Get()._currIndex < 0)
        return;

    Get()._currScene->OnDisable();
    Get()._currScene->OnDestroy();
}
