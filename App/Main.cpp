#include <cstdint>
#include <iostream>
#include <SFML.hpp>
#include <Windows.h>
#include "Xeph2D.h"
#include "Assets/Scripts/.generated/SceneManifest.generated.h"

using namespace Xeph2D;

int main()
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	AssetManager::LoadTextureFromFile("test.png");
	AssetManager::LoadAudioDataFromFile("gun-shot.wav", false);
	AssetManager::LoadAudioDataFromFile("music.ogg", false);
	WindowManager::Initialize(1280, 720);
	InputSystem::Initialize(WindowManager::GetHandle());
	SceneManager::Initialize(&Xeph2D::LoadSceneManifest);
	SceneManager::Startup();
	while (WindowManager::IsOpen())
	{
		InputSystem::Update();
		if (WindowManager::CheckCloseEvent() || InputSystem::GetKeyDown(Key::Esc))
			WindowManager::Close();
		Time::Update();
		Debug::Update();
		SceneManager::EarlyUpdate();
		SceneManager::Update();
		SceneManager::LateUpdate();
		RenderStack::Execute();
		SceneManager::HandleSceneChange();
	}
	SceneManager::Shutdown();
	return 0;
}