#pragma once

#ifdef _EDITOR
#include <SFML.hpp>
#include <memory>

namespace Xeph2D::Edit
{
    class EditorSplash final
    {
        EditorSplash() {}
        static EditorSplash& Get() { static EditorSplash instance; return instance; }
        std::unique_ptr<unsigned char[]> _textureData = nullptr;
        std::unique_ptr<sf::Texture> _texture = nullptr;
        std::unique_ptr<sf::Sprite> _sprite = nullptr;
        std::unique_ptr<sf::RenderWindow> _window = nullptr;

    public:
        ~EditorSplash() = default;
        EditorSplash(const EditorSplash& other) = delete;
        EditorSplash(const EditorSplash&& other) = delete;
        EditorSplash operator=(const EditorSplash& other) = delete;
        EditorSplash operator=(const EditorSplash&& other) = delete;

        static void Begin();
        static void End();
    };

}

#endif //_EDITOR