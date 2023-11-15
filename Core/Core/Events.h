#pragma once
#include "Macros.h"
#include "KeyCodes.h"

namespace Orchis {

	enum class EventType
	{
		UNDEFINED_EVENT = -1, 
		MOUSE_BUTTON_DOWN_EVENT, MOUSE_BUTTON_UP_EVENT, MOUSE_MOVE_EVENT, MOUSE_SCROLL_EVENT, 
		KEY_DOWN_EVENT, KEY_UP_EVENT,
		WINDOW_RESIZE_EVENT, WINDOW_CLOSE_EVENT, WINDOW_MINIMIZE_EVENT
	};

	class Event
	{
	public:
		void Consume() { m_Consumed = true; };
		bool Consumed() { return m_Consumed; }
		EventType GetEventType() const { return m_Type; };
	protected:
		Event(EventType type) : m_Type(type) {};
		virtual ~Event() = default;
	private:
		bool m_Consumed = false;
		EventType m_Type;
	};

	class MouseButtonDownEvent : public Event
	{
	public:
		MouseButtonDownEvent(Key button, float posX, float posY)
			: Event(EventType::MOUSE_BUTTON_DOWN_EVENT), m_Button(button), m_PosX(posX), m_PosY(posY) {}
		~MouseButtonDownEvent() = default;

		float GetX() const { return m_PosX; }
		float GetY() const { return m_PosY; }
		Key GetButton() const { return m_Button; }
		std::pair<float, float> GetPosition() const { return std::pair<float, float>(m_PosX, m_PosY); }
	private:
		Key m_Button;
		float m_PosX, m_PosY;
	};

	class MouseButtonUpEvent : public Event
	{
	public:
		MouseButtonUpEvent(Key button, float posX, float posY)
			: Event(EventType::MOUSE_BUTTON_UP_EVENT), m_Button(button), m_PosX(posX), m_PosY(posY) {}
		~MouseButtonUpEvent() = default;

		float GetX() const { return m_PosX; }
		float GetY() const { return m_PosY; }
		Key GetButton() const { return m_Button; }
		std::pair<float, float> GetPosition() const { return std::pair<float, float>(m_PosX, m_PosY); }
	private:
		Key m_Button;
		float m_PosX, m_PosY;
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float posX, float posY)
			: Event(EventType::MOUSE_MOVE_EVENT), m_PosX(posX), m_PosY(posY) {}
		
		~MouseMoveEvent() = default;

		float GetX() const { return m_PosX; }
		float GetY() const { return m_PosY; }
		std::pair<float, float> GetPosition() const { return std::pair<float, float>(m_PosX, m_PosY); }
	private:
		float m_PosX, m_PosY;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float delta)
			: Event(EventType::MOUSE_SCROLL_EVENT), m_Delta(delta) {}
		~MouseScrollEvent() = default;

		float GetDelta() const { return m_Delta; }
	private:
		float m_Delta;
	};

	class KeyDownEvent : public Event
	{
	public:
		KeyDownEvent(Key key)
			: Event(EventType::KEY_DOWN_EVENT), m_Key(key) {}
		~KeyDownEvent() = default;

		Key GetKey() const { return m_Key; }
	private:
		Key m_Key;
	};

	class KeyUpEvent : public Event
	{
	public:
		KeyUpEvent(Key key)
			: Event(EventType::KEY_UP_EVENT), m_Key(key) {}
		~KeyUpEvent() = default;

		Key GetKey() const { return m_Key; }
	private:
		Key m_Key;
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(float width, float height)
			: Event(EventType::WINDOW_RESIZE_EVENT), m_Width(width), m_Height(height) {}
		~WindowResizeEvent() = default;

		float GetWidth() const { return m_Width; }
		float GetHeight() const { return m_Height; }
		std::pair<float, float> GetDimensions() const { return { m_Width, m_Height }; }
	private:
		float m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent()
			: Event(EventType::WINDOW_CLOSE_EVENT) {}
		~WindowCloseEvent() = default;
	};

	class WindowMinimizeEvent : public Event
	{
	public:
		WindowMinimizeEvent()
			: Event(EventType::WINDOW_MINIMIZE_EVENT) {}
		~WindowMinimizeEvent() = default;
	};
}