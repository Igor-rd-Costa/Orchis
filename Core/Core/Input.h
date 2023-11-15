#pragma once
#include "Macros.h"
#include "KeyCodes.h"

namespace Orchis {
	class Window;
	struct CursorPos
	{
		long x, y;
	};

	class Input
	{
	public:
		static bool KeyPressed(Key key);
		static bool KeyReleased(Key key);
		static bool IsKeyDown(Key key);

		//implemented in platform specific files
		static CursorPos GetCursorPos();
		static void CenterCursor();
		static void ClipCursor();
		static void ToggleCursorVisibility();
	private:
		static std::array<char, 96> s_InputBuffer;
		static bool s_IsCursorVisible;
		static bool s_IsCursorClipped;
		friend class EventDispatcher;
		friend class Window;

		static void RegisterKeyDown(Key key);
		static void RegisterKeyUp(Key key);
		static void UpdateState();
	};
}