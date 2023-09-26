#ifdef _EDITOR
#include "Editor/EditorSplash.h"

#include "../../res/editor_splash_png.h"

void Xeph2D::Edit::EditorSplash::Begin()
{
	size_t dataSize = 0;
	res::editor_splash_png(Get()._textureData, dataSize);
	Get()._texture = std::make_unique<sf::Texture>();
	Get()._texture->loadFromMemory(Get()._textureData.get(), dataSize);
	Get()._sprite = std::make_unique<sf::Sprite>();
	Get()._sprite->setTexture(*Get()._texture);
	Get()._window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Xeph2D Loading...", sf::Style::None);
	Get()._window->clear(sf::Color::Black);
	Get()._window->draw(*Get()._sprite);
	Get()._window->display();
}

void Xeph2D::Edit::EditorSplash::End()
{
	Get()._window->close();
	Get()._window = nullptr;
	Get()._sprite = nullptr;
	Get()._texture = nullptr;
	Get()._textureData = nullptr;
}

#endif //_EDITOR