#include "Application.h"

#include <iostream>
#include <string>
#include <bitset>
#include <vector>
namespace Orchis {

	bool Application::m_IsRunning = true;
	Ref<Window> Application::m_Window;

	std::vector<int> vector;

	Application::Application()
	{
		Timer timer("App Init");
		m_Window = Window::Create("Window");
		EventDispatcher::RegisterKeyDownCallbackFn(OnKeyDown);
		EventDispatcher::RegisterKeyUpCallbackFn(OnKeyUp);
		EventDispatcher::RegisterMouseButtonDownCallbackFn(OnMouseDown);
		EventDispatcher::RegisterMouseButtonUpCallbackFn(OnMouseUp);
		EventDispatcher::RegisterMouseMoveCallbackFn(OnMouseMove);
		EventDispatcher::RegisterMouseScrollCallbackFn(OnMouseScroll);
		EventDispatcher::RegisterWindowResizeCallbackFn(OnWindowResize);
		EventDispatcher::RegisterWindowMinimizeCallbackFn(OnWindowMinimize);
		EventDispatcher::RegisterWindowCloseCallbackFn(OnWindowClose);
	}

	Application::~Application()
	{
		
	}

	void Orchis::Application::Run()
	{
		m_Window->Update();
		while (m_IsRunning)
		{


			Input::UpdateState();
			m_Window->Update();
		}
	}

	void Application::OnMouseDown(MouseButtonDownEvent* event)
	{
		
	}

	void Application::OnMouseUp(MouseButtonUpEvent* event)
	{
	
	}

	void Application::OnMouseMove(MouseMoveEvent* event)
	{
		
	}

	void Application::OnMouseScroll(MouseScrollEvent* event)
	{
		
	}

	void Application::OnKeyDown(KeyDownEvent* event)
	{

	}

	void Application::OnKeyUp(KeyUpEvent* event)
	{
		
	}

	void Application::OnWindowResize(WindowResizeEvent* event)
	{
		
	}

	void Application::OnWindowClose(WindowCloseEvent* event)
	{
		m_IsRunning = false;
	}

	void Application::OnWindowMinimize(WindowMinimizeEvent* event)
	{
		
	}

}


