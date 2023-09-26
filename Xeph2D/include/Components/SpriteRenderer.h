#pragma once
#include "Component.h"
#include "GameObject.h"

#include <SFML.hpp>

#include <string>
#include <memory>

namespace Xeph2D
{
	class SpriteRenderer : public Component
	{
		std::unique_ptr<sf::Sprite> _sprite = nullptr;
		std::string _textureKey = "";
	public:
		COMP_HEADER_STD(2);

		int order = 0;

		void EditorInit() override;
		void EditorShutdown() override;

		void Awake() override;
		void OnDestroy() override;

		void Draw();
		void LoadTextureByTag(const std::string& tag);
	};
}