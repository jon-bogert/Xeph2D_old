#ifdef _EDITOR
#include "Editor/Editor.h"
#include <Xeph2D.h>
#include <imgui.h>
#include <imgui-SFML.h>
//#include "Editor/EditorWindow.h"

#include "Editor/EditorWindows/Viewport.h"
#include "Editor/EditorWindows/Inspector.h"

#include "../res/BasierSquare_Medium_otf.h"
#include "../res/JetBrainsMono_ttf.h"

#include <iostream>
#include <windows.h>

using namespace Xeph2D;

void Xeph2D::Edit::Editor::Initialize()
{
    Get()._window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), "Xeph2D Editor");
	Get()._hwnd = (void*)FindWindowA(NULL, "Xeph2D Editor");
    ImGui::SFML::Init(*Get()._window);

    ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    Get().SetUIStyle();

	Get()._editorWindows.emplace_back(std::make_unique<Viewport>());
	Get()._editorWindows.emplace_back(std::make_unique<Inspector>());

	for (auto& window : Get()._editorWindows)
		window->Initialize();
}

void Xeph2D::Edit::Editor::CheckWindowEvents()
{
    sf::Event winEvent{};
    while (Get()._window->pollEvent(winEvent))
    {
        ImGui::SFML::ProcessEvent(*Get()._window, winEvent);

        if (winEvent.type == sf::Event::Closed)
        {
            Get()._window->close();
        }
    }
}

void Xeph2D::Edit::Editor::Update()
{
    ImGui::SFML::Update(*Get()._window, Get()._frameTimer.restart());
}

void Xeph2D::Edit::Editor::OnGUI()
{
	
	ImGui::BeginMainMenuBar();
	ImGui::MenuItem("File##MainMenu");
	ImGui::MenuItem("Edit##MainMenu");
	ImGui::MenuItem("View##MainMenu");
	ImGui::EndMainMenuBar();
	ImGui::DockSpaceOverViewport();
	for (auto& window : Get()._editorWindows)
	{
		if (!window->isOpen)
			continue;

		ImGui::Begin(window->GetName(), &window->isOpen, window->flags);
		window->OnGUI();
		ImGui::End();
	}
}

void Xeph2D::Edit::Editor::Draw()
{
    Get()._window->clear({ 5, 5, 5, 255 });
    ImGui::SFML::Render(*Get()._window);
    Get()._window->display();
}

void Xeph2D::Edit::Editor::Shutdown()
{
    ImGui::SFML::Shutdown();
}

bool Xeph2D::Edit::Editor::IsOpen()
{
    return Get()._window->isOpen();
}

void Xeph2D::Edit::Editor::SetUIStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	// Modify the style colors
	//style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);             // Text color
	//style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);         // Window background color
	//style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);          // Title background color
	//style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);            // Button background color
	//style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);     // Button hover color
	//style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);      // Button active color

	style.WindowRounding = 4.0f;  // Set window corner rounding to 4 pixels
	//style.FrameRounding = 2.0f;   // Set frame corner rounding to 2 pixels
	style.ScrollbarSize = 10.0f;  // Set scrollbar size to 10 pixels

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
	style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
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
	//res::JetBrainsMono_ttf(Get()._fontData, Get()._fontDataLength);
	res::BasierSquare_Medium_otf(Get()._fontData, Get()._fontDataLength);
	ImFontConfig fontConfig;
	fontConfig.FontDataOwnedByAtlas = false; // Important: Set to false so ImGui doesn't try to free the memory
	ImWchar defaultRange[] = { 0x0020, 0x00FF, 0x0100, 0x017F, 0 }; // Specify the character ranges you want to support
	ImFont* font = fontAtlas->AddFontFromMemoryTTF((void*)Get()._fontData.get(), Get()._fontDataLength, 16, &fontConfig, defaultRange);
	//ImFont* font = fontAtlas->AddFontFromFileTTF("JetBrainsMono.ttf", 16, &fontConfig, defaultRange);
	fontAtlas->Build();
	io.FontDefault = font;
	
	ImGui::SFML::UpdateFontTexture();
}

#endif //_EDITOR