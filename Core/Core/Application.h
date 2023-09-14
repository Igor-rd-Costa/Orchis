#pragma once
#include "Macros.h"
#include "Window.h"
#include "Events.h"
#include "../Debug/Logger.h"
#include "OrchisTime.h"

namespace Orchis {
	class ORCHIS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		virtual void OnUpdate() {};
		
		static inline Application& GetInstance() { return *s_Instance; }
		static inline Window* GetWindow() { return s_Window.get(); }

	private:
		static void OnWindowClose(WindowCloseEvent* event) { s_IsRunning = false; }
	private:
		static Logger s_Logger;
		static Scope<Window> s_Window;
		static Application* s_Instance;
		static bool s_IsRunning;
	};
}