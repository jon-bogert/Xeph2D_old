#include "Systems/Debug.h"
#include "Systems/WindowManager.h"
#include <cstdio>
#include <cstdarg>
#include <ctime>

#pragma warning (disable : 4996)

using namespace Xeph2D;

Debug& Debug::Get()
{
    static Debug instance;
    return instance;
}

void Xeph2D::Debug::Log(const char* format, ...)
{
#ifdef _DEBUG
    time_t now = time(0);
    struct tm* localtm = localtime(&now);

    char buffer[80];

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtm);

    printf("{%s}  [LOG] ", buffer);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
#endif //_DEBUG
}

void Xeph2D::Debug::LogWarn(const char* format, ...)
{
#ifdef _DEBUG
    time_t now = time(0);
    struct tm* localtm = localtime(&now);

    char buffer[80];

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtm);

    printf("{%s} [WARN] ", buffer);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
#endif //_DEBUG
}

void Xeph2D::Debug::LogErr(const char* format, ...)
{
#ifdef _DEBUG
    time_t now = time(0);
    struct tm* localtm = localtime(&now);

    char buffer[80];

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtm);

    printf("{%s}  [ERR] ", buffer);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
#endif //_DEBUG
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

    Get()._rectBuffer.clear();
    Get()._circleBuffer.clear();
    Get()._lineBuffer.clear();
#endif // _DEBUG
}
