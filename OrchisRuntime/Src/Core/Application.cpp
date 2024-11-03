#include "Application.h"
#include "IO/Console.h"
#include "Graphics/WindowManager.h"

namespace Orchis {

	Application::Application()
	{
		OC_ASSERT((s_Instance == nullptr));
		s_Instance = this;
#if defined(OC_DEBUG_BUILD) || defined(OC_HEADLESS_BUILD)
		// open console
		Console::Open();
		m_Logger.SetStandardOutput(Console::Instance());
		m_Logger.WriteLine("Starting!");
#endif // OC_DEBUG_BUILD || OC_HEADLESS_BUILD
#ifndef OC_HEADLESS_BUILD
		m_MainWindow = WindowManager::CreateWindow(OHandle::Null());
		m_MainWindow->OnClose([this](Window* window){
			m_IsRunning = false;
			//probably wait frame end here
			m_Renderer.Shutdown();
		});
		m_Renderer.Init(RENDER_API_TYPE_VULKAN);
#endif // !OC_HEADLESS_BUILD

	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		OC_ASSERT(m_IsRunning == false);
		m_IsRunning = true;
		m_Renderer.Run();
		while (m_IsRunning)
		{
			WindowManager::UpdateWindows();
		}
	}

}