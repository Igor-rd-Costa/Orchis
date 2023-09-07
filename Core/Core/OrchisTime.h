#pragma once

#include "Timer.h"

namespace Orchis {

	class Time {
	public:
		Time() = delete;
		Time(const Time&) = delete;
		Time(Time&&) = delete;
		
		static void Update();
		static float GetDelta() { return (float)s_Delta; }

	private:
		static Timer s_Timer;
		static double s_LastTime;
		static double s_Delta;
	};
}