#include "Application.h"
#include "Renderer/Renderer.h"
#include "EventDispatcher.h"
#include "VFS/FS.h"

namespace Orchis {

	bool Application::s_IsRunning = true;
	Logger Application::s_Logger{ "App" };
	RenderContext* Application::s_RenderContext = nullptr;

	void Application::Run()
	{
		Application::GetWindow()->Update();
		while (s_IsRunning)
		{
			Time::Update();
			Application::GetWindow()->Update();
		}
	}

	void Application::Update()
	{
		Time::Update();
		Application::GetWindow()->Update();
	}

	void* Application::CreateRenderContext(void* parentWindow)
	{
		s_RenderContext = new RenderContext(parentWindow);
		return s_RenderContext;
	}

	void Application::Init(void* parentWindow)
	{
		Log::Init();
		FS::Init();

		CreateRenderContext(parentWindow);
		EventDispatcher::RegisterWindowCloseCallbackFn(OnWindowClose);
	}

	void Application::Shutdown()
	{
		if (s_RenderContext)
			delete s_RenderContext;
	}
}