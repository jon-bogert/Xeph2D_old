#include "TestScript.h"
#include <windows.h>

using namespace Xeph2D;

void TestScript::Start()
{
	renderer = gameObject->GetComponent<SpriteRenderer>();
	renderer->LoadTextureByTag("test");

	audioSource = gameObject->GetComponent<AudioSource>();
	audioSource->LoadAudioByTag("gun-shot");

	InputActionMap* map = InputSystem::CreateInputActionMap("Player");
	InputAction* move = map->CreateAction("Move", InputAction::Type::Axis2D);
	move->Add2DAxis(Gamepad::Axis::LS);
	move->Add2DAxis(Key::A, Key::D, Key::S, Key::W);
	move->performed.Subscribe(XEInputActionCallback(TestScript::MoveInput));

	InputAction* onSpace = map->CreateAction("Space");
	onSpace->AddButton(Key::Space);
	onSpace->performed.Subscribe(XEInputActionCallback(TestScript::OnSpace));
}

void TestScript::Update()
{
	transform->position.x += speed * moveAxis.x * Time::DeltaTime();
	transform->position.y += speed * moveAxis.y * Time::DeltaTime();

	Color c = Color::Red;
	c.a = .5f;
	Debug::DrawCircleFilled(transform->position, 0.5f, c);
	Debug::DrawBoxOutline(transform->position, {1, 1}, Color::Green);
	Debug::DrawLine(transform->position, Vector2::Zero(), Color::Green);
	Debug::Monitor("Player Position", "X: " + std::to_string(transform->position.x) + " Y: " + std::to_string(transform->position.y));
}

void Xeph2D::TestScript::MoveInput(InputAction* ctx)
{
	ctx->ReadValue(&moveAxis.x);
	Debug::LogColor(Color::Magenta);
	Debug::Log("axis X: %.2f", moveAxis.x);
	Debug::Log("axis Y: %.2f", moveAxis.y);
}

void Xeph2D::TestScript::OnSpace(InputAction* ctx)
{
	audioSource->Play();
}
