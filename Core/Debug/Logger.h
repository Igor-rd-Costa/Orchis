#pragma once
#include "Macros.h"

namespace Orchis {
	
	class Log
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

		void Trace(const std::string& msg);
		void Info(const std::string& msg);
		void Warning(const std::string& msg);
		void Error(const std::string& msg);
	};
}