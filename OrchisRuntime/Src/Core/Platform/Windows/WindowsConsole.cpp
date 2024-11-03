#include <string>
#include "OrchisWindows.h"

namespace Orchis {

	WindowsConsole::WindowsConsole()
		: m_StdInputHandle(NULL), m_StdOutputHandle(NULL), m_StdErrorHandle(NULL)
	{
		BOOL result = AllocConsole();
		if (result == 0)
		{
			DWORD errorCode = GetLastError();
			if (errorCode != 5)
			{
				OC_ASSERT(false);
			}
			return;
		}

		m_StdInputHandle = GetStdHandle(STD_INPUT_HANDLE);
		m_StdOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		m_StdErrorHandle = GetStdHandle(STD_ERROR_HANDLE);

		OC_ASSERT((m_StdInputHandle != INVALID_HANDLE_VALUE));
		OC_ASSERT((m_StdOutputHandle != INVALID_HANDLE_VALUE));
		OC_ASSERT((m_StdErrorHandle != INVALID_HANDLE_VALUE));
	}

	WindowsConsole::~WindowsConsole()
	{
		FreeConsole();
	}

	void WindowsConsole::WriteLineImpl(std::string_view msg) const
	{
		WriteImpl(std::string(msg) + "\n");
	}

	void WindowsConsole::WriteImpl(std::string_view msg) const
	{
		WriteConsoleA(m_StdOutputHandle, msg.data(), (DWORD)msg.length(), NULL, NULL);
	}

}