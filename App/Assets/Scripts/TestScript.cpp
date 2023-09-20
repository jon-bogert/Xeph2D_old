#include "TestScript.h"
#include <windows.h>

using namespace Xeph2D;

void TestScript::Start()
{
	std::cout << "Calling Start" << std::endl;
	renderer = gameObject->GetComponent<SpriteRenderer>();
	renderer->LoadTextureByTag("test");

	InputActionMap* map = InputSystem::CreateInputActionMap("Player");
	InputAction* move = map->CreateAction("Move", InputAction::Type::Axis2D);
	move->Add2DAxis(Gamepad::Axis::LS);
	move->Add2DAxis(Key::A, Key::D, Key::S, Key::W);
	move->performed.Subscribe(XEInputActionCallback(TestScript::MoveInput));
}

void TestScript::Update()
{
	transform->position.x += speed * moveAxis.x * Time::DeltaTime();
	transform->position.y += speed * moveAxis.y * Time::DeltaTime();

	Color c = Color::Red;
	Debug::DrawCircleFilled(transform->position, 0.5f, c);
}

void Xeph2D::TestScript::MoveInput(InputAction* ctx)
{
	ctx->ReadValue(&moveAxis.x);
}
