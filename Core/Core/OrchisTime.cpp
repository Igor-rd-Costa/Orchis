#include "OrchisTime.h"

namespace Orchis {

	Timer Time::s_Timer;
	double Time::s_LastTime = 0.0;
	double Time::s_Delta = 0.0;

	void Time::Update()
	{
		static double currentTime = 0.0;
		currentTime = s_Timer.GetTime(TIME_FORMAT_SECONDS);
		s_Delta = currentTime - s_LastTime;
		s_LastTime = currentTime;
	}

}