#include "GameObject.h"

#include "Systems/Serializer.h"
#include "Component.h"

#ifdef _EDITOR
#include "Editor/Editor.h"
#endif //_EDITOR

#define __CALLONALL(func) for (auto& c : _components) { c->func(); }

using namespace Xeph2D;

void Xeph2D::GameObject::RemoveComponent(Component* comp)
{
    for (auto it = _components.begin(); it != _components.end(); it++)
    {
        if (it->get() == comp)
        {
#ifdef _EDITOR
            comp->EditorShutdown();
#else
            comp->OnDisable();
            comp->OnDestroy();
#endif //_EDITOR
            _components.erase(it);
            return;
        }
    }
}

void Xeph2D::GameObject::RemoveComponent(uint32_t compID)
{
    for (auto it = _components.begin(); it != _components.end(); it++)
    {
        if ((*it)->GetTypeID() == compID)
        {
#ifdef _EDITOR
            (*it)->EditorShutdown();
#else
            (*it)->OnDisable();
            (*it)->OnDestroy();
#endif //_EDITOR
            _components.erase(it);
            return;
        }
    }
}

#ifdef _EDITOR
void Xeph2D::GameObject::MoveUp(size_t index)
{
    std::swap(_components[index], _components[index - 1]);
}

void Xeph2D::GameObject::MoveDown(size_t index)
{
    std::swap(_components[index], _components[index + 1]);
}
#endif //_EDITOR

std::unique_ptr<Component>& Xeph2D::GameObject::__GetNewEmptyComponentPtr()
{
    return _components.emplace_back(nullptr);
}

void Xeph2D::GameObject::Serializables()
{
    Serializer::Register(instID, Serializer::DataType::String, &name, "go_name");
    Serializer::Register(instID, Serializer::DataType::Transform, &transform, "go_transform");
    __CALLONALL(Serializables)
}

void GameObject::EditorInit()       { __CALLONALL(EditorInit) }
void GameObject::EditorShutdown()   { __CALLONALL(EditorShutdown) }

void GameObject::Awake()		    { __CALLONALL(Awake) }
void GameObject::Start()		    { __CALLONALL(Start) }
void GameObject::OnEnable()		    { __CALLONALL(OnEnable) }
void GameObject::EarlyUpdate()	    { __CALLONALL(EarlyUpdate) }
void GameObject::Update()		    { __CALLONALL(Update) }
void GameObject::LateUpdate()	    { __CALLONALL(LateUpdate) }
void GameObject::DebugDraw()        { __CALLONALL(DebugDraw) }
void GameObject::OnDisable()	    { __CALLONALL(OnDisable) }
void GameObject::OnDestroy()	    { __CALLONALL(OnDestroy) }

GameObject* Xeph2D::GameObject::GetParent() const
{
    return _parent;
}

void Xeph2D::GameObject::SetParent(GameObject* parent)
{
    _parent = parent;
    // TODO - Re-jig Scene
}
