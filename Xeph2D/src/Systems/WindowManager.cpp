#include "Systems/WindowManager.h"
#include "Systems/Debug.h"

using namespace Xeph2D;

#ifdef _EDITOR
#include "Editor/Editor.h"
#define __CAMERA Edit::Editor::GetViewportTransform()
#else
#define __CAMERA Get()._camera->transform
#endif //_EDITOR

WindowManager& WindowManager::Get()
{
	static WindowManager instance;
	return instance;
}

#ifdef _EDITOR
void Xeph2D::WindowManager::__UpdateViewportSize(Vector2 size)
{
	float windRatio = size.x / size.y;
	if (windRatio > Get()._aspect)
	{
		Get()._width = size.y * Get()._aspect;
		Get()._height = size.y;
	}
	else
	{
		Get()._width = size.x;
		Get()._height = size.x / Get()._aspect;
	}
	Get()._resScale = Get()._height / static_cast<float>(Get()._refHeight);
	Get()._viewport->create(Get()._width, Get()._height);
}
#endif //_EDITOR

void Xeph2D::WindowManager::Initialize(uint32_t width, uint32_t height)
{
	Get()._width = width;
	Get()._height = height;
	Get()._resScale = Get()._height / static_cast<float>(Get()._refHeight);

#ifdef _EDITOR
	Get()._viewport = std::make_unique<sf::RenderTexture>();
	Get()._viewport->create(width, height);
#else
	Get()._window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "WindowTitle");
	Get()._handle = FindWindowA(NULL, "WindowTitle");
#endif //_EDITOR
}

void Xeph2D::WindowManager::DrawSprite(const GameObject* gameObject, sf::Sprite* sprite)
{
	Transform finalTransform = gameObject->transform;
	if (gameObject->GetParent())
	{
		GameObject* currObj = gameObject->GetParent();
		while (currObj)
		{
			finalTransform.position.x += currObj->transform.position.x;
			finalTransform.position.y += currObj->transform.position.y;

			finalTransform.rotation.AddDeg(currObj->transform.rotation.GetDeg());

			finalTransform.scale.x *= currObj->transform.scale.x;
			finalTransform.scale.y *= currObj->transform.scale.y;

			currObj = currObj->GetParent();
		}
	}

	sprite->setPosition({
	(finalTransform.position.x * Get()._ppu * Get()._resScale) - (__CAMERA->position.x * Get()._ppu * Get()._resScale),
	Get()._height - ((finalTransform.position.y * Get()._ppu * Get()._resScale) - (__CAMERA->position.y * Get()._ppu * Get()._resScale)) });
	
	sprite->setScale(finalTransform.scale.x * Get()._resScale, finalTransform.scale.y * Get()._resScale);
	sprite->setRotation(finalTransform.rotation.GetDeg());

	//sprite->setPosition(finalTransform.position.x, finalTransform.position.y);
#ifdef _EDITOR
	Get()._viewport->draw(*sprite);
#else
	Get()._window->draw(*sprite);
#endif // _EDITOR
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

void Xeph2D::WindowManager::CheckWindowEvents()
{
	sf::Event winEvent;
	while (Get()._window->pollEvent(winEvent))
	{
		if (winEvent.type == sf::Event::Closed)
			Close();
		if (winEvent.type == sf::Event::Resized)
		{
			sf::FloatRect visibleArea(0, 0, winEvent.size.width, winEvent.size.height);
			Get()._window->setView(sf::View(visibleArea));
			Get()._width = winEvent.size.width;
			Get()._height = winEvent.size.height;
			Get()._resScale = Get()._height / static_cast<float>(Get()._refHeight);
		}
	}
}

void Xeph2D::WindowManager::Begin()
{
#ifdef _EDITOR
	Get()._viewport->clear(Color(0.1f, 0.1f, 0.1f, 1.f));
#else
	Get()._window->clear(sf::Color::Black);
#endif //_EDITOR
}

void Xeph2D::WindowManager::End()
{
#ifdef _EDITOR
	Get()._viewport->display();
#else
	Get()._window->display();
#endif //_EDITOR
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

uint32_t Xeph2D::WindowManager::GetWidthPixels()
{
	return Get()._width;
}

uint32_t Xeph2D::WindowManager::GetHeightPixels()
{
	return Get()._height;
}

float Xeph2D::WindowManager::GetWidthUnits()
{
	return PixelToUnit(Get()._width);
}

float Xeph2D::WindowManager::GetHeightUnits()
{
	return PixelToUnit(Get()._height);
}

Vector2 Xeph2D::WindowManager::WorldWindowMinimum()
{
	return Vector2(__CAMERA->position.x, __CAMERA->position.y);
}

Vector2 Xeph2D::WindowManager::WorldWindowMaximum()
{
	return Vector2(__CAMERA->position.x + (float)GetWidthUnits(), __CAMERA->position.y + (float)GetHeightUnits());
}

Vector2 Xeph2D::WindowManager::PixelToScreen(const Vector2 point)
{
	return Vector2(point.x / static_cast<float>(Get()._ppu) / Get()._resScale, (Get()._height - point.y) / static_cast<float>(Get()._ppu) / Get()._resScale);
}

Vector2 Xeph2D::WindowManager::PixelToWorld(const Vector2 point)
{
	return PixelToScreen(point) - __CAMERA->position;
}

Vector2 Xeph2D::WindowManager::ScreenToPixel(const Vector2 point)
{
	return Vector2(point.x * Get()._ppu * Get()._resScale, Get()._height - point.y * Get()._ppu * Get()._resScale);
}

Vector2 Xeph2D::WindowManager::WorldToPixel(const Vector2 point)
{
	return Vector2((point.x - __CAMERA->position.x) * Get()._ppu * Get()._resScale, Get()._height - (point.y - __CAMERA->position.y) * Get()._ppu * Get()._resScale);
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

void Xeph2D::WindowManager::SetTargetFramerate(uint32_t framerate)
{
#ifndef _EDITOR
	Get()._window->setFramerateLimit(framerate);
#endif //!_EDITOR
}

sf::RenderWindow* Xeph2D::WindowManager::__UnWrap()
{
	return Get()._window.get();
}
