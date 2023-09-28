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
    Get()._scriptCallback = scriptCallback;
    Get()._currIndex = 0;
    AddScene("Main"); // TODO - Find some way to add this from settings file;
    //Get()._scriptCallback(&Get(), Get()._currIndex, true);
    Get().DoSceneLoading();
    Serializer::LoadFromFile(GetCurrentName());
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

std::string Xeph2D::SceneManager::GetCurrentName()
{
    return Get()._scenes[Get()._currIndex];
}

void Xeph2D::SceneManager::EditorInit()
{
    Get()._currScene->EditorInit();
}

void Xeph2D::SceneManager::EditorShutdown()
{
    Get()._currScene->EditorShutdown();
}

void Xeph2D::SceneManager::Serializables()
{
    Get()._currScene->Serializables();
}

void Xeph2D::SceneManager::HandleSceneChange()
{
    if (!Get()._doLoadScene)
        return;

    Get()._doLoadScene = false;

    Get()._currScene->OnDisable();
    Get()._currScene->OnDestroy();

    Debug::ClearMonitorBuffer();
    //Get()._scriptCallback(&Get(), Get()._nextIndex, false);
    Get().DoSceneLoading();
    Get()._currIndex = Get()._nextIndex;

    Get()._currScene->Awake();
    Get()._currScene->Start();
    Get()._currScene->OnEnable();
}

void Xeph2D::SceneManager::Startup()
{
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
            gameObject = _currScene->AddGameObject();
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
        _scriptCallback(compPtr, type);
        compPtr->Register(gameObject);
        compAdded.insert(type);
    }
    file.close();
}

void Xeph2D::SceneManager::__AddComponentByID(GameObject* obj, uint32_t id)
{
    std::unique_ptr<Component>& compPtr = obj->__GetNewEmptyComponentPtr();
    _scriptCallback(compPtr, id);
    compPtr->Register(obj);
    compPtr->Serializables();
    compPtr->EditorInit();
}
