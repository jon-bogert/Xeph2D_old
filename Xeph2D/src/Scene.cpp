#include "Scene.h"

#define __CALLONALL(func) for (auto& go : _gameObjects) { go->func(); }

using namespace Xeph2D;

GameObject* Xeph2D::Scene::AddGameObject(GameObject* parent)
{
	GameObject* obj = _gameObjects.emplace_back(std::make_unique<GameObject>()).get();
	if (parent)
	{
		obj->SetParent(parent);
	}
	return obj;
}

void Scene::Serializables()		{ __CALLONALL(Serializables) }

void Scene::EditorInit()		{ __CALLONALL(EditorInit) }
void Scene::EditorShutdown()	{ __CALLONALL(EditorShutdown) }

void Scene::Awake()				{ __CALLONALL(Awake) }
void Scene::Start()				{ __CALLONALL(Start) }
void Scene::OnEnable()			{ __CALLONALL(OnEnable) }
void Scene::EarlyUpdate()		{ __CALLONALL(EarlyUpdate) }
void Scene::Update()			{ __CALLONALL(Update) }
void Scene::LateUpdate()		{ __CALLONALL(LateUpdate) }
void Scene::DebugDraw()			{ __CALLONALL(DebugDraw) }
void Scene::OnDisable()			{ __CALLONALL(OnDisable) }
void Scene::OnDestroy()			{ __CALLONALL(OnDestroy) }

std::vector<GameObject*> Xeph2D::Scene::GetAllObjects()
{
	std::vector<GameObject*> objs;

	for (auto& ptr : _gameObjects)
		objs.push_back(ptr.get());

	return objs;
}
