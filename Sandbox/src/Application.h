#pragma once
#include <Orchis.h>

namespace Orchis {

	class Application
	{
	public:
		Application();
		~Application();

		void Run();
	private:
		static void OnMouseDown(MouseButtonDownEvent* event);
		static void OnMouseUp(MouseButtonUpEvent* event);
		static void OnMouseMove(MouseMoveEvent* event);
		static void OnMouseScroll(MouseScrollEvent* event);
		static void OnKeyDown(KeyDownEvent* event);
		static void OnKeyUp(KeyUpEvent* event);
		static void OnWindowResize(WindowResizeEvent* event);
		static void OnWindowClose(WindowCloseEvent* event);
		static void OnWindowMinimize(WindowMinimizeEvent* event);
		static bool m_IsRunning;
		static Ref<Window> m_Window;
	};

}