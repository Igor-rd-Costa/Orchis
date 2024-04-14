#include "EventDispatcher.h"
namespace Orchis {

	EventCallbackFn<MouseButtonDownEvent> EventDispatcher::s_MouseButtonDownCallback = [](MouseButtonDownEvent* event){};
	EventCallbackFn<MouseButtonUpEvent>	EventDispatcher::s_MouseButtonUpCallback = [](MouseButtonUpEvent* event) {};
	EventCallbackFn<MouseMoveEvent>	EventDispatcher::s_MouseMoveCallback = [](MouseMoveEvent* event) {};
	EventCallbackFn<MouseScrollEvent> EventDispatcher::s_MouseScrollCallback = [](MouseScrollEvent* event) {};
	EventCallbackFn<KeyDownEvent> EventDispatcher::s_KeyDownCallback = [](KeyDownEvent* event) {};
	EventCallbackFn<KeyUpEvent> EventDispatcher::s_KeyUpCallback = [](KeyUpEvent* event) {};
	EventCallbackFn<WindowResizeEvent> EventDispatcher::s_WindowResizeCallback = [](WindowResizeEvent* event) {};
	EventCallbackFn<WindowCloseEvent> EventDispatcher::s_WindowCloseCallback = [](WindowCloseEvent* event) {};
	EventCallbackFn<WindowMinimizeEvent> EventDispatcher::s_WindowMinimizeCallback = [](WindowMinimizeEvent* event) {};

}