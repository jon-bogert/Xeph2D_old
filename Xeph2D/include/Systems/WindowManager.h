#pragma once

#include "GameObject.h"
#include "Components/Camera.h"

#include <SFML.hpp>

#include <memory>
#include <Windows.h>

namespace Xeph2D
{
	class WindowManager final
	{
		WindowManager() {}
		static WindowManager& Get();
#ifdef _EDITOR
		std::unique_ptr<sf::RenderTexture> _viewport = nullptr;
	public:
		static sf::RenderTexture* __Viewport() { return Get()._viewport.get(); }
#endif
		std::unique_ptr<sf::RenderWindow> _window = nullptr;

		uint32_t _width = 1280;
		uint32_t _height = 720;
		uint32_t _refWidth = 1920;
		uint32_t _refHeight = 1080;

		uint32_t _ppu = 32;

		float _resScale = 1.f;

		Camera* _camera = nullptr;

		HWND _handle = nullptr;

	public:
		~WindowManager() = default;
		WindowManager(const WindowManager& other) = delete;
		WindowManager(const WindowManager&& other) = delete;
		WindowManager operator=(const WindowManager& other) = delete;
		WindowManager operator=(const WindowManager&& other) = delete;

		static void Initialize(uint32_t width, uint32_t height);

		static void DrawSprite(const GameObject* gameObject, sf::Sprite* sprite);

		static void SetCamera(Camera* camera);
		static void ClearCamera(Camera* camera = nullptr);

		static void CheckWindowEvents();
		static void Begin();
		static void End();

		static bool IsOpen();
		static void Close();
		static HWND& GetHandle();

		static uint32_t GetWidthPixels();
		static uint32_t GetHeightPixels();
		static uint32_t GetWidthUnits();
		static uint32_t GetHeightUnits();

		static Vector2 PixelToScreen(const Vector2 point);
		static Vector2 PixelToWorld(const Vector2 point);
		static Vector2 ScreenToPixel(const Vector2 point);
		static Vector2 WorldToPixel(const Vector2 point);
		static float PixelToUnit(const float val);
		static Vector2 PixelToUnit(const Vector2 val);
		static float UnitToPixel(const float val);
		static Vector2 UnitToPixel(const Vector2 val);

		static sf::RenderWindow* __UnWrap();
	};
}