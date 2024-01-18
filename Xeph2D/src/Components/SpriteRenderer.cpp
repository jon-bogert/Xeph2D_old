#include "Components/SpriteRenderer.h"
#include "Systems/AssetManager.h"
#include "Systems/RenderStack.h"
#include "Systems/Serializer.h"

using namespace Xeph2D;

void Xeph2D::SpriteRenderer::EditorInit()
{
	Awake();
}

void Xeph2D::SpriteRenderer::EditorShutdown()
{
	OnDestroy();
}

void SpriteRenderer::Serializables()
{
	SERIALIZE_DEFAULT;
	SERIALIZE_STRING(m_textureKey);
}


void Xeph2D::SpriteRenderer::Awake()
{
	LoadTextureByTag(m_textureKey);
	RenderStack::SubscribeDrawCall(this, std::bind(&SpriteRenderer::Draw, this));
}

void Xeph2D::SpriteRenderer::OnDestroy()
{
	RenderStack::UnubscribeDrawCall(this);
}

void Xeph2D::SpriteRenderer::Draw()
{
	if (!m_sprite)
		return;
	if (!IsActiveAndEnabled())
		return;

	RenderStack::AddSprite(gameObject, m_sprite.get(), order);
}

void SpriteRenderer::LoadTextureByTag(const std::string& tag)
{
	m_sprite = std::make_unique<sf::Sprite>();
	sf::Texture* tex = AssetManager::GetTexture(tag);
	if (tex == nullptr)
	{
		Debug::LogErr("SpriteRenderer %s tried to get texture '%s'", gameObject->name.c_str(), tag.c_str());
#ifdef _EDITOR
		tex = AssetManager::GetTexture("__no-image");
#else
		return;
#endif // _EDITOR
	}
	m_sprite->setTexture(*tex);

	m_sprite->setOrigin(tex->getSize().x * 0.5f, tex->getSize().y * 0.5f);
}
