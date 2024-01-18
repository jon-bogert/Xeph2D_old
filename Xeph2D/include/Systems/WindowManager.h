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
	public:
#ifdef _EDITOR
		static sf::RenderTexture* __Viewport() { return Get().m_viewport.get(); }
		static void __UpdateViewportSize(Vector2 size);
#endif //_EDITOR

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
		static float GetWidthUnits();
		static float GetHeightUnits();

		static Vector2 WorldWindowMinimum();
		static Vector2 WorldWindowMaximum();

		static Vector2 PixelToScreen(const Vector2 point);
		static Vector2 PixelToWorld(const Vector2 point);
		static Vector2 ScreenToPixel(const Vector2 point);
		static Vector2 WorldToPixel(const Vector2 point);
		static float PixelToUnit(const float val);
		static Vector2 PixelToUnit(const Vector2 val);
		static float UnitToPixel(const float val);
		static Vector2 UnitToPixel(const Vector2 val);

		static void SetTargetFramerate(uint32_t framerate);

		static sf::RenderWindow* __UnWrap();

	private:
#ifdef _EDITOR
		std::unique_ptr<sf::RenderTexture> m_viewport = nullptr;
#endif //_EDITOR

		WindowManager() {}
		static WindowManager& Get();

		std::unique_ptr<sf::RenderWindow> m_window = nullptr;

		uint32_t m_width = 1280;
		uint32_t m_height = 720;
		uint32_t m_refWidth = 1920;
		uint32_t m_refHeight = 1080;
		float m_aspect = m_refWidth / (float)m_refHeight;
		uint32_t m_ppu = 32;

		float m_resScale = 1.f;

		Camera* m_camera = nullptr;

		HWND m_handle = nullptr;
	};
}