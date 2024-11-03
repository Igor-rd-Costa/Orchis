#include "Platform/Windows/WindowsWindow.h"
#include "WindowManager.h"

namespace Orchis {

	#undef CreateWindow // windows.h macro
	WeakRef<Window> WindowManager::CreateWindow(const OHandle& parent)
	{
		Window* window = nullptr;
#ifdef OC_WINDOWS
		window = new WindowsWindow(parent);
#elif OC_LINUX
	
#elif OC_APPLE
	
#endif
		OC_ASSERT((window != nullptr));
		s_RegisteredWindows[window->Handle()] = window;
		return window;
	}

	void WindowManager::DestroyWindow(const OHandle& handle)
	{
		auto it = s_RegisteredWindows.find(handle);
		if (it == s_RegisteredWindows.end())
			return;

#ifdef OC_WINDOWS
		delete reinterpret_cast<WindowsWindow*>(it->second);
#elif OC_LINUX
		OC_ASSERT(false);
#elif OC_APPLE
		OC_ASSERT(false);
#elif
		OC_ASSERT(false);
#endif
		s_RegisteredWindows.erase(handle);
	}

	WeakRef<Window> WindowManager::GetWindow(const OHandle& handle)
	{
		auto it = s_RegisteredWindows.find(handle);
		if (it != s_RegisteredWindows.end())
		{
			return it->second;
		}
		return nullptr;
	}

	void WindowManager::UpdateWindows()
	{
		for (const auto& pair : s_RegisteredWindows)
		{
			OC_ASSERT((pair.second != nullptr));
			pair.second->Update();
		}
	}
}