#pragma once
#include <string_view>
#include <format>
#include "OutputBase.h"

namespace Orchis {

	enum OC_API LogLevel {
		LEVEL_TRACE, LEVEL_INFO, LEVEL_WARNING, LEVEL_ERROR, LEVEL_FATAL
	};

	
	class OC_API Logger
	{
	public:
		Logger();

		void SetLevel(LogLevel level);
		[[nodiscard]] LogLevel GetLevel() const;
		void SetPattern(const char* str);
		void SetStandardOutput(const OutputBase* outputStream);
		const OutputBase* GetStandardOutput() const;

		void WriteLine(std::string_view msg) const
		{
			if (m_StandartOutput == nullptr)
				return;
			switch (m_LogLevel)
			{
			case LEVEL_TRACE:
				Trace(msg);
				break;
			case LEVEL_INFO:
				Info(msg);
				break;
			case LEVEL_WARNING:
				Warning(msg);
				break;
			case LEVEL_ERROR:
				Error(msg);
				break;
			case LEVEL_FATAL:
				Fatal(msg);
				break;
			}
		}
		template<typename... Args>
		void WriteLine(std::string_view msg, Args... args) const
		{
			if (m_StandartOutput == nullptr)
				return;
			std::string_view message = std::vformat(msg, std::make_format_args(std::forward<Args&>(args)...));
			switch (m_LogLevel)
			{
			case LEVEL_TRACE:
				Trace(message);
				break;
			case LEVEL_INFO:
				Info(message);
				break;
			case LEVEL_WARNING:
				Warning(message);
				break;
			case LEVEL_ERROR:
				Error(message);
				break;
			case LEVEL_FATAL:
				Fatal(message);
				break;
			}
		}

		void Trace(std::string_view msg) const
		{
			if (m_LogLevel <= LEVEL_TRACE && m_StandartOutput)
				m_StandartOutput->WriteLineImpl(msg);
		}
		
		template<typename... Args>
		void Trace(std::string_view msg, Args... args) const
		{
			if (m_LogLevel <= LEVEL_TRACE && m_StandartOutput)
				m_StandartOutput->WriteLineImpl(std::vformat(msg, std::make_format_args(std::forward<Args&>(args)...)));
		}
		
		void Info(std::string_view msg) const
		{
			if (m_LogLevel <= LEVEL_INFO && m_StandartOutput)
				m_StandartOutput->WriteLineImpl(msg);
		}

		template<typename... Args>
		void Info(std::string_view msg, Args... args) const
		{
			if (m_LogLevel <= LEVEL_INFO && m_StandartOutput)
				m_StandartOutput->WriteLineImpl(std::vformat(msg, std::make_format_args(std::forward<Args&>(args)...)));
		}
		
		void Warning(std::string_view msg) const
		{
			if (m_LogLevel <= LEVEL_WARNING && m_StandartOutput)
				m_StandartOutput->WriteLineImpl(msg);
		}
		
		template<typename... Args>
		void Warning(std::string_view msg, Args... args) const
		{
			if (m_LogLevel <= LEVEL_WARNING && m_StandartOutput)
				m_StandartOutput->WriteLineImpl(std::vformat(msg, std::make_format_args(std::forward<Args&>(args)...)));
		}
		
		void Error(std::string_view msg) const
		{
			if (m_LogLevel <= LEVEL_ERROR && m_StandartOutput)
				m_StandartOutput->WriteLineImpl(msg);
		}
		
		template<typename... Args>
		void Error(std::string_view msg, Args... args) const
		{
			if (m_LogLevel <= LEVEL_ERROR && m_StandartOutput)
				m_StandartOutput->WriteLineImpl(std::vformat(msg, std::make_format_args(std::forward<Args&>(args)...)));
		}

		void Fatal(std::string_view msg) const
		{
			if (m_LogLevel <= LEVEL_FATAL && m_StandartOutput)
				m_StandartOutput->WriteLineImpl(msg);
		}

		template<typename... Args>
		void Fatal(std::string_view msg, Args... args) const
		{
			if (m_LogLevel <= LEVEL_FATAL && m_StandartOutput)
				m_StandartOutput->WriteLineImpl(std::vformat(msg, std::make_format_args(std::forward<Args&>(args)...)));
		}
		
	private:
		Logger(const Logger&) = delete;
		Logger(Logger&&) = delete;
	private:
		const OutputBase* m_StandartOutput = nullptr;
		LogLevel m_LogLevel = LEVEL_TRACE;
		const char* m_Pattern = "";
	};

	extern Logger g_CoreLogger;

#ifdef OC_DEBUG_BUILD
#define OC_LOG_TRACE(msg, ...) g_CoreLogger.Trace(msg, __VA_ARGS__);
#define OC_LOG_INFO(msg, ...) g_CoreLogger.Trace(msg, __VA_ARGS__);
#define OC_LOG_WARNING(msg, ...) g_CoreLogger.Trace(msg, __VA_ARGS__);
#define OC_LOG_ERROR(msg, ...) g_CoreLogger.Trace(msg, __VA_ARGS__);
#define OC_LOG_FATAL(msg, ...) g_CoreLogger.Trace(msg, __VA_ARGS__);
#elif OC_RELEASE_BUILD
#define OC_LOG_TRACE(msg, ...)
#define OC_LOG_INFO(msg, ...)
#define OC_LOG_WARNING(msg, ...) g_CoreLogger.Trace(msg, __VA_ARGS__);
#define OC_LOG_ERROR(msg, ...) g_CoreLogger.Trace(msg, __VA_ARGS__);
#define OC_LOG_FATAL(msg, ...) g_CoreLogger.Trace(msg, __VA_ARGS__);
#elif OC_DIST_BUILD
#define OC_LOG_TRACE(msg, ...)
#define OC_LOG_INFO(msg, ...)
#define OC_LOG_WARNING(msg, ...)
#define OC_LOG_ERROR(msg, ...)
#define OC_LOG_FATAL(msg, ...)
#endif
}