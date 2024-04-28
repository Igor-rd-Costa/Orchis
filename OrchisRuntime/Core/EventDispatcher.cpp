#include "EventDispatcher.h"
namespace Orchis {

	EventCallbackFn<MouseScrollEvent> EventDispatcher::s_MouseScrollCallback = [](MouseScrollEvent* event) {};
	EventCallbackFn<KeyDownEvent> EventDispatcher::s_KeyDownCallback = [](KeyDownEvent* event) {};
	EventCallbackFn<KeyUpEvent> EventDispatcher::s_KeyUpCallback = [](KeyUpEvent* event) {};
	EventCallbackFn<WindowResizeEvent> EventDispatcher::s_WindowResizeCallback = [](WindowResizeEvent* event) {};
	EventCallbackFn<WindowCloseEvent> EventDispatcher::s_WindowCloseCallback = [](WindowCloseEvent* event) {};
	EventCallbackFn<WindowMinimizeEvent> EventDispatcher::s_WindowMinimizeCallback = [](WindowMinimizeEvent* event) {};

	std::vector<EventCallbackFn<MouseLeaveEvent>> EventDispatcher::s_MouseLeaveCallbacks = {};
	std::vector<EventCallbackFn<MouseMoveEvent>> EventDispatcher::s_MouseMoveCallbacks = {};
	std::vector <EventCallbackFn<MouseButtonDownEvent>> EventDispatcher::s_MouseButtonDownCallbacks = {};
	std::vector <EventCallbackFn<MouseButtonUpEvent>> EventDispatcher::s_MouseButtonUpCallbacks = {};

	void EventDispatcher::OnMouseMove(EventCallbackFn<MouseMoveEvent> callback)
	{
		s_MouseMoveCallbacks.push_back(callback);
	}

	void EventDispatcher::OnMouseLeave(EventCallbackFn<MouseLeaveEvent> callback)
	{
		s_MouseLeaveCallbacks.push_back(callback);
	}

	void EventDispatcher::OnMouseButtonDown(EventCallbackFn<MouseButtonDownEvent> callback)
	{
		s_MouseButtonDownCallbacks.push_back(callback);
	}

	void EventDispatcher::OnMouseButtonUp(EventCallbackFn<MouseButtonUpEvent> callback)
	{
		s_MouseButtonUpCallbacks.push_back(callback);
	}

}