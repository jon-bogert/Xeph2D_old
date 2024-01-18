#include "Scene.h"

#define __CALLONALL(func) for (auto& go : m_gameObjects) { go->func(); }

using namespace Xeph2D;

GameObject* Xeph2D::Scene::AddGameObject(GameObject* parent)
{
	GameObject* obj = m_gameObjects.emplace_back(std::make_unique<GameObject>()).get();
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

void Xeph2D::Scene::Destroy(GameObject* obj)
{
	//TODO - Destroy Children
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		if ((*it).get() == obj)
		{
#ifdef _EDITOR
			(*it)->EditorShutdown();
#else
			(*it)->OnDisable();
			(*it)->OnDestroy();
#endif //_EDITOR
			m_gameObjects.erase(it);
			return;
		}
	}
}

std::vector<GameObject*> Xeph2D::Scene::GetAllObjects()
{
	std::vector<GameObject*> objs;

	for (auto& ptr : m_gameObjects)
		objs.push_back(ptr.get());

	return objs;
}

#ifdef _EDITOR
void Xeph2D::Scene::MoveUp(size_t index)
{
	std::swap(m_gameObjects[index], m_gameObjects[index - 1]);
}

void Xeph2D::Scene::MoveDown(size_t index)
{
	std::swap(m_gameObjects[index], m_gameObjects[index + 1]);
}
#endif //_EDITOR
