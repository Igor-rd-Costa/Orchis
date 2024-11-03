#include "Logger.h"
#include "Console.h"

namespace Orchis {

	Logger g_CoreLogger;

	Logger::Logger()
		: m_StandartOutput(Console::Instance()) {}

	void Logger::SetLevel(LogLevel level)
	{
		m_LogLevel = level;
	}

	LogLevel Logger::GetLevel() const {
		return m_LogLevel;
	}

	void Logger::SetPattern(const char* pattern)
	{
		m_Pattern = pattern;
	}

	void Logger::SetStandardOutput(const OutputBase* outputStream)
	{
		m_StandartOutput = outputStream;
	}

	const OutputBase* Logger::GetStandardOutput() const
	{
		return m_StandartOutput;
	}
}