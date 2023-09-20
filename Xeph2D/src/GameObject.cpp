#include "GameObject.h"

#include "Component.h"

#define __CALLONALL(func) for (auto& c : _components) { c->func(); }

using namespace Xeph2D;

void GameObject::Awake()		{ __CALLONALL(Awake) }
void GameObject::Start()		{ __CALLONALL(Start) }
void GameObject::OnEnable()		{ __CALLONALL(OnEnable) }
void GameObject::EarlyUpdate()	{ __CALLONALL(EarlyUpdate) }
void GameObject::Update()		{ __CALLONALL(Update) }
void GameObject::LateUpdate()	{ __CALLONALL(LateUpdate) }
void GameObject::OnDisable()	{ __CALLONALL(OnDisable) }
void GameObject::OnDestroy()	{ __CALLONALL(OnDestroy) }