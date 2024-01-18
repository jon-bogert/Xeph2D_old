#pragma once

#ifdef _EDITOR
#include <SFML.hpp>
#include <memory>

namespace Xeph2D::Edit
{
    class EditorSplash final
    {
    public:
        ~EditorSplash() = default;
        EditorSplash(const EditorSplash& other) = delete;
        EditorSplash(const EditorSplash&& other) = delete;
        EditorSplash operator=(const EditorSplash& other) = delete;
        EditorSplash operator=(const EditorSplash&& other) = delete;

        static void Begin();
        static void End();

    private:
        EditorSplash() {}
        static EditorSplash& Get() { static EditorSplash instance; return instance; }
        std::unique_ptr<unsigned char[]> m_textureData = nullptr;
        std::unique_ptr<sf::Texture> m_texture = nullptr;
        std::unique_ptr<sf::Sprite> m_sprite = nullptr;
        std::unique_ptr<sf::RenderWindow> m_window = nullptr;
    };

}

#endif //_EDITOR