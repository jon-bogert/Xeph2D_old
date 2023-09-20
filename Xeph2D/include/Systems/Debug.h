#pragma once
#include "Structs.h"
#include <iostream>

namespace Xeph2D
{
	class Debug final
	{
	public:
		using VertexChain = std::vector<Vector2>;

	private:
		Debug() {}
		static Debug& Get();

		std::vector<sf::RectangleShape> _rectBuffer;
		std::vector<sf::CircleShape> _circleBuffer;
		std::vector<sf::VertexArray> _lineBuffer;


	public:
		~Debug() = default;
		Debug(const Debug& other) = delete;
		Debug(const Debug&& other) = delete;
		Debug operator=(const Debug& other) = delete;
		Debug operator=(const Debug&& other) = delete;

		static void Log(const char* format, ...);
		static void LogWarn(const char* format, ...);
		static void LogErr(const char* format, ...);

		static void DrawLine(Vector2 start, Vector2 end, Color color = Color::White, bool isWorldSpace = true);
		static void DrawChainLine(const VertexChain& vertBuffer, Color color = Color::White, bool isWorldSpace = true);
		static void DrawBoxOutline(Vector2 center, Vector2 span, Color color = Color::White, bool isWorldSpace = true);
		static void DrawBoxFilled(Vector2 center, Vector2 span, Color color = Color::White, bool isWorldSpace = true);
		static void DrawCircleOutline(Vector2 center, float radius, Color color = Color::White, bool isWorldSpace = true);
		static void DrawCircleFilled(Vector2 center, float radius, Color color = Color::White, bool isWorldSpace = true);

		static void DrawToWindow();
	};
}