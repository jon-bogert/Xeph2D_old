#include "Systems/SceneManager.h"
#include "Systems/WindowManager.h"
#include "Systems/Debug.h"
#include "Systems/Serializer.h"

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
    Get().DoSceneLoading();
    Serializer::LoadFromFile(GetCurrentName());
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
    std::string scene = GetCurrentName();

    std::ifstream file("Assets/Scenes/" + scene + ".x2dsc");
    if (!file.is_open())
    {
        Debug::LogWarn("Serializer::LoadFromFile -> No file to load: %s", (scene + ".x2dsc").c_str());
        return;
    }

    std::unordered_set<uint32_t> compAdded;
    GameObject* gameObject = nullptr;
    NewScene();

    std::string line;
    while (std::getline(file, line))
    {
        if (line.substr(0, 5) == "inst=")
        {
            std::stringstream id;
            uint32_t inst;
            id << std::hex << line.substr(5);
            id >> inst;
            gameObject = m_currScene->AddGameObject();
            gameObject->instID = inst;
            compAdded.clear();
            continue;
        }
        if (line[0] != '\t' && line.substr(0, 4) != "    ")
        {
            Debug::LogErr("Serializer::LoadFromFile -> Bad Formatting: %s", line.c_str());
            continue;
        }
        std::stringstream linestream((line[0] == '\t') ? line.substr(1) : line.substr(4));
        std::string key;
        std::getline(linestream, key, ' ');
        std::getline(linestream, key, '=');
        if (key.substr(0, 2) == "go")
            continue;
        std::stringstream id;
        uint32_t type;
        id << std::hex << key.substr(0, 8);
        id >> type;
        if (compAdded.find(type) != compAdded.end())
            continue;
        std::unique_ptr<Component>& compPtr = gameObject->__GetNewEmptyComponentPtr();
        m_scriptCallback(compPtr, type);
        compPtr->Register(gameObject);
        compAdded.insert(type);
    }
    file.close();
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
