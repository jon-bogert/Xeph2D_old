#include "Systems/Debug.h"
#ifdef _DEBUG
#include "Systems/WindowManager.h"
#include "Systems/Time.h"
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <filesystem>

#include "../res/JetBrainsMono_ttf.h"

#pragma warning (disable : 4996)
#endif // _DEBUG

using namespace Xeph2D;

#define FONT_SIZE 12
#define LINE_OFFSET 14
#define INSTR_OFFSET 80.f

Xeph2D::Debug::Debug()
{
#ifdef _DEBUG
    res::JetBrainsMono_ttf(_fontData, _fontDataLength);

    _font = std::make_unique<sf::Font>();
    _font->loadFromMemory((void*)_fontData.get(), _fontDataLength);

    _textTemplate.setFont(*_font);
    _textTemplate.setCharacterSize(FONT_SIZE);
    _textTemplate.setFillColor(_defaultLogColor);

    InputActionMap* map = InputSystem::CreateInputActionMap("Xeph2D::Debug");
    InputAction* toggleShowGraphics = map->CreateAction("ToggleShowGraphics");
    toggleShowGraphics->AddButton(_drawGraphicsKey);
    toggleShowGraphics->performed.Subscribe(XEInputActionCallback(Debug::ToggleShowGraphics));
    InputAction* toggleShowOutput = map->CreateAction("ToggleShowOutput");
    toggleShowOutput->AddButton(_drawOutputKey);
    toggleShowOutput->performed.Subscribe(XEInputActionCallback(Debug::ToggleShowOutput));

    _instructions = _textTemplate;
    _instructions.setString("Press: [F1] to hide Debug Text, [F2] to hide Debug Graphics");
    _instructions.setPosition(INSTR_OFFSET, 0.f);

    if (_logToFile)
    {
        if (!std::filesystem::exists(L"log/"))
            std::filesystem::create_directories(L"log/");

        time_t now = time(0);
        struct tm* localtm = localtime(&now);
        char timebuffer[80];
        strftime(timebuffer, sizeof(timebuffer), "%Y-%m-%d_%H-%M-%S", localtm);

        _logFile.open("log/" + std::string(timebuffer) + ".log");
    }
#endif // _DEBUG
}

Xeph2D::Debug::~Debug()
{
#ifdef _DEBUG
    if (_logFile.is_open())
        _logFile.close();
#endif // _DEBUG
}

Debug& Debug::Get()
{
    static Debug instance;
    return instance;
}

void Xeph2D::Debug::Update()
{
#ifdef _DEBUG
#ifndef _EDITOR
    if (Get()._closeOnEscape && InputSystem::GetKeyDown(Key::Esc))
        WindowManager::Close();
#endif //!_EDITOR

    for (size_t i = 0; i < Get()._logBuffer.size(); ++i)
    {
        Get()._logBuffer[i].timer += Time::UnscaledDeltaTime();
        if (Get()._logBuffer[i].timer >= Get()._logTime)
        {
            Get()._logBuffer.resize(i);
            return;
        }
    }
#endif //_DEBUG
}

void Xeph2D::Debug::Log(const char* format, ...)
{
#ifdef _DEBUG
    time_t now = time(0);
    struct tm* localtm = localtime(&now);

    char timebuffer[80];

    strftime(timebuffer, sizeof(timebuffer), "%Y-%m-%d %H:%M:%S", localtm);

    LogEntry entry;
    entry.dateTime = "{" + std::string(timebuffer) + "} ";

    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    entry.contents = " [LOG] " + std::string(buffer);
    entry.color = Get()._logColor;
    Get()._logColor = Get()._defaultLogColor;

    Get().AddToLogBuffer(entry);
#ifdef _CONSOLE
    std::cout << entry.dateTime + entry.contents << std::endl;
#else
    OutputDebugStringA((entry.dateTime + entry.contents + "\n").c_str());
#endif // _CONSOLE
    Get().LogToFile(entry.dateTime + entry.contents);
#endif //_DEBUG
}

void Xeph2D::Debug::LogWarn(const char* format, ...)
{
#ifdef _DEBUG
    time_t now = time(0);
    struct tm* localtm = localtime(&now);

    char timebuffer[80];

    strftime(timebuffer, sizeof(timebuffer), "%Y-%m-%d %H:%M:%S", localtm);

    LogEntry entry;
    entry.dateTime = "{" + std::string(timebuffer) + "} ";

    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    entry.contents = "[WARN] " + std::string(buffer);
    entry.color = Get()._warnColor;
    Get()._warnColor = Get()._defaultWarnColor;

    Get().AddToLogBuffer(entry);
#ifdef _CONSOLE
    std::cout << entry.dateTime + entry.contents << std::endl;
#else
    OutputDebugStringA((entry.dateTime + entry.contents + "\n").c_str());
#endif // _CONSOLE
    Get().LogToFile(entry.dateTime + entry.contents);
#endif //_DEBUG
}

void Xeph2D::Debug::LogErr(const char* format, ...)
{
#ifdef _DEBUG
    time_t now = time(0);
    struct tm* localtm = localtime(&now);

    char timebuffer[80];

    strftime(timebuffer, sizeof(timebuffer), "%Y-%m-%d %H:%M:%S", localtm);

    LogEntry entry;
    entry.dateTime = "{" + std::string(timebuffer) + "} ";

    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    entry.contents = " [ERR] " + std::string(buffer);
    entry.color = Get()._errColor;
    Get()._errColor = Get()._defaultErrColor;

    Get().AddToLogBuffer(entry);
#ifdef _CONSOLE
    std::cout << entry.dateTime + entry.contents << std::endl;
#else
    OutputDebugStringA((entry.dateTime + entry.contents + "\n").c_str());
#endif // _CONSOLE
    Get().LogToFile(entry.dateTime + entry.contents);
#endif //_DEBUG
}

void Xeph2D::Debug::LogColor(Color color, LogType type)
{
#ifdef _DEBUG
    switch (type)
    {
    case LogType::Log:
        Get()._logColor = color;
        break;
    case LogType::Warn:
        Get()._warnColor = color;
        break;
    case LogType::Err:
        Get()._errColor = color;
        break;
    }
#endif // _DEBUG
}

void Xeph2D::Debug::Monitor(const std::string& key, const std::string& valueStr)
{
#ifdef _DEBUG
    Get()._monitorBuffer[key] = valueStr;
#endif // _DEBUG
}

void Xeph2D::Debug::ClearMonitorBuffer()
{
#ifdef _DEBUG
    Get()._monitorBuffer.clear();
#endif // _DEBUG
}

void Debug::DrawLine(Vector2 start, Vector2 end, Color color, bool isWorldSpace)
{
#ifdef _DEBUG
    sf::VertexArray line = sf::VertexArray(sf::LinesStrip, 2);
    line[0].position = (isWorldSpace) ? WindowManager::WorldToPixel(start) : WindowManager::ScreenToPixel(start);
    line[1].position = (isWorldSpace) ? WindowManager::WorldToPixel(end) : WindowManager::ScreenToPixel(end);
    line[0].color = line[1].color = color;
    Get()._lineBuffer.push_back(line);
#endif // _DEBUG
}

void Debug::DrawChainLine(const VertexChain& vertBuffer, Color color, bool isWorldSpace)
{
#ifdef _DEBUG
    sf::VertexArray chain = sf::VertexArray(sf::LinesStrip, vertBuffer.size());
    for (int i = 0; i < vertBuffer.size(); ++i)
    {
        chain[i].position = (isWorldSpace) ? WindowManager::WorldToPixel(vertBuffer[i]) : WindowManager::ScreenToPixel(vertBuffer[i]);
        chain[i].color = color;
    }
#endif // _DEBUG
}

void Debug::DrawBoxOutline(Vector2 center, Vector2 span, Color color, bool isWorldSpace)
{
#ifdef _DEBUG
    sf::RectangleShape shape;
    shape.setFillColor(Color::Transparent);
    shape.setOutlineColor(color);
    shape.setOutlineThickness(-1.f);
    Vector2 size = WindowManager::UnitToPixel(span);
    shape.setSize(size);
    shape.setOrigin(size * 0.5f);
    shape.setPosition((isWorldSpace) ? WindowManager::WorldToPixel(center) : WindowManager::ScreenToPixel(center));
    Get()._rectBuffer.push_back(shape);
#endif // _DEBUG
}

void Debug::DrawBoxFilled(Vector2 center, Vector2 span, Color color, bool isWorldSpace)
{
#ifdef _DEBUG
    sf::RectangleShape shape;
    shape.setFillColor(color);
    Vector2 size = WindowManager::UnitToPixel(span);
    shape.setSize(size);
    shape.setOrigin(size * 0.5f);
    shape.setPosition((isWorldSpace) ? WindowManager::WorldToPixel(center) : WindowManager::ScreenToPixel(center));
    Get()._rectBuffer.insert(Get()._rectBuffer.begin(), shape);
#endif // _DEBUG
}

void Debug::DrawCircleOutline(Vector2 center, float radius, Color color, bool isWorldSpace)
{
#ifdef _DEBUG
    sf::CircleShape shape;
    shape.setFillColor(Color::Transparent);
    shape.setOutlineColor(color);
    shape.setOutlineThickness(-1.f);
    float rad = WindowManager::UnitToPixel(radius);
    shape.setRadius(rad);
    shape.setOrigin(rad, rad);
    shape.setPosition((isWorldSpace) ? WindowManager::WorldToPixel(center) : WindowManager::ScreenToPixel(center));
    Get()._circleBuffer.push_back(shape);
#endif // _DEBUG
}

void Debug::DrawCircleFilled(Vector2 center, float radius, Color color, bool isWorldSpace)
{
#ifdef _DEBUG
    sf::CircleShape shape;
    shape.setFillColor(color);
    float rad = WindowManager::UnitToPixel(radius);
    shape.setRadius(rad);
    shape.setOrigin(rad, rad);
    shape.setPosition((isWorldSpace) ? WindowManager::WorldToPixel(center) : WindowManager::ScreenToPixel(center));
    Get()._circleBuffer.insert(Get()._circleBuffer.begin(), shape);
#endif // _DEBUG
}

void Debug::DrawToWindow()
{
#ifdef _DEBUG

#ifdef _EDITOR
    if (Get()._drawGraphics)
    {
        for (sf::RectangleShape& r : Get()._rectBuffer)
            WindowManager::__Viewport()->draw(r);

        for (sf::CircleShape& c : Get()._circleBuffer)
            WindowManager::__Viewport()->draw(c);

        for (sf::VertexArray& va : Get()._lineBuffer)
        {
            if (va.getVertexCount() <= 1)
                return;

            WindowManager::__Viewport()->draw(va);
        }
    }

    if (Get()._drawOutput)
    {
        WindowManager::__Viewport()->draw(Get()._instructions);

        Get().UpdateTextBuffer();

        for (sf::Text& t : Get()._textBuffer)
        {
            WindowManager::__Viewport()->draw(t);
        }
    }
#else
    if (Get()._drawGraphics)
    {
        for (sf::RectangleShape& r : Get()._rectBuffer)
            WindowManager::__UnWrap()->draw(r);

        for (sf::CircleShape& c : Get()._circleBuffer)
            WindowManager::__UnWrap()->draw(c);

        for (sf::VertexArray& va : Get()._lineBuffer)
        {
            if (va.getVertexCount() <= 1)
                return;

            WindowManager::__UnWrap()->draw(va);
        }
    }

    if (Get()._drawOutput)
    {
        WindowManager::__UnWrap()->draw(Get()._instructions);

        Get().UpdateTextBuffer();

        for (sf::Text& t : Get()._textBuffer)
        {
            WindowManager::__UnWrap()->draw(t);
        }
    }
#endif //_EDITOR

    Get()._rectBuffer.clear();
    Get()._circleBuffer.clear();
    Get()._lineBuffer.clear();
#endif // _DEBUG
}

#ifdef _DEBUG
void Xeph2D::Debug::UpdateTextBuffer()
{
    _textBuffer.resize(1 + _monitorBuffer.size() + _logBuffer.size(), _textTemplate);
    _textBuffer[0].setString("FPS: " + std::to_string(Time::FPS()));
    size_t index = 1;
    for (auto& keyval : _monitorBuffer)
    {
        sf::Text& text = _textBuffer[index];
        text.setString(keyval.first + ": " + keyval.second);
        text.setPosition(0, LINE_OFFSET * (index++));
    }
    for (LogEntry& entry : _logBuffer)
    {
        sf::Text& text = _textBuffer[index];
        Color color = entry.color;
        color.a = (_logTime - entry.timer) / _logTime;
        text.setColor(color);
        text.setString(entry.contents);
        text.setPosition(0, LINE_OFFSET * (index++));
    }
}

void Xeph2D::Debug::AddToLogBuffer(const LogEntry& entry)
{
    _logBuffer.push_front(entry);
}

void Xeph2D::Debug::ToggleShowOutput(InputAction* ctx)
{
    _drawOutput = !_drawOutput;
}

void Xeph2D::Debug::ToggleShowGraphics(InputAction* ctx)
{
    _drawGraphics = !_drawGraphics;
}

void Xeph2D::Debug::LogToFile(const std::string& msg)
{
    if (!_logToFile || !_logFile.is_open())
        return;

    _logFile << msg << std::endl;
    _logFile.flush();
}
#endif // _DEBUG
