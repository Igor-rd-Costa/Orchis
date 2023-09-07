#include "OrchisPCH.h"
#include "Input.h"
#include <Windows.h>
#include "Window.h"
#include "Application.h"
namespace Orchis {

	CursorPos Input::GetCursorPos()
	{
		static POINT point;
		::GetCursorPos(&point);
		return { point.x, point.y };
	}

	void Input::ClipCursor()
	{
		if (!s_IsCursorClipped)
		{
			static RECT rect;
			GetWindowRect(*reinterpret_cast<HWND*>(Application::GetWindow()->GetHandle()), &rect);
			::ClipCursor(&rect);
		}
		else
		{
			::ClipCursor(NULL);
		}
		s_IsCursorClipped = !s_IsCursorClipped;
	}

	void Input::ToggleCursorVisibility()
	{
		ShowCursor(!s_IsCursorVisible);
		s_IsCursorVisible = !s_IsCursorVisible;
	}

	void Input::CenterCursor()
	{
		static RECT rect;
		GetWindowRect(*reinterpret_cast<HWND*>(Application::GetWindow()->GetHandle()), &rect);
		::SetCursorPos(((rect.left + rect.right) / 2), ((rect.top + rect.bottom) / 2));
	}
}