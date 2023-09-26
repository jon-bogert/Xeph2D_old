#include "TestScript.h"

using namespace Xeph2D;

void TestScript::Serializables()
{
	SERIALIZE_DEFAULT;
	SERIALIZE_BOOL(_showDebug);
}

void TestScript::Start()
{	
	audioSource = gameObject->GetComponent<AudioSource>();

	InputActionMap* map = InputSystem::FindInputActionMap("Player");
	InputAction* move = map->FindInputAction("Move");
	InputAction* onSpace = map->FindInputAction("Space");
	
	move->performed.Subscribe(XEInputActionCallback(TestScript::MoveInput));
	onSpace->performed.Subscribe(XEInputActionCallback(TestScript::OnSpace));
}

void TestScript::Update()
{
	transform->position.x += speed * moveAxis.x * Time::DeltaTime();
	transform->position.y += speed * moveAxis.y * Time::DeltaTime();

	Debug::Monitor("Player Position", "X: " + std::to_string(transform->position.x) + " Y: " + std::to_string(transform->position.y));

	if (InputSystem::GetKeyDown(Key::E))
		SceneManager::LoadScene(1);
	else if (InputSystem::GetKeyDown(Key::Q))
		SceneManager::LoadScene(0);
}

void Xeph2D::TestScript::DebugDraw()
{
	if (!_showDebug)
		return;
	Color c = Color::Red;
	c.a = .5f;
	Debug::DrawCircleFilled(transform->position, 0.5f, c);
	Debug::DrawBoxOutline(transform->position, { 1, 1 }, Color::Green);
	Debug::DrawLine(transform->position, Vector2::Zero(), Color::Green);
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
	//Debug::LogColor(Color::Magenta);
	//Debug::Log("axis X: %.2f", moveAxis.x);
	//Debug::Log("axis Y: %.2f", moveAxis.y);
}

void Xeph2D::TestScript::OnSpace(InputAction* ctx)
{
	Debug::Log("Space Pressed");
	//audioSource->Play();
}
