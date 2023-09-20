#include <cstdint>
#include <iostream>

#include "Xeph2D.h"
#include <SFML.hpp>

#include "Assets/Scripts/TestScript.h"

#include <Windows.h>

using namespace Xeph2D;

int main()
{
	AssetManager::LoadTextureFromFile("test.png");
	WindowManager::Initialize(1280, 720);
	InputSystem::Initialize(WindowManager::GetHandle());

	GameObject camera;
	camera.AddComponent<Camera>();

	GameObject go;
	SpriteRenderer* renderer = go.AddComponent<SpriteRenderer>();
	TestScript* ptr = go.AddComponent<TestScript>();

	camera.Awake();
	go.Awake();
	go.Start();

	while (WindowManager::IsOpen())
	{
		InputSystem::Update();
		if (WindowManager::CheckCloseEvent() || InputSystem::GetKeyDown(Key::Esc))
			WindowManager::Close();

		Time::Update();
		go.Update();

		RenderStack::Execute();
	}

	camera.OnDestroy();

	return 0;
}