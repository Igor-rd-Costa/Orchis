#include "Console.h"
#include "Core/IO/Logger.h"

#ifdef OC_WINDOWS
#include "Core/Platform/Windows/OrchisWindows.h"
#elif OC_LINUX

#elif OC_APPLE

#endif

namespace Orchis {

	void Console::Open()
	{
		OC_ASSERT((s_Instance == nullptr));
#ifdef OC_WINDOWS
		s_Instance = new WindowsConsole();
#elif OC_LINUX
		
#elif OC_APPLE

#endif
		OC_ASSERT((s_Instance != nullptr));
		g_CoreLogger.SetStandardOutput(s_Instance);
	}

	void Console::Close()
	{
		if (s_Instance != nullptr)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	WeakRef<Console> Console::Instance()
	{
		return s_Instance;
	}

}