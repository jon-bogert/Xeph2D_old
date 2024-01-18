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
	if (windRatio > Get().m_aspect)
	{
		Get().m_width = size.y * Get().m_aspect;
		Get().m_height = size.y;
	}
	else
	{
		Get().m_width = size.x;
		Get().m_height = size.x / Get().m_aspect;
	}
	Get().m_resScale = Get().m_height / static_cast<float>(Get().m_refHeight);
	Get().m_viewport->create(Get().m_width, Get().m_height);
}
#endif //_EDITOR

void Xeph2D::WindowManager::Initialize(uint32_t width, uint32_t height)
{
	Get().m_width = width;
	Get().m_height = height;
	Get().m_resScale = Get().m_height / static_cast<float>(Get().m_refHeight);

#ifdef _EDITOR
	Get().m_viewport = std::make_unique<sf::RenderTexture>();
	Get().m_viewport->create(width, height);
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
	(finalTransform.position.x * Get().m_ppu * Get().m_resScale) - (__CAMERA->position.x * Get().m_ppu * Get().m_resScale),
	Get().m_height - ((finalTransform.position.y * Get().m_ppu * Get().m_resScale) - (__CAMERA->position.y * Get().m_ppu * Get().m_resScale)) });
	
	sprite->setScale(finalTransform.scale.x * Get().m_resScale, finalTransform.scale.y * Get().m_resScale);
	sprite->setRotation(finalTransform.rotation.GetDeg());

	//sprite->setPosition(finalTransform.position.x, finalTransform.position.y);
#ifdef _EDITOR
	Get().m_viewport->draw(*sprite);
#else
	Get()._window->draw(*sprite);
#endif // _EDITOR
}

void Xeph2D::WindowManager::SetCamera(Camera* camera)
{
	if (Get().m_camera)
		Debug::LogWarn("WindowManager::SetCamera -> Camera was not cleared before setting");

	Get().m_camera = camera;
}

void Xeph2D::WindowManager::ClearCamera(Camera* camera)
{
	if (!camera)
	{
		Get().m_camera = nullptr;
		return;
	}

	if (camera == Get().m_camera)
	{
		Get().m_camera = nullptr;
	}

	Debug::LogWarn("WindowManager::ClearCamera -> Camera to be cleared was not the same as provided camera");
}

void Xeph2D::WindowManager::CheckWindowEvents()
{
	sf::Event winEvent;
	while (Get().m_window->pollEvent(winEvent))
	{
		if (winEvent.type == sf::Event::Closed)
			Close();
		if (winEvent.type == sf::Event::Resized)
		{
			sf::FloatRect visibleArea(0, 0, winEvent.size.width, winEvent.size.height);
			Get().m_window->setView(sf::View(visibleArea));
			Get().m_width = winEvent.size.width;
			Get().m_height = winEvent.size.height;
			Get().m_resScale = Get().m_height / static_cast<float>(Get().m_refHeight);
		}
	}
}

void Xeph2D::WindowManager::Begin()
{
#ifdef _EDITOR
	Get().m_viewport->clear(Color(0.1f, 0.1f, 0.1f, 1.f));
#else
	Get()._window->clear(sf::Color::Black);
#endif //_EDITOR
}

void Xeph2D::WindowManager::End()
{
#ifdef _EDITOR
	Get().m_viewport->display();
#else
	Get()._window->display();
#endif //_EDITOR
}

bool Xeph2D::WindowManager::IsOpen()
{
	return Get().m_window->isOpen();
}

void Xeph2D::WindowManager::Close()
{
	Get().m_window->close();
}

HWND& WindowManager::GetHandle()
{
	return Get().m_handle;
}

uint32_t Xeph2D::WindowManager::GetWidthPixels()
{
	return Get().m_width;
}

uint32_t Xeph2D::WindowManager::GetHeightPixels()
{
	return Get().m_height;
}

float Xeph2D::WindowManager::GetWidthUnits()
{
	return PixelToUnit(Get().m_width);
}

float Xeph2D::WindowManager::GetHeightUnits()
{
	return PixelToUnit(Get().m_height);
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
	return Vector2(point.x / static_cast<float>(Get().m_ppu) / Get().m_resScale, (Get().m_height - point.y) / static_cast<float>(Get().m_ppu) / Get().m_resScale);
}

Vector2 Xeph2D::WindowManager::PixelToWorld(const Vector2 point)
{
	return PixelToScreen(point) - __CAMERA->position;
}

Vector2 Xeph2D::WindowManager::ScreenToPixel(const Vector2 point)
{
	return Vector2(point.x * Get().m_ppu * Get().m_resScale, Get().m_height - point.y * Get().m_ppu * Get().m_resScale);
}

Vector2 Xeph2D::WindowManager::WorldToPixel(const Vector2 point)
{
	return Vector2((point.x - __CAMERA->position.x) * Get().m_ppu * Get().m_resScale, Get().m_height - (point.y - __CAMERA->position.y) * Get().m_ppu * Get().m_resScale);
}

float Xeph2D::WindowManager::PixelToUnit(const float val)
{
	return val / Get().m_ppu / Get().m_resScale;
}

Vector2 Xeph2D::WindowManager::PixelToUnit(const Vector2 val)
{
	return val / Get().m_ppu / Get().m_resScale;
}

float Xeph2D::WindowManager::UnitToPixel(const float val)
{
	return val * Get().m_ppu * Get().m_resScale;
}

Vector2 Xeph2D::WindowManager::UnitToPixel(const Vector2 val)
{
	return val * Get().m_ppu * Get().m_resScale;
}

void Xeph2D::WindowManager::SetTargetFramerate(uint32_t framerate)
{
#ifndef _EDITOR
	Get()._window->setFramerateLimit(framerate);
#endif //!_EDITOR
}

sf::RenderWindow* Xeph2D::WindowManager::__UnWrap()
{
	return Get().m_window.get();
}
