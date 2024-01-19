#include "Systems/SceneManager.h"
#include "Systems/WindowManager.h"
#include "Systems/Debug.h"
#include "Systems/Serializer.h"
#include "Utility.h"

#include <unordered_set>
#include <string>
#include <fstream>
#include <sstream>

using namespace Xeph2D;

SceneManager& SceneManager::Get()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::Initialize(std::function<void(std::unique_ptr<Component>& ptr, uint32_t compID)> scriptCallback)
{
    Get().m_scriptCallback = scriptCallback;
    Get().m_currIndex = 0;
    AddScene("Main"); // TODO - Find some way to add this from settings file;
    //Get()._scriptCallback(&Get(), Get()._currIndex, true);
    Serializer::LoadFromFile(GetCurrentName());
    Get().DoSceneLoading();
}

Scene* Xeph2D::SceneManager::NewScene()
{
    Get().m_currScene = std::make_unique<Scene>();
    return Get().m_currScene.get();
}

void SceneManager::AddScene(const std::string& name)
{
    Get().m_scenes.emplace_back(name);
}

void SceneManager::LoadScene(const std::string& name)
{
    LoadScene(GetSceneIndex(name));
}

void SceneManager::LoadScene(const int index)
{
    Get().m_nextIndex = index;
    Get().m_doLoadScene = true;
}

int SceneManager::GetSceneIndex(const std::string& name)
{
    for (int i = 0; i < Get().m_scenes.size(); ++i)
    {
        if (Get().m_scenes[i] == name)
            return i;
    }
    return -1;
}

Scene* Xeph2D::SceneManager::GetCurrentScene()
{
    return Get().m_currScene.get();
}

int Xeph2D::SceneManager::GetCurrentIndex()
{
    return Get().m_currIndex;
}

std::string Xeph2D::SceneManager::GetCurrentName()
{
    return Get().m_scenes[Get().m_currIndex];
}

void Xeph2D::SceneManager::EditorInit()
{
    Get().m_currScene->EditorInit();
}

void Xeph2D::SceneManager::EditorShutdown()
{
    Get().m_currScene->EditorShutdown();
}

void Xeph2D::SceneManager::Serializables()
{
    Get().m_currScene->Serializables();
}

void Xeph2D::SceneManager::HandleSceneChange()
{
    if (!Get().m_doLoadScene)
        return;

    Get().m_doLoadScene = false;

    Get().m_currScene->OnDisable();
    Get().m_currScene->OnDestroy();

    Debug::ClearMonitorBuffer();
    //Get()._scriptCallback(&Get(), Get()._nextIndex, false);
    Get().DoSceneLoading();
    Get().m_currIndex = Get().m_nextIndex;

    Get().m_currScene->Awake();
    Get().m_currScene->Start();
    Get().m_currScene->OnEnable();
}

void Xeph2D::SceneManager::Startup()
{
    if (Get().m_scenes.size() <= 0)
    {
        WindowManager::Close();
        return;
    }

    Get().m_currScene->Awake();
    Get().m_currScene->Start();
    Get().m_currScene->OnEnable();
}

void Xeph2D::SceneManager::Update()
{
    Get().m_currScene->EarlyUpdate();
    Get().m_currScene->Update();
    Get().m_currScene->LateUpdate();
}

void Xeph2D::SceneManager::DebugDraw()
{
    Get().m_currScene->DebugDraw();
}

void Xeph2D::SceneManager::Shutdown()
{
    if (Get().m_currIndex < 0)
        return;

    Get().m_currScene->OnDisable();
    Get().m_currScene->OnDestroy();
}

void Xeph2D::SceneManager::DoSceneLoading()
{

    std::vector<Serializer::IDInfo> sceneObjects = Serializer::GetIDInfo();
    NewScene();
    for (Serializer::IDInfo objInfo : sceneObjects)
    {
        GameObject* gameObject = m_currScene->AddGameObject();
        gameObject->instID = objInfo.gameObject;
        for (uint32_t compID : objInfo.components)
        {
            std::unique_ptr<Component>& compPtr = gameObject->__GetNewEmptyComponentPtr();
            m_scriptCallback(compPtr, compID);
            compPtr->Register(gameObject);
        }
    }
}

#ifdef _EDITOR
void Xeph2D::SceneManager::__AddComponentByID(GameObject* obj, uint32_t id)
{
    std::unique_ptr<Component>& compPtr = obj->__GetNewEmptyComponentPtr();
    m_scriptCallback(compPtr, id);
    compPtr->Register(obj);
    compPtr->Serializables();
    compPtr->EditorInit();
}
#endif // _EDITOR
