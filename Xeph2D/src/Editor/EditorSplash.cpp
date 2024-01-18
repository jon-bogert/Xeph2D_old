#ifdef _EDITOR
#include "Editor/EditorSplash.h"

#include "../../res/editor_splash_png.h"

void Xeph2D::Edit::EditorSplash::Begin()
{
	size_t dataSize = 0;
	res::editor_splash_png(Get().m_textureData, dataSize);
	Get().m_texture = std::make_unique<sf::Texture>();
	Get().m_texture->loadFromMemory(Get().m_textureData.get(), dataSize);
	Get().m_sprite = std::make_unique<sf::Sprite>();
	Get().m_sprite->setTexture(*Get().m_texture);
	Get().m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Xeph2D Loading...", sf::Style::None);
	Get().m_window->clear(sf::Color::Black);
	Get().m_window->draw(*Get().m_sprite);
	Get().m_window->display();
}

void Xeph2D::Edit::EditorSplash::End()
{
	Get().m_window->close();
	Get().m_window = nullptr;
	Get().m_sprite = nullptr;
	Get().m_texture = nullptr;
	Get().m_textureData = nullptr;
}

#endif //_EDITOR