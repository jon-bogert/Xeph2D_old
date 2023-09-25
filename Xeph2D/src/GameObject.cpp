#include "GameObject.h"

#include "Systems/Serializer.h"
#include "Component.h"

#define __CALLONALL(func) for (auto& c : _components) { c->func(); }

using namespace Xeph2D;

void Xeph2D::GameObject::Serializables()
{
    Serializer::Register(instID, Serializer::DataType::Transform, &transform, "go_transform");
    Serializer::Register(instID, Serializer::DataType::String, &name, "go_name");
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
