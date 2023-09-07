#pragma once
#include "Macros.h"

namespace Orchis {
	
	class ORCHIS_API Log
	{
	public:
		static void Init();
	};

	enum LogLevel
	{
		LOG_LEVEL_TRACE, LOG_LEVEL_INFO, LOG_LEVEL_WARNING, LOG_LEVEL_ERROR
	};

	class Logger
	{
	private:
		std::string m_Name;
	public:
		
		Logger(const std::string& name, LogLevel level = LOG_LEVEL_TRACE);
		~Logger() = default;

		void Trace(std::string_view msg);
		void Info(std::string_view msg);
		void Warning(std::string_view msg);
		void Error(std::string_view msg);
	};
}