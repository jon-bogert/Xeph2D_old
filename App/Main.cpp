#include <cstdint>
#include <iostream>

#include "Xeph2D.h"
#include <SFML.hpp>

#include "Assets/Scripts/TestScript.h"
#include "Assets/Scripts/CameraController.h"
#include "Assets/Scripts/MusicManager.h"

#include <Windows.h>

using namespace Xeph2D;

int main()
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	AssetManager::LoadTextureFromFile("test.png");
	AssetManager::LoadAudioDataFromFile("gun-shot.wav", false);
	AssetManager::LoadAudioDataFromFile("music.ogg", false);
	WindowManager::Initialize(1280, 720);
	InputSystem::Initialize(WindowManager::GetHandle());

	GameObject camera;
	camera.AddComponent<Camera>();
	camera.AddComponent<CameraController>();

	GameObject musicManager;
	musicManager.AddComponent<AudioSource>();
	musicManager.AddComponent<MusicManager>();

	GameObject go;
	SpriteRenderer* renderer = go.AddComponent<SpriteRenderer>();
	go.AddComponent<AudioSource>();
	TestScript* ptr = go.AddComponent<TestScript>();

	camera.Awake();
	go.Awake();
	go.Start();
	camera.Start();
	musicManager.Start();

	while (WindowManager::IsOpen())
	{
		InputSystem::Update();
		if (WindowManager::CheckCloseEvent() || InputSystem::GetKeyDown(Key::Esc))
			WindowManager::Close();

		Time::Update();
		Debug::Update();
		go.Update();
		camera.Update();

		RenderStack::Execute();
	}

	camera.OnDestroy();

	return 0;
}