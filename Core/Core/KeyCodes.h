#pragma once
#include "Macros.h"

namespace Orchis {

	enum class ORCHIS_API Key
	{
		KEY_UNKNOWN = -1,
		KEY_W, /* 'W' key. */
		KEY_D, /* 'D' key. */
		MOUSE_LBUTTON,  /* Left mouse button. */
		MOUSE_RBUTTON,  /* Right mouse button. */
		MOUSE_MBUTTON,  /* Middle mouse button. */
		MOUSE_EXTRA1, /* Extra mouse button 1. */
		MOUSE_EXTRA2, /* Extra mouse button 2. */
		KEY_CANCEL, /* Control-break processing */
		KEY_BACK,   /* Backspace key. */
		KEY_TAB,    /* Tab key. */
		KEY_CLEAR,  
		KEY_RETURN,
		KEY_SHIFT, /* Shift keys. */
		KEY_CTRL, /* Ctrl keys.*/
		KEY_ALT,
		KEY_PAUSE,
		KEY_CAPITAL,
		//KEY_KANA,
		//KEY_HANGEUL,
		KEY_HANGUL,
		KEY_IME_ON,
		KEY_JUNJA,
		KEY_FINAL,
		//KEY_HANJA,
		KEY_KANJI,
		KEY_IME_OFF,
		KEY_ESCAPE,
		KEY_CONVERT,
		KEY_NONCONVERT,
		KEY_ACCEPT,
		KEY_MODECHANGE,
		KEY_SPACE,
		KEY_PRIOR,
		KEY_NEXT,
		KEY_END,
		KEY_HOME,
		KEY_LEFT,
		KEY_UP,
		KEY_RIGHT,
		KEY_DOWN,
		KEY_SELECT,
		KEY_PRINT,
		KEY_EXEC, /* Execute key. */
		KEY_SNAPSHOT,
		KEY_INSERT,
		KEY_DELETE,
		KEY_HELP,
		KEY_0, /* '0' key. */
		KEY_1, /* '1' key. */
		KEY_2, /* '2' key. */
		KEY_3, /* '3' key. */
		KEY_4, /* '4' key. */
		KEY_5, /* '5' key. */
		KEY_6, /* '6' key. */
		KEY_7, /* '7' key. */
		KEY_8, /* '8' key. */
		KEY_9, /* '9' key. */
		KEY_NUMPAD0, /* Numpad '0' key. */
		KEY_NUMPAD1, /* Numpad '1' key. */
		KEY_NUMPAD2, /* Numpad '2' key. */
		KEY_NUMPAD3, /* Numpad '3' key. */
		KEY_NUMPAD4, /* Numpad '4' key. */
		KEY_NUMPAD5, /* Numpad '5' key. */
		KEY_NUMPAD6, /* Numpad '6' key. */
		KEY_NUMPAD7, /* Numpad '7' key. */
		KEY_NUMPAD8, /* Numpad '8' key. */
		KEY_NUMPAD9, /* Numpad '9' key. */
		KEY_A, /* 'A' key. */
		KEY_B, /* 'B' key. */
		KEY_C, /* 'C' key. */

		KEY_E, /* 'E' key. */
		KEY_F, /* 'F' key. */
		KEY_G, /* 'G' key. */
		KEY_H, /* 'H' key. */
		KEY_I, /* 'I' key. */
		KEY_J, /* 'J' key. */
		KEY_K, /* 'K' key. */
		KEY_L, /* 'L' key. */
		KEY_M, /* 'M' key. */
		KEY_N, /* 'N' key. */
		KEY_O, /* 'O' key. */
		KEY_P, /* 'P' key. */
		KEY_Q, /* 'Q' key. */
		KEY_R, /* 'R' key. */
		KEY_S, /* 'S' key. */
		KEY_T, /* 'T' key. */
		KEY_U, /* 'U' key. */
		KEY_V, /* 'V' key. */

		KEY_X, /* 'X' key. */
		KEY_Y, /* 'Y' key. */
		KEY_Z, /* 'Z' key. */
		KEY_LWIN,
		KEY_RWIN,
		KEY_APPS,
		KEY_SLEEP,
		KEY_MULTIPLY,
		KEY_ADD,
		KEY_SEPARATOR,
		KEY_SUBTRACT,
		KEY_DECIMAL,
		KEY_DIVIDE,
		KEY_F1,	            /* F1 Key.  */
		KEY_F2,	            /* F2 Key.  */
		KEY_F3,	            /* F3 Key.  */
		KEY_F4,	            /* F4 Key.  */
		KEY_F5,	            /* F5 Key.  */
		KEY_F6,	            /* F6 Key.  */
		KEY_F7,	            /* F7 Key.  */
		KEY_F8,	            /* F8 Key.  */
		KEY_F9,	            /* F9 Key.  */
		KEY_F10,            /* F10 Key. */
		KEY_F11,            /* F11 Key. */
		KEY_F12,            /* F12 Key. */
		KEY_F13,            /* F13 Key. */
		KEY_F14,            /* F14 Key. */
		KEY_F15,            /* F15 Key. */
		KEY_F16,            /* F16 Key. */
		KEY_F17,            /* F17 Key. */
		KEY_F18,            /* F18 Key. */
		KEY_F19,            /* F19 Key. */
		KEY_F20,            /* F20 Key. */
		KEY_F21,            /* F21 Key. */
		KEY_F22,            /* F22 Key. */
		KEY_F23,            /* F23 Key. */
		KEY_F24,            /* F24 Key. */
		KEY_NUMLOCK,        /* Num lock key.   */
		KEY_SCROLL,         /* Scroll lock key */
		//KEY_OEM_NEC_EQUAL,  /* '=' key on numpad. NEC PC-9800 keyboard        */
		KEY_OEM_FJ_JISHO,   /* 'Dictionary' key. Fujitsu/OASYS keyboard.      */
		KEY_OEM_FJ_MASSHOU, /* 'Unregister word' key. Fujitsu/OASYS keyboard. */
		KEY_OEM_FJ_TOUROKU, /* 'Register word' key. Fujitsu/OASYS keyboard.   */
		KEY_OEM_FJ_LOYA,    /* 'Left OYAYUBI' key. Fujitsu/OASYS keyboard.    */
		KEY_OEM_FJ_ROYA,    /* 'Right OYAYUBI' key. Fujitsu/OASYS keyboard.   */
		KEY_LSHIFT,         /* Left shift key. */
		KEY_RSHIFT,         /* Right shift key. */
		KEY_LCTRL,       /* Left ctrl key. */
		KEY_RCTRL,       /* Right ctrl key*/
		KEY_LALT,          /* Left menu key. */
		KEY_RALT,          /* Right menu key. */
		KEY_BROWSER_BACK,  
		KEY_BROWSER_FORWARD,
		KEY_BROWSER_REFRESH,
		KEY_BROWSER_STOP,
		KEY_BROWSER_SEARCH,
		KEY_BROWSER_FAVORITES,
		KEY_BROWSER_HOME,
		KEY_VOLUME_MUTE,
		KEY_VOLUME_DOWN,
		KEY_VOLUME_UP,
		KEY_MEDIA_NEXT_TRACK,
		KEY_MEDIA_PREV_TRACK,
		KEY_MEDIA_STOP,
		KEY_MEDIA_PLAY_PAUSE,
		KEY_LAUNCH_MAIL,
		KEY_LAUNCH_MEDIA_SELECT,
		KEY_LAUNCH_APP1,
		KEY_LAUNCH_APP2,
		KEY_OEM_1,
		KEY_OEM_PLUS,
		KEY_OEM_COMMA,
		KEY_OEM_MINUS,
		KEY_OEM_PERIOD,
		KEY_OEM_2,
		KEY_OEM_3,
	};

}
//TODO add remaining descriptions. add gamepad codes.