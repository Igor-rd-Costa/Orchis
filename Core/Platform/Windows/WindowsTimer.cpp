#include "OrchisPCH.h"
#include "Timer.h"
#include <Windows.h>

namespace Orchis {
	
	static LARGE_INTEGER frequency;
	Timer::Timer(const char* name)
		: m_Name(name)
	{
		QueryPerformanceFrequency(&frequency);
		LARGE_INTEGER tempStart;
		QueryPerformanceCounter(&tempStart);
		m_Start = tempStart.QuadPart;
	}

	Timer::~Timer()
	{
		std::cout << "[" << m_Name << " Timer]" << GetTime(TIME_FORMAT_SECONDS) << "s\n";
	}

	double Timer::GetTime(TimeFormat format)
	{
		static LARGE_INTEGER end;

		switch (format)
		{
		case TIME_FORMAT_SECONDS:
		{
			QueryPerformanceCounter(&end);
			return (end.QuadPart - m_Start) / (double)frequency.QuadPart;
		}
		case TIME_FORMAT_MILISECONDS: return GetTime(TIME_FORMAT_SECONDS) * 1000;
		}

		return 0.0;
	}
}