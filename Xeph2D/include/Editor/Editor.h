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
        Editor() {}
        static Editor& Get() { static Editor instance; return instance; }

        std::unique_ptr<sf::RenderWindow> _window;
        sf::Clock _frameTimer{};

        std::unique_ptr<sf::Font> _font = nullptr;
        std::unique_ptr<unsigned char[]> _fontData = nullptr;
        size_t _fontDataLength = 0;

        std::vector<std::unique_ptr<EditorWindow>> _editorWindows;
        void* _hwnd;

        Transform _viewportTransform{};
        Viewport* _viewportWindow = nullptr;
        Inspector* _inspector = nullptr;
        Hierarchy* _hierarchyWindow = nullptr;
        ScriptManager* _scriptManager = nullptr;
        ScriptCreator* _scriptCreator = nullptr;
        

        bool _hasSaved = true;
        bool _showSaveWindow = false;

        TransformGizmo _transformGizmo;

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
        static TransformGizmo* GetTransformGizmo();

    private:
        void ViewportGUI();
        void SetUIStyle();
    };

}

#endif //_EDITOR