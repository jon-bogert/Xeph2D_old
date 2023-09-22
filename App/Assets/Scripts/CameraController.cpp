#include "CameraController.h"

using namespace Xeph2D;

void CameraController::Start()
{
	InputActionMap* map = InputSystem::FindInputActionMap("Player");
	InputAction* action = map->FindInputAction("Camera");
	if (!action)
	{
		action = map->CreateAction("Camera", InputAction::Type::Axis2D);
		action->Add2DAxis(Key::Left, Key::Right, Key::Down, Key::Up);
	}
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
