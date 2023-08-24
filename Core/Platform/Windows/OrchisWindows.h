#pragma once
#include <string>
#include <Windows.h>
#include "KeyCodes.h"
#include <bitset>

namespace Orchis {

	std::wstring StringToWideString(const std::string& string)
	{
		int stringSize = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, string.data(), static_cast<int>(string.size()), nullptr, 0);
		std::wstring wString(static_cast<size_t>(stringSize), L'\n');
		MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, string.data(), static_cast<int>(string.size()), wString.data(), stringSize);
		return wString;
	}

	std::wstring StringToWideString(std::string_view string)
	{
		int stringSize = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, string.data(), static_cast<int>(string.size()), nullptr, 0);
		std::wstring wString(static_cast<size_t>(stringSize), L'\n');
		MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, string.data(), static_cast<int>(string.size()), wString.data(), stringSize);
		return wString;
	}

	Key TranslateKeyCode(WPARAM wParam, LPARAM lParam)
	{
		switch (wParam)
		{
			case 'A': return Key::KEY_A;
			case 'B': return Key::KEY_B;
			case 'C': return Key::KEY_C;
			case 'D': return Key::KEY_D;
			case 'E': return Key::KEY_E;
			case 'F': return Key::KEY_F;
			case 'G': return Key::KEY_G;
			case 'H': return Key::KEY_H;
			case 'I': return Key::KEY_I;
			case 'J': return Key::KEY_J;
			case 'K': return Key::KEY_K;
			case 'L': return Key::KEY_L;
			case 'M': return Key::KEY_M;
			case 'N': return Key::KEY_N;
			case 'O': return Key::KEY_O;
			case 'P': return Key::KEY_P;
			case 'Q': return Key::KEY_Q;
			case 'R': return Key::KEY_R;
			case 'S': return Key::KEY_S;
			case 'T': return Key::KEY_T;
			case 'U': return Key::KEY_U;
			case 'V': return Key::KEY_V;
			case 'W': return Key::KEY_W;
			case 'X': return Key::KEY_X;
			case 'Y': return Key::KEY_Y;
			case 'Z': return Key::KEY_Z;
			case '0': return Key::KEY_0;
			case '1': return Key::KEY_1;
			case '2': return Key::KEY_2;
			case '3': return Key::KEY_3;
			case '4': return Key::KEY_4;
			case '5': return Key::KEY_5;
			case '6': return Key::KEY_6;
			case '7': return Key::KEY_7;
			case '8': return Key::KEY_8;
			case '9': return Key::KEY_9;
			case VK_SPACE: return Key::KEY_SPACE;
			case VK_SHIFT:
			{
				if (((lParam >> 16) & CHAR_MAX) == MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC))
				{
					return Key::KEY_LSHIFT;
				}
				else
				{
					return Key::KEY_RSHIFT;
				}
			}
			case VK_CONTROL: 
			{
				if ((lParam >> 24) & 1)
				{
					return Key::KEY_RCTRL;
				}
				else
				{
					return Key::KEY_LCTRL;
				}
			}
			case VK_MENU:
			{
				if ((lParam >> 24) & 1)
				{
					return Key::KEY_RALT;
				}
				else
				{
					return Key::KEY_LALT;
				}
			}
			case VK_TAB: return Key::KEY_TAB;
			case VK_NUMPAD0: return Key::KEY_NUMPAD0;
			case VK_NUMPAD1: return Key::KEY_NUMPAD1;
			case VK_NUMPAD2: return Key::KEY_NUMPAD2;
			case VK_NUMPAD3: return Key::KEY_NUMPAD3;
			case VK_NUMPAD4: return Key::KEY_NUMPAD4;
			case VK_NUMPAD5: return Key::KEY_NUMPAD5;
			case VK_NUMPAD6: return Key::KEY_NUMPAD6;
			case VK_NUMPAD7: return Key::KEY_NUMPAD7;
			case VK_NUMPAD8: return Key::KEY_NUMPAD8;
			case VK_NUMPAD9: return Key::KEY_NUMPAD9;
			case VK_CANCEL: return Key::KEY_CANCEL;
			case VK_BACK: return Key::KEY_BACK;
			case VK_CLEAR: return Key::KEY_CLEAR;
			case VK_RETURN: return Key::KEY_RETURN;
			case VK_PAUSE: return Key::KEY_PAUSE;
			case VK_CAPITAL: return Key::KEY_CAPITAL;
			case VK_ESCAPE: return Key::KEY_ESCAPE;
			case VK_PRIOR: return Key::KEY_PRIOR;
			case VK_NEXT: return Key::KEY_NEXT;
			case VK_END: return Key::KEY_END;
			case VK_HOME: return Key::KEY_HOME;
			case VK_LEFT: return Key::KEY_LEFT;
			case VK_UP: return Key::KEY_UP;
			case VK_RIGHT: return Key::KEY_RIGHT;
			case VK_DOWN: return Key::KEY_DOWN;
			case VK_SELECT: return Key::KEY_SELECT;
			case VK_PRINT: return Key::KEY_PRINT;
			case VK_EXECUTE: return Key::KEY_EXEC;
			case VK_SNAPSHOT: return Key::KEY_SNAPSHOT;
			case VK_INSERT: return Key::KEY_INSERT;
			case VK_DELETE: return Key::KEY_DELETE;
			case VK_HELP: return Key::KEY_HELP;
			case VK_LWIN: return Key::KEY_LWIN;
			case VK_RWIN: return Key::KEY_RWIN;
			case VK_APPS: return Key::KEY_APPS;
			case VK_SLEEP: return Key::KEY_SLEEP;
			case VK_MULTIPLY: return Key::KEY_MULTIPLY;
			case VK_ADD: return Key::KEY_ADD;
			case VK_SEPARATOR: return Key::KEY_SEPARATOR;
			case VK_SUBTRACT: return Key::KEY_SUBTRACT;
			case VK_DECIMAL: return Key::KEY_DECIMAL;
			case VK_DIVIDE: return Key::KEY_DIVIDE;
			case VK_F1: return Key::KEY_F1;
			case VK_F2: return Key::KEY_F2;
			case VK_F3: return Key::KEY_F3;
			case VK_F4: return Key::KEY_F4;
			case VK_F5: return Key::KEY_F5;
			case VK_F6: return Key::KEY_F6;
			case VK_F7: return Key::KEY_F7;
			case VK_F8: return Key::KEY_F8;
			case VK_F9: return Key::KEY_F9;
			case VK_F10: return Key::KEY_F10;
			case VK_F11: return Key::KEY_F11;
			case VK_F12: return Key::KEY_F12;
			case VK_F13: return Key::KEY_F13;
			case VK_F14: return Key::KEY_F14;
			case VK_F15: return Key::KEY_F15;
			case VK_F16: return Key::KEY_F16;
			case VK_F17: return Key::KEY_F17;
			case VK_F18: return Key::KEY_F18;
			case VK_F19: return Key::KEY_F19;
			case VK_F20: return Key::KEY_F20;
			case VK_F21: return Key::KEY_F21;
			case VK_F22: return Key::KEY_F22;
			case VK_F23: return Key::KEY_F23;
			case VK_F24: return Key::KEY_F24;
			case VK_NUMLOCK: return Key::KEY_NUMLOCK;
			case VK_SCROLL: return Key::KEY_SCROLL;
			case VK_OEM_1: return Key::KEY_OEM_1;
			case VK_OEM_PLUS: return Key::KEY_OEM_PLUS;
			case VK_OEM_COMMA: return Key::KEY_OEM_COMMA;
			case VK_OEM_MINUS: return Key::KEY_OEM_MINUS;
			case VK_OEM_PERIOD: return Key::KEY_OEM_PERIOD;
			case VK_OEM_2: return Key::KEY_OEM_2;
			case VK_OEM_3: return Key::KEY_OEM_3;
			case VK_VOLUME_MUTE: return Key::KEY_VOLUME_MUTE;
			case VK_VOLUME_DOWN: return Key::KEY_VOLUME_DOWN;
			case VK_VOLUME_UP: return Key::KEY_VOLUME_UP;
			case VK_MEDIA_NEXT_TRACK: return Key::KEY_MEDIA_NEXT_TRACK;
			case VK_MEDIA_PREV_TRACK: return Key::KEY_MEDIA_PREV_TRACK;
			case VK_MEDIA_STOP: return Key::KEY_MEDIA_STOP;
			case VK_MEDIA_PLAY_PAUSE: return Key::KEY_MEDIA_PLAY_PAUSE;
			case VK_LAUNCH_MAIL: return Key::KEY_LAUNCH_MAIL;
			case VK_LAUNCH_MEDIA_SELECT: return Key::KEY_LAUNCH_MEDIA_SELECT;
			case VK_LAUNCH_APP1: return Key::KEY_LAUNCH_APP1;
			case VK_LAUNCH_APP2: return Key::KEY_LAUNCH_APP2;
			case VK_BROWSER_BACK: return Key::KEY_BROWSER_BACK;
			case VK_BROWSER_FORWARD: return Key::KEY_BROWSER_FORWARD;
			case VK_BROWSER_REFRESH: return Key::KEY_BROWSER_REFRESH;
			case VK_BROWSER_STOP: return Key::KEY_BROWSER_STOP;
			case VK_BROWSER_SEARCH: return Key::KEY_BROWSER_SEARCH;
			case VK_BROWSER_FAVORITES: return Key::KEY_BROWSER_FAVORITES;
			case VK_BROWSER_HOME: return Key::KEY_BROWSER_HOME;
			case VK_OEM_FJ_JISHO: return Key::KEY_OEM_FJ_JISHO;
			case VK_OEM_FJ_MASSHOU: return Key::KEY_OEM_FJ_MASSHOU;
			case VK_OEM_FJ_TOUROKU: return Key::KEY_OEM_FJ_TOUROKU;
			case VK_OEM_FJ_LOYA: return Key::KEY_OEM_FJ_LOYA;
			case VK_OEM_FJ_ROYA: return Key::KEY_OEM_FJ_ROYA;
			case VK_CONVERT: return Key::KEY_CONVERT;
			case VK_NONCONVERT: return Key::KEY_NONCONVERT;
			case VK_ACCEPT: return Key::KEY_ACCEPT;
			case VK_MODECHANGE: return Key::KEY_MODECHANGE;
			case VK_HANGUL: return Key::KEY_HANGUL;
			case VK_IME_ON: return Key::KEY_IME_ON;
			case VK_JUNJA: return Key::KEY_JUNJA;
			case VK_FINAL: return Key::KEY_FINAL;
			case VK_KANJI: return Key::KEY_KANJI;
			case VK_IME_OFF: return Key::KEY_IME_OFF;
		}
		std::cout << "Undefined key code " << (int)wParam << ".\n";
		return Key::KEY_UNKNOWN;
	}
}