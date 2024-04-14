#pragma once
#include "Macros.h"
#include "Window.h"
#include "Events.h"
#include "../Debug/Logger.h"
#include "OrchisTime.h"
#include "Renderer/RenderContext.h"
#include <thread>

namespace Orchis {
	class OC_API Application
	{
	public:
		Application() = delete;
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		~Application() = delete;

		static void Init(void* parentWindow);
		static void Shutdown();
		static void Run();
		static void Update();

		static inline Window* GetWindow() 
		{
			if (s_RenderContext)
				return s_RenderContext->GetWindow(); 

			return nullptr;
		}
		static inline RendererAPI* GetRenderAPI() { return s_RenderContext->GetAPI(); }

		static bool s_IsRunning;
	private:
		static void OnWindowClose(WindowCloseEvent* event) { s_IsRunning = false; }
	private:
		static RenderContext* s_RenderContext;
		static Logger s_Logger;
	};
}