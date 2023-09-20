#pragma once

#include <SFML.hpp>

namespace Xeph2D
{
	class Time final
	{
		Time() {}
		static Time& Get();

		float _delta = 0.f;
		float _timescale = 1.f;
		sf::Clock _timer = {};

	public:
		~Time() = default;
		Time(const Time& other) = delete;
		Time(const Time&& other) = delete;
		Time operator=(const Time& other) = delete;
		Time operator=(const Time&& other) = delete;

		static void Update();

		static float DeltaTime();
		static float UnscaledDeltaTime();
		static float GetTimeScale();
		static void SetTimeScale(float timescale);
	};
}