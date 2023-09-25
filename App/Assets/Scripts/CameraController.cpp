#include "CameraController.h"

using namespace Xeph2D;

void CameraController::Serializables()
{
	SERIALIZE_DEFAULT;
}

void CameraController::Start()
{
	InputActionMap* map = InputSystem::FindInputActionMap("Player");
	InputAction* action = map->FindInputAction("Camera");
	action->performed.Subscribe(XEInputActionCallback(CameraController::MoveInput));
}

void CameraController::Update()
{
	transform->position += moveAxis * _speed * Time::DeltaTime();
}

void Xeph2D::CameraController::OnDestroy()
{
	InputSystem::FindInputActionMap("Player")->FindInputAction("Camera")->performed.UnsubscribeAll(this);
}

void Xeph2D::CameraController::MoveInput(InputAction* ctx)
{
	ctx->ReadValue(&moveAxis.x);
}
