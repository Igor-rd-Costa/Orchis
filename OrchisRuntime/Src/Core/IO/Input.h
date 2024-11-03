#pragma once
#include "../Src/Core/Macros.h"

namespace Orchis {

	enum OC_API Key {
		KEY_ESC, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
		KEY_PRINT_SCREEN, KEY_SCROLL_LOCK, KEY_PAUSE, KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
		KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_HYPHEN, KEY_EQUAL, KEY_COMMA, KEY_PERIOD, KEY_BACKSPACE, KEY_ENTER, KEY_LCTRL,
		KEY_RCTRL, KEY_LALT, KEY_RALT, KEY_LSHIFT, KEY_RSHIFT, KEY_LSYSTEM, KEY_RSYSTEM, KEY_CAPS_LOCK, KEY_TAB, KEY_CLEAR, KEY_SPACE,
		KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q,
		KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, KEY_ARROW_UP, KEY_ARROW_LEFT, KEY_ARROW_DOWN, KEY_ARROW_RIGHT,
		KEY_NUMPAD0, KEY_NUMPAD1, KEY_NUMPAD2, KEY_NUMPAD3, KEY_NUMPAD4, KEY_NUMPAD5, KEY_NUMPAD6, KEY_NUMPAD7,
		KEY_NUMPAD8, KEY_NUMPAD9, KEY_NUMLOCK, KEY_NUMPAD_DIVIDE, KEY_NUMPAD_MULTIPLY, KEY_NUMPAD_MINUS, KEY_NUMPAD_PLUS, KEY_NUMPAD_ENTER,
		KEY_NUMPAD_COMMA, KEY_INSERT, KEY_HOME, KEY_PAGE_UP, KEY_PAGE_DOWN, KEY_END, KEY_DELETE,
		KEY_LMOUSE, KEY_MMOUSE, KEY_RMOUSE, KEY_XMOUSE1, KEY_XMOUSE2, KEY_APPS,
		KEY_OEM1, KEY_OEM2, KEY_OEM3, KEY_OEM4, KEY_OEM5, KEY_OEM6, KEY_OEM7, KEY_OEM8, KEY_OEM_102, KEY_FSLASH, KEY_MAX_VALUE
	};

#define OC_INPUT_BUFFER_SIZE 14

	class OC_API Input
	{
	public:
		static bool WasKeyPressed(Key key);
		static bool IsKeyDown(Key key);
		static bool WasKeyReleased(Key key);

		/*
		Gonna leave the not static version of the functions with
		the shitty name cause the user should be using the static version
		inside game code.
		*/
		bool mWasKeyPressed(Key key) const;
		bool mIsKeyDown(Key key) const;
		bool mWasKeyReleased(Key key) const;

		void RegisterKeyPress(Key key);
		void RegisterKeyRelease(Key key);
		void ClearKeyPressState();
	private:
		static void SetStandartInput(const Input* input);
	private:
		int m_Buffer[OC_INPUT_BUFFER_SIZE];
		static inline const Input* s_ActiveInput = nullptr;
	};

}