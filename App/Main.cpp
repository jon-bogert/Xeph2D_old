#include <SFML.hpp>
#include <Windows.h>

#include "Xeph2D.h"
#include "Assets/Scripts/.generated/SceneManifest.generated.h"
#include "Assets/Scripts/.generated/InputActions.generated.h"

using namespace Xeph2D;

#ifdef _CONSOLE
int main()
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif // _CONSOLE
{
	//---- MOVE TO SCENE LOAD
	AssetManager::LoadTextureFromFile("test.png");
	AssetManager::LoadAudioDataFromFile("gun-shot.wav", false);
	AssetManager::LoadAudioDataFromFile("music.ogg", false);
	//----
	WindowManager::Initialize(1280, 720);
	InputSystem::Initialize(WindowManager::GetHandle(), __X2D_LOAD_INPUTACTIONS);
	SceneManager::Initialize(__X2D_LOAD_SCENES);
	SceneManager::Startup();
	while (WindowManager::IsOpen())
	{
		InputSystem::Update();
		WindowManager::CheckWindowEvents();
		Time::Update();
		Debug::Update();
		SceneManager::Update();
		RenderStack::Draw();
		SceneManager::HandleSceneChange();
	}
	SceneManager::Shutdown();
	return 0;
}