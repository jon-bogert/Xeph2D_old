#include "Systems/Time.h"

using namespace Xeph2D;

Time& Time::Get()
{
	static Time instance;
	return instance;
}

void Xeph2D::Time::Update()
{
	Get()._delta = Get()._timer.restart().asSeconds();
}

float Xeph2D::Time::DeltaTime()
{
	return Get()._delta * Get()._timescale;
}

float Xeph2D::Time::UnscaledDeltaTime()
{
	return Get()._delta;
}

float Xeph2D::Time::GetTimeScale()
{
	return Get()._timescale;
}

void Xeph2D::Time::SetTimeScale(float timescale)
{
	Get()._timescale = timescale;
}

uint32_t Xeph2D::Time::FPS()
{
	return (Get()._delta <= 0.f) ? 0.f : 1.f / Get()._delta;
}
