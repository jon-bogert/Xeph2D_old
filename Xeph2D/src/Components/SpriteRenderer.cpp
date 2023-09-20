#include "Components/SpriteRenderer.h"
#include "Systems/AssetManager.h"
#include "Systems/RenderStack.h"

using namespace Xeph2D;

void Xeph2D::SpriteRenderer::Awake()
{
	RenderStack::SubscribeDrawCall(this, std::bind(&SpriteRenderer::Draw, this));
}

void Xeph2D::SpriteRenderer::OnDestroy()
{
	RenderStack::UnubscribeDrawCall(this);
}

void Xeph2D::SpriteRenderer::Draw()
{
	if (!_sprite)
		return;
	if (!IsActiveAndEnabled())
		return;

	RenderStack::AddSprite(gameObject, _sprite.get(), order);
}

void SpriteRenderer::LoadTextureByTag(const std::string& tag)
{
	_sprite = std::make_unique<sf::Sprite>();
	sf::Texture& tex = AssetManager::GetTexture(tag);
	_sprite->setTexture(tex);

	_sprite->setOrigin(tex.getSize().x * 0.5f, tex.getSize().y * 0.5f);
}
