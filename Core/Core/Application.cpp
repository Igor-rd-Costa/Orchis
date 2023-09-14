#include "OrchisPCH.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "EventDispatcher.h"

namespace Orchis {

	Scope<Window> Application::s_Window;
	Application* Application::s_Instance;
	bool Application::s_IsRunning = true;
	Logger Application::s_Logger{ "App" };

	Application::Application()
	{
		if (s_Instance)
		{
			s_Logger.Error("Application recreation.");
			OC_ASSERT(false);
		}
		Log::Init();
		s_Window = Window::Create("Orchis Engine");
		Renderer::Init();

		EventDispatcher::RegisterWindowCloseCallbackFn(OnWindowClose);
		
		s_Instance = this;
	}

	void Application::Run()
	{
		s_Window->Update();
		while (s_IsRunning)
		{
			Time::Update();
			OnUpdate();
			s_Window->Update();
		}
	}
	
	Application::~Application()
	{
		Renderer::Shutdown();
	}
}