#pragma once
#include "Macros.h"

namespace Orchis {
	
	enum TimeFormat
	{
		TIME_FORMAT_SECONDS, TIME_FORMAT_MILISECONDS
	};

	class Timer
	{
	public:	
		Timer(const char* name = "Unnamed");
		~Timer();

		double GetTime(TimeFormat format);
	protected:
	private:
		const char* m_Name;
		_int64 m_Start;
	};
}