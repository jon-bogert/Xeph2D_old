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
	AssetManager::LoadTextureFromFile("test2.png");
	AssetManager::LoadAudioDataFromFile("gun-shot.wav", false);
	AssetManager::LoadAudioDataFromFile("music.ogg", false);
	//----
	Editor::Initialize();
	WindowManager::Initialize(1280, 720);
	InputSystem::Initialize(WindowManager::GetHandle(), __X2D_LOAD_INPUTACTIONS);
	SceneManager::Initialize(__X2D_LOAD_SCENES);
	SceneManager::Serializables();
	SceneManager::EditorInit();
	while (Editor::IsOpen())
	{
		InputSystem::Update();
		Editor::CheckWindowEvents();
		Editor::InputProc();
		Editor::Update();
		Time::Update();
		Debug::Update();
		//SceneManager::Update();
#ifdef _DEBUG
		SceneManager::DebugDraw();
#endif //_DEBUG
		RenderStack::Draw();
		Editor::OnGUI();
		Editor::Draw();
	}
	Serializer::SaveToFile(SceneManager::GetCurrentName());
	Editor::Shutdown();
	SceneManager::EditorShutdown();
	AssetManager::UnloadAll();
}
#else
void BuildMain()
{
	//---- MOVE TO SCENE LOAD
	AssetManager::LoadTextureFromFile("test.png");
	AssetManager::LoadTextureFromFile("test2.png");
	AssetManager::LoadAudioDataFromFile("gun-shot.wav", false);
	AssetManager::LoadAudioDataFromFile("music.ogg", false);
	//----
	WindowManager::Initialize(1280, 720);
	InputSystem::Initialize(WindowManager::GetHandle(), __X2D_LOAD_INPUTACTIONS);
	SceneManager::Initialize(__X2D_LOAD_SCENES);
	SceneManager::Serializables();
	SceneManager::Startup();
	while (WindowManager::IsOpen())
	{
		InputSystem::Update();
		WindowManager::CheckWindowEvents();
		Time::Update();
		Debug::Update();
		SceneManager::Update();
#ifdef _DEBUG
		SceneManager::DebugDraw();
#endif //_DEBUG
		RenderStack::Draw();
		SceneManager::HandleSceneChange();
	}
	SceneManager::Shutdown();
	AssetManager::UnloadAll();
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