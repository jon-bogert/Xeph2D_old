#include "Systems/WindowManager.h"
#include "Systems/Debug.h"

using namespace Xeph2D;

WindowManager& WindowManager::Get()
{
	static WindowManager instance;
	return instance;
}

void Xeph2D::WindowManager::Initialize(uint32_t width, uint32_t height)
{
	Get()._width = width;
	Get()._height = height;
	Get()._resScale = Get()._height / static_cast<float>(Get()._refHeight);

	Get()._window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "WindowTitle");
	Get()._handle = FindWindowA(NULL, "WindowTitle");
}

void Xeph2D::WindowManager::DrawSprite(const GameObject* gameObject, sf::Sprite* sprite)
{
	Transform finalTransform = gameObject->transform;
	if (gameObject->parent)
	{
		GameObject* currObj = gameObject->parent;
		while (currObj)
		{
			finalTransform.position.x += currObj->transform.position.x;
			finalTransform.position.y += currObj->transform.position.y;

			finalTransform.rotation.AddDeg(currObj->transform.rotation.GetDeg());

			finalTransform.scale.x *= currObj->transform.scale.x;
			finalTransform.scale.y *= currObj->transform.scale.y;

			currObj = currObj->parent;
		}
	}

	sprite->setPosition({
	(finalTransform.position.x * Get()._ppu * Get()._resScale) - (Get()._camera->transform->position.x * Get()._ppu * Get()._resScale),
	Get()._height - ((finalTransform.position.y * Get()._ppu * Get()._resScale) - (Get()._camera->transform->position.y * Get()._ppu * Get()._resScale)) });
	
	sprite->setScale(finalTransform.scale.x * Get()._resScale, finalTransform.scale.y * Get()._resScale);
	sprite->setRotation(finalTransform.rotation.GetDeg());

	//sprite->setPosition(finalTransform.position.x, finalTransform.position.y);
	Get()._window->draw(*sprite);
}

void Xeph2D::WindowManager::SetCamera(Camera* camera)
{
	if (Get()._camera)
		Debug::LogWarn("WindowManager::SetCamera -> Camera was not cleared before setting");

	Get()._camera = camera;
}

void Xeph2D::WindowManager::ClearCamera(Camera* camera)
{
	if (!camera)
	{
		Get()._camera = nullptr;
		return;
	}

	if (camera == Get()._camera)
	{
		Get()._camera = nullptr;
	}

	Debug::LogWarn("WindowManager::ClearCamera -> Camera to be cleared was not the same as provided camera");
}

bool Xeph2D::WindowManager::CheckCloseEvent()
{
	sf::Event winEvent;
	while (Get()._window->pollEvent(winEvent))
	{
		if (winEvent.type == sf::Event::Closed)
			return true;
	}
	return false;
}

void Xeph2D::WindowManager::Begin()
{
	Get()._window->clear(sf::Color::Black);
}

void Xeph2D::WindowManager::End()
{
	Get()._window->display();
}

bool Xeph2D::WindowManager::IsOpen()
{
	return Get()._window->isOpen();
}

void Xeph2D::WindowManager::Close()
{
	Get()._window->close();
}

HWND& WindowManager::GetHandle()
{
	return Get()._handle;
}

Vector2 Xeph2D::WindowManager::PixelToScreen(const Vector2 point)
{
	return Vector2(point.x / static_cast<float>(Get()._ppu) / Get()._resScale, (Get()._height - point.y) / static_cast<float>(Get()._ppu) / Get()._resScale);
}

Vector2 Xeph2D::WindowManager::PixelToWorld(const Vector2 point)
{
	return PixelToScreen(point) - Get()._camera->transform->position;
}

Vector2 Xeph2D::WindowManager::ScreenToPixel(const Vector2 point)
{
	return Vector2(point.x * Get()._ppu * Get()._resScale, Get()._height - point.y * Get()._ppu * Get()._resScale);
}

Vector2 Xeph2D::WindowManager::WorldToPixel(const Vector2 point)
{
	return Vector2((point.x - Get()._camera->transform->position.x) * Get()._ppu * Get()._resScale, Get()._height - (point.y - Get()._camera->transform->position.y) * Get()._ppu * Get()._resScale);
}

float Xeph2D::WindowManager::PixelToUnit(const float val)
{
	return val / Get()._ppu / Get()._resScale;
}

Vector2 Xeph2D::WindowManager::PixelToUnit(const Vector2 val)
{
	return val / Get()._ppu / Get()._resScale;
}

float Xeph2D::WindowManager::UnitToPixel(const float val)
{
	return val * Get()._ppu * Get()._resScale;
}

Vector2 Xeph2D::WindowManager::UnitToPixel(const Vector2 val)
{
	return val * Get()._ppu * Get()._resScale;
}

sf::RenderWindow* Xeph2D::WindowManager::__UnWrap()
{
	return Get()._window.get();
}
