#pragma once
#include "Structs.h"
#include "InputSystem/InputSystem.h"
#include <iostream>
#include <deque>
#include <fstream>

namespace Xeph2D
{
	class Debug final
	{
	public:
		using VertexChain = std::vector<Vector2>;
		enum class LogType {Log, Warn, Err};

	private:
		struct LogEntry
		{
			Color color = Color::LightGrey;
			float timer = 0.f;
			std::string dateTime = "";
			std::string contents = "";
		};

		Debug();
		static Debug& Get();

		bool _logToFile = false;
		std::string _logPath = "log/";
		std::ofstream _logFile;

		bool _drawOutput = true;
		bool _drawGraphics = true;
		Key _drawOutputKey = Key::F1;
		Key _drawGraphicsKey = Key::F2;

		std::vector<sf::RectangleShape> _rectBuffer;
		std::vector<sf::CircleShape> _circleBuffer;
		std::vector<sf::VertexArray> _lineBuffer;

		std::unique_ptr<sf::Font> _font = nullptr;
		std::unique_ptr<unsigned char[]> _fontData = nullptr;
		size_t _fontDataLength = 0;
		std::deque<sf::Text> _textBuffer;
		sf::Text _instructions;
		sf::Text _textTemplate{};

		std::map<std::string, std::string> _monitorBuffer;
		std::deque<LogEntry> _logBuffer;
		Color _defaultLogColor = Color::LightGrey;
		Color _defaultWarnColor = Color::Yellow;
		Color _defaultErrColor = Color::Red;
		Color _logColor = _defaultLogColor;
		Color _warnColor = _defaultWarnColor;
		Color _errColor = _defaultErrColor;
		float _logTime = 5.f;


	public:
		~Debug();
		Debug(const Debug& other) = delete;
		Debug(const Debug&& other) = delete;
		Debug operator=(const Debug& other) = delete;
		Debug operator=(const Debug&& other) = delete;

		static void Update();

		static void Log(const char* format, ...);
		static void LogWarn(const char* format, ...);
		static void LogErr(const char* format, ...);
		static void LogColor(Color color, LogType type = LogType::Log);
		static void Monitor(const std::string& key, const std::string& valueStr);
		static void ClearMonitorBuffer();

		static void DrawLine(Vector2 start, Vector2 end, Color color = Color::White, bool isWorldSpace = true);
		static void DrawChainLine(const VertexChain& vertBuffer, Color color = Color::White, bool isWorldSpace = true);
		static void DrawBoxOutline(Vector2 center, Vector2 span, Color color = Color::White, bool isWorldSpace = true);
		static void DrawBoxFilled(Vector2 center, Vector2 span, Color color = Color::White, bool isWorldSpace = true);
		static void DrawCircleOutline(Vector2 center, float radius, Color color = Color::White, bool isWorldSpace = true);
		static void DrawCircleFilled(Vector2 center, float radius, Color color = Color::White, bool isWorldSpace = true);

		static void DrawToWindow();

	private:
		void UpdateTextBuffer();
		void AddToLogBuffer(const LogEntry& entry);
		void ToggleShowOutput(InputAction* ctx);
		void ToggleShowGraphics(InputAction* ctx);
		void LogToFile(const std::string& msg);
	};
}