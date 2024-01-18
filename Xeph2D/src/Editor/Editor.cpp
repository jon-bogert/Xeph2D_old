#ifdef _EDITOR
#include "Editor/Editor.h"
#include <Xeph2D.h>
#include <imgui.h>
#include <imgui-SFML.h>

#include "Editor/EditorWindows/Viewport.h"
#include "Editor/EditorWindows/Inspector.h"
#include "Editor/EditorWindows/Hierarchy.h"

#include "../res/BasierSquare_Medium_otf.h"
#include "../res/JetBrainsMono_ttf.h"

#include <iostream>
#include <windows.h>

using namespace Xeph2D;

void Xeph2D::Edit::Editor::Initialize()
{
	Get().m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), "Xeph2D Editor");
	Get().m_hwnd = (void*)FindWindowA(NULL, "Xeph2D Editor");
	ImGui::SFML::Init(*Get().m_window);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	Get().SetUIStyle();

	Get().m_viewportWindow =
		(Viewport*)Get().m_editorWindows.emplace_back(std::make_unique<Viewport>()).get();
	Get().m_inspector =
		(Inspector*)Get().m_editorWindows.emplace_back(std::make_unique<Inspector>()).get();
	Get().m_hierarchyWindow =
		(Hierarchy*)Get().m_editorWindows.emplace_back(std::make_unique<Hierarchy>()).get();
	Get().m_scriptManager =
		(ScriptManager*)Get().m_editorWindows.emplace_back(std::make_unique<ScriptManager>()).get();
	Get().m_scriptCreator =
		(ScriptCreator*)Get().m_editorWindows.emplace_back(std::make_unique<ScriptCreator>()).get();

	for (auto& window : Get().m_editorWindows)
		window->Initialize();
}

void Xeph2D::Edit::Editor::CheckWindowEvents()
{
	sf::Event winEvent{};
	while (Get().m_window->pollEvent(winEvent))
	{
		ImGui::SFML::ProcessEvent(*Get().m_window, winEvent);

		if (winEvent.type == sf::Event::Closed)
		{
			Close();
		}
	}
}

void Xeph2D::Edit::Editor::InputProc()
{
	if (Get().m_viewportWindow->IsHovered())
	{
		if (InputSystem::GetMouseHold(Mouse::Button::Right) ||
			(InputSystem::GetKeyHold(Key::LAlt) && InputSystem::GetMouseHold(Mouse::Button::Left)))
		{
			Vector2 delta{};
			InputSystem::GetMouseDelta(&delta.x);
			Get().m_viewportTransform.position.x -= WindowManager::PixelToUnit(delta).x;
			Get().m_viewportTransform.position.y += WindowManager::PixelToUnit(delta).y;
		}
	}
	if (Get().m_viewportWindow->IsFocused())
	{
		if (InputSystem::GetKeyDown(Key::W))
		{
			Get().m_transformGizmo.SetMode(TransformGizmo::Mode::Position);
		}
		if (InputSystem::GetKeyDown(Key::E))
		{
			Get().m_transformGizmo.SetMode(TransformGizmo::Mode::Rotation);
		}
		if (InputSystem::GetKeyDown(Key::R))
		{
			Get().m_transformGizmo.SetMode(TransformGizmo::Mode::Scale);
		}
	}
	if (InputSystem::GetKeyHold(Key::Ctrl))
	{
		if (InputSystem::GetKeyHold(Key::Shift))
		{
			if (InputSystem::GetKeyDown(Key::N))
			{
				Get().m_scriptCreator->Open();
			}
		}
		else //============
		{
			if (InputSystem::GetKeyDown(Key::S))
			{
				Serializer::SaveToFile(SceneManager::GetCurrentName());
				Get().m_hasSaved = true;
			}
			if (InputSystem::GetKeyDown(Key::Q))
			{
				Close();
			}
		}
	}
}

void Xeph2D::Edit::Editor::Update()
{
	Get().m_transformGizmo.UpdateMouse(Get().m_viewportWindow->GetMousePos());
	ImGui::SFML::Update(*Get().m_window, Get().m_frameTimer.restart());
}

void Xeph2D::Edit::Editor::OnGUI()
{
	Get().ViewportGUI();
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File##MainMenu"))
	{
		if (ImGui::MenuItem("Save", "Ctrl+S"))
		{
			Serializer::SaveToFile(SceneManager::GetCurrentName());
			Get().m_hasSaved = true;
		}
		if (ImGui::MenuItem("Close", "Ctrl+Q"))
		{
			Editor::Close();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit##MainMenu"))
	{
		if (ImGui::MenuItem("Create New Script", "Ctrl+Shift+N"))
		{
			Get().m_scriptCreator->Open();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View##MainMenu"))
	{
		if (ImGui::BeginMenu("Windows##MainMenu"))
		{
			if (ImGui::MenuItem("Hierarchy"))
			{
				Get().m_hierarchyWindow->Open();
			}
			if (ImGui::MenuItem("Inspector"))
			{
				Get().m_inspector->Open();
			}
			if (ImGui::MenuItem("Script Manager"))
			{
				Get().m_scriptManager->Open();
			}
			if (ImGui::MenuItem("Viewport"))
			{
				Get().m_viewportWindow->Open();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	ImGui::DockSpaceOverViewport();
	for (auto& window : Get().m_editorWindows)
	{
		if (!window->isOpen)
			continue;

		ImGui::Begin(window->GetName(), &window->isOpen, window->flags);
		window->UpdateValues();
		window->OnGUI();
		ImGui::End();
	}
	if (Get().m_showSaveWindow)
	{
		ImGui::Begin("Save Your Progress?", &Get().m_showSaveWindow,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove);
		ImGui::SetWindowPos({ (ImGui::GetMainViewport()->Size.x - ImGui::GetWindowWidth()) * 0.5f, (ImGui::GetMainViewport()->Size.y - ImGui::GetWindowHeight()) * 0.5f });
		ImGui::Text("Would you like to save the Active Scene?");
		ImGui::NewLine();
		if (ImGui::Button("Save"))
		{
			Get().m_hasSaved = true;
			Serializer::SaveToFile(SceneManager::GetCurrentName());
			Close();
			Get().m_showSaveWindow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Don't Save"))
		{
			Get().m_hasSaved = true;
			Close();
			Get().m_showSaveWindow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			Get().m_showSaveWindow = false;
		}
		ImGui::End();
	}
}

void Xeph2D::Edit::Editor::Draw()
{
	Get().m_window->clear({ 5, 5, 5, 255 });
	ImGui::SFML::Render(*Get().m_window);
	Get().m_window->display();
}

void Xeph2D::Edit::Editor::Shutdown()
{
	ImGui::SFML::Shutdown();
	if (Get().m_rebuildProject)
		Get().DoProjectRebuild();
}

bool Xeph2D::Edit::Editor::IsOpen()
{
	return Get().m_window->isOpen();
}

void Xeph2D::Edit::Editor::Close()
{
	bool hs = Get().m_hasSaved;
	if (Get().m_hasSaved)
		Get().m_window->close();
	else
		Get().m_showSaveWindow = true;
}

bool Xeph2D::Edit::Editor::GetHasSaved()
{
	return Get().m_hasSaved;
}

void Xeph2D::Edit::Editor::SetHasSaved(const bool hasSaved)
{
	Get().m_hasSaved = hasSaved;
}

void Xeph2D::Edit::Editor::RegisterComponentNames(std::function<std::unordered_map<uint32_t, std::string>(void)> callback)
{
	if (Get().m_inspector == nullptr)
	{
		Debug::LogErr("Editor -> Inspector window is null, could not register component names");
		return;
	}
	Get().m_inspector->RegisterComponentNames(callback);
}

Transform* Xeph2D::Edit::Editor::GetViewportTransform()
{
	return &Get().m_viewportTransform;
}

Xeph2D::Edit::Inspector* Xeph2D::Edit::Editor::GetInspectorWindow()
{
	return Get().m_inspector;
}

Xeph2D::Edit::ScriptManager* Xeph2D::Edit::Editor::GetScriptManager()
{
	return Get().m_scriptManager;
}

Xeph2D::Edit::ScriptCreator* Xeph2D::Edit::Editor::GetScriptCreator()
{
	return Get().m_scriptCreator;
}

Edit::TransformGizmo* Xeph2D::Edit::Editor::GetTransformGizmo()
{
	return &Get().m_transformGizmo;
}

void Xeph2D::Edit::Editor::RebuildProject()
{
	Get().m_rebuildProject = true;
}

void Xeph2D::Edit::Editor::ViewportGUI()
{
	//TODO Guard against min max of longlong
	Vector2 x0_min = Vector2(WindowManager::WorldWindowMinimum().x, 0);
	Vector2 x0_max = Vector2(WindowManager::WorldWindowMaximum().x, 0);
	Vector2 y0_min = Vector2(0, WindowManager::WorldWindowMinimum().y);
	Vector2 y0_max = Vector2(0, WindowManager::WorldWindowMaximum().y);

	Color worldOriginColor = { 0.75, 0.75, 0.75, 1.f };
	Color worldUnitColor = { 0.2, 0.2, 0.2, 1.f };

	Debug::DrawLine(x0_min, x0_max, worldOriginColor);
	Debug::DrawLine(y0_min, y0_max, worldOriginColor);

	//X-Lines
	for (long long y = (long long)WindowManager::WorldWindowMinimum().y;
		y < WindowManager::WorldWindowMaximum().y; ++y)
	{
		if (y == 0)
			continue;
		Vector2 min = Vector2(WindowManager::WorldWindowMinimum().x, y);
		Vector2 max = Vector2(WindowManager::WorldWindowMaximum().x, y);
		Debug::DrawLine(min, max, worldUnitColor);
	}
	//Y-Lines
	for (long long x = (long long)WindowManager::WorldWindowMinimum().x;
		x < WindowManager::WorldWindowMaximum().x; ++x)
	{
		if (x == 0)
			continue;
		Vector2 min = Vector2(x, WindowManager::WorldWindowMinimum().y);
		Vector2 max = Vector2(x, WindowManager::WorldWindowMaximum().y);
		Debug::DrawLine(min, max, worldUnitColor);
	}
	m_transformGizmo.Draw();
}

void Xeph2D::Edit::Editor::SetUIStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding = 4.0f;
	style.ScrollbarSize = 10.0f;

	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.7f, 0.7f, 0.7f, 0.70f);
	style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	style.GrabRounding = style.FrameRounding = 2.3f;

	//Font
	ImGuiIO& io = ImGui::GetIO();
	ImFontAtlas* fontAtlas = io.Fonts;
	res::BasierSquare_Medium_otf(Get().m_fontData, Get().m_fontDataLength);
	ImFontConfig fontConfig;
	fontConfig.FontDataOwnedByAtlas = false;
	ImWchar defaultRange[] = { 0x0020, 0x00FF, 0x0100, 0x017F, 0 };
	ImFont* font = fontAtlas->AddFontFromMemoryTTF((void*)Get().m_fontData.get(), Get().m_fontDataLength, 16, &fontConfig, defaultRange);
	fontAtlas->Build();
	io.FontDefault = font;

	ImGui::SFML::UpdateFontTexture();
}

void Xeph2D::Edit::Editor::DoProjectRebuild()
{
	HWND handle = GetForegroundWindow();
	int bufsize = GetWindowTextLength(handle) + 1;
	char* title = new char[bufsize];
	GetWindowTextA(handle, title, bufsize);

	system((std::string("start ..\\reload-sln.exe \"") + title + "\"").c_str());

	delete[] title;
}

#endif //_EDITOR