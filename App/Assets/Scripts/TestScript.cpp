#include "TestScript.h"
#include <windows.h>

using namespace Xeph2D;

void TestScript::Start()
{
	renderer = gameObject->GetComponent<SpriteRenderer>();
	renderer->LoadTextureByTag("test");

	audioSource = gameObject->GetComponent<AudioSource>();
	audioSource->LoadAudioByTag("gun-shot");

	InputAction* move;
	InputAction* onSpace;
	InputActionMap* map = InputSystem::FindInputActionMap("Player");
	if (!map)
	{
		map = InputSystem::CreateInputActionMap("Player");

		move = map->CreateAction("Move", InputAction::Type::Axis2D);
		move->Add2DAxis(Gamepad::Axis::LS);
		move->Add2DAxis(Key::A, Key::D, Key::S, Key::W);

		onSpace = map->CreateAction("Space");
		onSpace->AddButton(Key::Space);
	}
	else
	{
		move = map->FindInputAction("Move");
		onSpace = map->FindInputAction("Space");
	}
	
	move->performed.Subscribe(XEInputActionCallback(TestScript::MoveInput));
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

	if (InputSystem::GetKeyDown(Key::E))
		SceneManager::LoadScene(1);
	else if (InputSystem::GetKeyDown(Key::Q))
		SceneManager::LoadScene(0);
}

void Xeph2D::TestScript::OnDestroy()
{
	InputActionMap* map = InputSystem::FindInputActionMap("Player");
	map->FindInputAction("Move")->performed.UnsubscribeAll(this);
	map->FindInputAction("Space")->performed.UnsubscribeAll(this);
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
