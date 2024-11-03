#pragma once
#include "Macros.h"
#include "IO/Logger.h"
#ifndef OC_HEADLESS_BUILD
#include "../Src/Graphics/Window.h"
#include "../Src/Graphics/Renderer.h"
#endif // !OC_HEADLESS_BUILD


namespace Orchis {

	class OC_API Application 
	{
	public:
		void Run();
	protected:
		Application();
		virtual ~Application();
	private:
		static inline WeakRef<Application> s_Instance = nullptr;
		Logger m_Logger;
		bool m_IsRunning = false;
#ifndef OC_HEADLESS_BUILD
		WeakRef<Window> m_MainWindow = nullptr;
		Renderer m_Renderer;
#endif // !OC_HEADLESS_BUILD

	};

}