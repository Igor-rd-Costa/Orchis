#pragma once
#include "Window.h"

namespace Orchis {

	class WindowManager
	{
	public:
#undef CreateWindow
		static WeakRef<Window> CreateWindow(const OHandle& parent);
		static void DestroyWindow(const OHandle& handle);
		static WeakRef<Window> GetWindow(const OHandle& handle);
		static void UpdateWindows();
	private:
		static inline std::unordered_map<OHandle, Window*> s_RegisteredWindows;
	};

}