#pragma once
#ifdef _EDITOR
#include "SFML.hpp"

#include <memory>
#include "EditorWindow.h"
#include "Xeph2D.h"
#include "Editor/EditorWindows/Viewport.h"
#include "Editor/EditorWindows/Inspector.h"

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

        static Transform* GetViewportTransform();
        static Inspector* GetInspectorWindow();

    private:
        void ViewportGUI();
        void SetUIStyle();
    };

}

#endif //_EDITOR