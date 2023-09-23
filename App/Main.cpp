#include <SFML.hpp>
#include <Windows.h>

#include "Xeph2D.h"
#ifdef _EDITOR
#include "Editor/Editor.h"
#endif //_EDITOR
#include "Assets/Scripts/.generated/SceneManifest.generated.h"
#include "Assets/Scripts/.generated/InputActions.generated.h"

using namespace Xeph2D;
#ifdef _EDITOR
using namespace Xeph2D::Edit;
#endif //_!EDITOR

#ifdef _EDITOR
void EditMain()
{
	//---- MOVE TO SCENE LOAD
	AssetManager::LoadTextureFromFile("test.png");
	AssetManager::LoadAudioDataFromFile("gun-shot.wav", false);
	AssetManager::LoadAudioDataFromFile("music.ogg", false);
	//----
	Editor::Initialize();
	WindowManager::Initialize(1280, 720);
	InputSystem::Initialize(WindowManager::GetHandle(), __X2D_LOAD_INPUTACTIONS);
	SceneManager::Initialize(__X2D_LOAD_SCENES);
	SceneManager::Startup();
	while (Editor::IsOpen())
	{
		//InputSystem::Update();
		Editor::CheckWindowEvents();
		Editor::Update();
		Time::Update();
		Debug::Update();
		SceneManager::Update();
		RenderStack::Draw();
		Editor::OnGUI();
		Editor::Draw();
	}
	Editor::Shutdown();
	SceneManager::Shutdown();
}
#else
void BuildMain()
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
}
#endif // _EDITOR

#ifdef _CONSOLE
int main()
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif // _CONSOLE
{
#ifdef _EDITOR
	EditMain();
#else //_EDITOR
	BuildMain();
#endif //_EDITOR
	return 0;
}