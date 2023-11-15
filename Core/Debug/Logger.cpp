#include "Logger.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Orchis {

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
	}

	Logger::Logger(const std::string& name, LogLevel level)
		: m_Name(name)
	{
		spdlog::level::level_enum logLevel = spdlog::level::trace;
		switch (level)
		{
			case LOG_LEVEL_TRACE: logLevel = spdlog::level::trace; break;
			case LOG_LEVEL_INFO: logLevel = spdlog::level::info; break;
			case LOG_LEVEL_WARNING: logLevel = spdlog::level::warn; break;
			case LOG_LEVEL_ERROR: logLevel = spdlog::level::err; break;
		}
		
		auto logger = spdlog::stdout_color_mt(name);
		logger->set_level(logLevel);

	}

	void Logger::Trace(const std::string& msg)
	{
		spdlog::get(m_Name)->trace(msg);
	}

	void Logger::Info(const std::string& msg)
	{
		spdlog::get(m_Name)->info(msg);
	}

	void Logger::Warning(const std::string& msg)
	{
		spdlog::get(m_Name)->warn(msg);
	}

	void Logger::Error(const std::string& msg)
	{
		spdlog::get(m_Name)->error(msg);
	}

}