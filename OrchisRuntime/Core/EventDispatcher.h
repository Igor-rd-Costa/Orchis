#pragma once
#include <queue>
#include "Events.h"
#include "Input.h"
#include <iostream>

namespace Orchis {

	template<typename T>
	using EventCallbackFn = void(*)(T* event);

	class EventDispatcher
	{
	public:
		static void OnMouseLeave(EventCallbackFn<MouseLeaveEvent> callback);
		static void OnMouseMove(EventCallbackFn<MouseMoveEvent> callback);
		static void OnMouseButtonDown(EventCallbackFn<MouseButtonDownEvent> callback);
		static void OnMouseButtonUp(EventCallbackFn<MouseButtonUpEvent> callback);

		inline static void RegisterMouseScrollCallbackFn(EventCallbackFn<MouseScrollEvent> callback) { s_MouseScrollCallback = callback; }
		inline static void RegisterKeyDownCallbackFn(EventCallbackFn<KeyDownEvent> callback) { s_KeyDownCallback = callback; }
		inline static void RegisterKeyUpCallbackFn(EventCallbackFn<KeyUpEvent> callback) { s_KeyUpCallback = callback; }
		inline static void RegisterWindowResizeCallbackFn(EventCallbackFn<WindowResizeEvent> callback) { s_WindowResizeCallback = callback; }
		inline static void RegisterWindowCloseCallbackFn(EventCallbackFn<WindowCloseEvent> callback) { s_WindowCloseCallback = callback; }
		inline static void RegisterWindowMinimizeCallbackFn(EventCallbackFn<WindowMinimizeEvent> callback) { s_WindowMinimizeCallback = callback; }
	
	
	
		template<typename T>
			requires std::is_same_v<T, MouseButtonDownEvent> || std::is_same_v<T, MouseButtonUpEvent> || std::is_same_v<T, MouseMoveEvent> || std::is_same_v<T, MouseScrollEvent>
		|| std::is_same_v<T, MouseLeaveEvent>
			|| std::is_same_v<T, KeyDownEvent> || std::is_same_v<T, KeyUpEvent>
			|| std::is_same_v<T, WindowResizeEvent> || std::is_same_v<T, WindowCloseEvent> || std::is_same_v<T, WindowMinimizeEvent>
			static void Dispatch(T&& event)
		{
			switch (event.GetEventType())
			{
			case EventType::MOUSE_BUTTON_DOWN_EVENT:
			{
				MouseButtonDownEvent* eventPtr = reinterpret_cast<MouseButtonDownEvent*>(&event);
				Input::RegisterKeyDown(eventPtr->GetButton());
				for (EventCallbackFn<MouseButtonDownEvent> callback : s_MouseButtonDownCallbacks)
					callback(eventPtr);
			} break;
			case EventType::MOUSE_BUTTON_UP_EVENT:
			{
				MouseButtonUpEvent* eventPtr = reinterpret_cast<MouseButtonUpEvent*>(&event);
				Input::RegisterKeyUp(eventPtr->GetButton());
				for (EventCallbackFn<MouseButtonUpEvent> callback : s_MouseButtonUpCallbacks)
					callback(eventPtr);
			} break;
			case EventType::MOUSE_MOVE_EVENT:
			{
				MouseMoveEvent* eventPtr = reinterpret_cast<MouseMoveEvent*>(&event);
				for (const EventCallbackFn<MouseMoveEvent>& callback : s_MouseMoveCallbacks)
					callback(eventPtr);
			} break;
			case EventType::MOUSE_SCROLL_EVENT:
			{
				s_MouseScrollCallback(reinterpret_cast<MouseScrollEvent*>(&event));
			} break;
			case EventType::MOUSE_LEAVE_EVENT:
			{
				MouseLeaveEvent* eventPtr = reinterpret_cast<MouseLeaveEvent*>(&event);
				for (EventCallbackFn<MouseLeaveEvent> callback : s_MouseLeaveCallbacks)
					callback(eventPtr);
			}
			case EventType::KEY_DOWN_EVENT:
			{
				KeyDownEvent* eventPtr = reinterpret_cast<KeyDownEvent*>(&event);
				Input::RegisterKeyDown(eventPtr->GetKey());
				s_KeyDownCallback(eventPtr);
			} break;
			case EventType::KEY_UP_EVENT:
			{
				KeyUpEvent* eventPtr = reinterpret_cast<KeyUpEvent*>(&event);
				Input::RegisterKeyUp(eventPtr->GetKey());
				s_KeyUpCallback(eventPtr);
			} break;
			case EventType::WINDOW_RESIZE_EVENT:
			{
				s_WindowResizeCallback(reinterpret_cast<WindowResizeEvent*>(&event));
			} break;
			case EventType::WINDOW_MINIMIZE_EVENT:
			{
				s_WindowMinimizeCallback(reinterpret_cast<WindowMinimizeEvent*>(&event));
			} break;
			case EventType::WINDOW_CLOSE_EVENT:
			{
				s_WindowCloseCallback(reinterpret_cast<WindowCloseEvent*>(&event));
			} break;
			}
		}
	private:
		EventDispatcher() = delete;
		EventDispatcher(const EventDispatcher&) = delete;
		EventDispatcher(EventDispatcher&&) = delete;
	private:
		static std::vector<EventCallbackFn<MouseLeaveEvent>> s_MouseLeaveCallbacks;
		static std::vector<EventCallbackFn<MouseMoveEvent>> s_MouseMoveCallbacks;
		static std::vector <EventCallbackFn<MouseButtonDownEvent>> s_MouseButtonDownCallbacks;
		static std::vector <EventCallbackFn<MouseButtonUpEvent>> s_MouseButtonUpCallbacks;

		static EventCallbackFn<MouseScrollEvent> s_MouseScrollCallback;
		static EventCallbackFn<KeyDownEvent> s_KeyDownCallback;
		static EventCallbackFn<KeyUpEvent> s_KeyUpCallback;
		static EventCallbackFn<WindowResizeEvent> s_WindowResizeCallback;
		static EventCallbackFn<WindowCloseEvent> s_WindowCloseCallback;
		static EventCallbackFn<WindowMinimizeEvent> s_WindowMinimizeCallback;
	};
}
