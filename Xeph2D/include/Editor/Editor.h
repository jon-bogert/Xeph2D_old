#pragma once
#ifdef _EDITOR
#include "SFML.hpp"

#include <memory>
#include "EditorWindow.h"
#include "Xeph2D.h"
#include "Editor/EditorWindows/Viewport.h"
#include "Editor/EditorWindows/Inspector.h"
#include "Editor/EditorWindows/Hierarchy.h"
#include "Editor/EditorWindows/ScriptCreator.h"
#include "Editor/EditorWindows/ScriptManager.h"
#include "Editor/EditorSplash.h"
#include "Editor/TransformGizmo.h"

namespace Xeph2D::Edit
{
    class EditorWindow;
    class Editor final
    {
    public:
        ~Editor() = default;
        Editor(const Editor& other) = delete;
        Editor(const Editor&& other) = delete;
        Editor operator=(const Editor& other) = delete;
        Editor operator=(const Editor&& other) = delete;

        static void Initialize();
        static void CheckWindowEvents();
        static void InputProc();
        static void Update();
        static void OnGUI();
        static void Draw();
        static void Shutdown();

        static bool IsOpen();
        static void Close();

        static bool GetHasSaved();
        static void SetHasSaved(const bool hasSaved);

        static void RegisterComponentNames(std::function<std::unordered_map<uint32_t, std::string>(void)> callback);

        static Transform* GetViewportTransform();
        static Inspector* GetInspectorWindow();
        static ScriptManager* GetScriptManager();
        static ScriptCreator* GetScriptCreator();
        static TransformGizmo* GetTransformGizmo();

        static void RebuildProject();

    private:
        void ViewportGUI();
        void SetUIStyle();
        void DoProjectRebuild();

        Editor() {}
        static Editor& Get() { static Editor instance; return instance; }

        std::unique_ptr<sf::RenderWindow> m_window;
        sf::Clock m_frameTimer{};

        std::unique_ptr<sf::Font> m_font = nullptr;
        std::unique_ptr<unsigned char[]> m_fontData = nullptr;
        size_t m_fontDataLength = 0;

        std::vector<std::unique_ptr<EditorWindow>> m_editorWindows;
        void* m_hwnd;

        Transform m_viewportTransform{};
        Viewport* m_viewportWindow = nullptr;
        Inspector* m_inspector = nullptr;
        Hierarchy* m_hierarchyWindow = nullptr;
        ScriptManager* m_scriptManager = nullptr;
        ScriptCreator* m_scriptCreator = nullptr;


        bool m_hasSaved = true;
        bool m_showSaveWindow = false;
        bool m_rebuildProject = false;

        TransformGizmo m_transformGizmo;
    };

}

#endif //_EDITOR