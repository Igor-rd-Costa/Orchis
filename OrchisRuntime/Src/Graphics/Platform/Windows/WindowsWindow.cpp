#include "WindowsWindow.h"
#include "../../WindowManager.h"
#include "Core/IO/Logger.h"
namespace Orchis {

	HINSTANCE WindowsAppInstance()
	{
		static HINSTANCE hIntance = (HINSTANCE)GetCurrentProcess();
		return hIntance;
	}

	WindowsWindow::WindowsWindow(const OHandle& parent)
		: m_PlatformHandle(NULL)
	{
		const wchar_t* className = L"OrchisWindow";
		WNDCLASSEX wc = {};
		wc.lpfnWndProc = WindowsWindow::WinProc;
		wc.hInstance = WindowsAppInstance();
		wc.lpszClassName = className;
		wc.cbSize = sizeof(WNDCLASSEX);

		ATOM atom = RegisterClassEx(&wc);

		if (atom == 0) {
			DWORD error = GetLastError();
			OC_ASSERT(false);
			return;
		}

		HWND parentWindow = NULL;
		Window* pWindow = WindowManager::GetWindow(parent);
		if (pWindow)
		{
			parentWindow = reinterpret_cast<WindowsWindow*>(pWindow)->m_PlatformHandle;
		}

		m_PlatformHandle = CreateWindowEx(
			0,
			className,
			L"Orchis",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			parentWindow,
			NULL,
			WindowsAppInstance(),
			NULL
		);
		OC_ASSERT(m_PlatformHandle != NULL);

		SetWindowLongPtr(m_PlatformHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		Open();
	}

	WindowsWindow::~WindowsWindow()
	{
	}
	void WindowsWindow::Update()
	{
		static MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowsWindow::Close()
	{
		CloseWindow(m_PlatformHandle);
	}

	void WindowsWindow::Open()
	{
		ShowWindow(m_PlatformHandle, SW_SHOWDEFAULT);
	}

	void WindowsWindow::Destroy()
	{
		DestroyWindow(m_PlatformHandle);
	}

	LRESULT CALLBACK WindowsWindow::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowsWindow* window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (uMsg)
		{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN: {
			window->m_Input.RegisterKeyPress(WindowsWindow::WindowsKeyToOrchisKey(wParam, lParam));
		} break;
		case WM_LBUTTONDOWN: {
			window->m_Input.RegisterKeyPress(Key::KEY_LMOUSE);
		} break;
		case WM_RBUTTONDOWN: {
			window->m_Input.RegisterKeyPress(Key::KEY_RMOUSE);
		} break;
		case WM_MBUTTONDOWN: {
			window->m_Input.RegisterKeyPress(Key::KEY_MMOUSE);
		} break;
		case WM_XBUTTONDOWN: {
			int button = HIWORD(wParam);
			if (button == 1) {
				window->m_Input.RegisterKeyPress(Key::KEY_XMOUSE1);
			}
			else if (button == 2) {
				window->m_Input.RegisterKeyPress(Key::KEY_XMOUSE2);
			}
		} break;
		case WM_SYSKEYUP:
		case WM_KEYUP: {
			window->m_Input.RegisterKeyRelease(WindowsWindow::WindowsKeyToOrchisKey(wParam, lParam));
		} break;
		case WM_LBUTTONUP: {
			window->m_Input.RegisterKeyRelease(Key::KEY_LMOUSE);
		} break;
		case WM_RBUTTONUP: {
			window->m_Input.RegisterKeyRelease(Key::KEY_RMOUSE);
		} break;
		case WM_MBUTTONUP: {
			window->m_Input.RegisterKeyRelease(Key::KEY_MMOUSE);
		} break;
		case WM_XBUTTONUP: {
			int button = HIWORD(wParam);
			if (button == 1) {
				window->m_Input.RegisterKeyRelease(Key::KEY_XMOUSE1);
			}
			else if (button == 2) {
				window->m_Input.RegisterKeyRelease(Key::KEY_XMOUSE2);
			}
		} break;
		case WM_MOUSEMOVE: {
			// TODO: Add events
		} break;
		case WM_SIZE: {
			window->m_Width = static_cast<unsigned int>(LOWORD(lParam));
			window->m_Height = static_cast<unsigned int>(HIWORD(lParam));
		} break;
		case WM_DPICHANGED: {
			//TODO: Handle this
			//Logger::WriteLine("DPI Change!");
		} break;
		case WM_CLOSE: {
			DestroyWindow(hWnd);
		} break;
		case WM_DESTROY: {
			PostQuitMessage(0);
			for (const OnCloseCallbackFn& callback : window->m_OnCloseCallbacks)
			{
				callback(window);
			}
		} break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		return 0;
	}

	Key WindowsWindow::WindowsKeyToOrchisKey(WPARAM wParam, LPARAM lParam)
	{
		switch (wParam)
		{
		case VK_LBUTTON: return KEY_LMOUSE;
		case VK_RBUTTON: return KEY_RMOUSE;

		case VK_MBUTTON: return KEY_MMOUSE;
		case VK_XBUTTON1: return KEY_XMOUSE1;
		case VK_XBUTTON2: return KEY_XMOUSE2;
		case VK_BACK: return KEY_BACKSPACE;
		case VK_TAB: return KEY_TAB;
		case VK_CLEAR: return KEY_CLEAR;
		case VK_RETURN: {
			if (((lParam >> 24) & 1) == 1)
				return KEY_NUMPAD_ENTER;
			return KEY_ENTER;
		}
		case VK_SHIFT: {
			if (((lParam >> 24) & 1) == 1)
				return KEY_RSHIFT;
			return KEY_LSHIFT;
		}
		case VK_CONTROL: {
			if (((lParam >> 24) & 1) == 1)
				return KEY_RCTRL;
			return KEY_LCTRL;
		}
		case VK_MENU: {
			if (((lParam >> 24) & 1) == 1)
				return KEY_RALT;
			return KEY_LALT;
		};
		case VK_PAUSE: return KEY_PAUSE;
		case VK_CAPITAL: return KEY_CAPS_LOCK;

		case VK_ESCAPE: return KEY_ESC;
		case VK_SPACE: return KEY_SPACE;
		case VK_PRIOR: return KEY_PAGE_UP;
		case VK_NEXT: return KEY_PAGE_DOWN;
		case VK_END: return KEY_END;
		case VK_HOME: return KEY_HOME;
		case VK_LEFT: return KEY_ARROW_LEFT;
		case VK_UP: return KEY_ARROW_UP;
		case VK_RIGHT: return KEY_ARROW_RIGHT;
		case VK_DOWN: return KEY_ARROW_DOWN;
			//case VK_SELECT: return KEY_SELECT;
		case VK_PRINT: return KEY_PRINT_SCREEN;
		case VK_INSERT: return KEY_INSERT;
		case VK_DELETE: return KEY_DELETE;
		case VK_SCROLL: return KEY_SCROLL_LOCK;
		case VK_APPS: return KEY_APPS;
		case VK_F1: return KEY_F1;
		case VK_F2: return KEY_F2;
		case VK_F3: return KEY_F3;
		case VK_F4: return KEY_F4;
		case VK_F5: return KEY_F5;
		case VK_F6: return KEY_F6;
		case VK_F7: return KEY_F7;
		case VK_F8: return KEY_F8;
		case VK_F9: return KEY_F9;
		case VK_F10: return KEY_F10;
		case VK_F11: return KEY_F11;
		case VK_F12: return KEY_F12;
		case '0': return KEY_0;
		case '1': return KEY_1;
		case '2': return KEY_2;
		case '3': return KEY_3;
		case '4': return KEY_4;
		case '5': return KEY_5;
		case '6': return KEY_6;
		case '7': return KEY_7;
		case '8': return KEY_8;
		case '9': return KEY_9;
		case 'A': return KEY_A;
		case 'B': return KEY_B;
		case 'C': return KEY_C;
		case 'D': return KEY_D;
		case 'E': return KEY_E;
		case 'F': return KEY_F;
		case 'G': return KEY_G;
		case 'H': return KEY_H;
		case 'I': return KEY_I;
		case 'J': return KEY_J;
		case 'K': return KEY_K;
		case 'L': return KEY_L;
		case 'M': return KEY_M;
		case 'N': return KEY_N;
		case 'O': return KEY_O;
		case 'P': return KEY_P;
		case 'Q': return KEY_Q;
		case 'R': return KEY_R;
		case 'S': return KEY_S;
		case 'T': return KEY_T;
		case 'U': return KEY_U;
		case 'V': return KEY_V;
		case 'W': return KEY_W;
		case 'X': return KEY_X;
		case 'Y': return KEY_Y;
		case 'Z': return KEY_Z;
		case VK_NUMLOCK: return KEY_NUMLOCK;
		case VK_NUMPAD0: return KEY_NUMPAD0;
		case VK_NUMPAD1: return KEY_NUMPAD1;
		case VK_NUMPAD2: return KEY_NUMPAD2;
		case VK_NUMPAD3: return KEY_NUMPAD3;
		case VK_NUMPAD4: return KEY_NUMPAD4;
		case VK_NUMPAD5: return KEY_NUMPAD5;
		case VK_NUMPAD6: return KEY_NUMPAD6;
		case VK_NUMPAD7: return KEY_NUMPAD7;
		case VK_NUMPAD8: return KEY_NUMPAD8;
		case VK_NUMPAD9: return KEY_NUMPAD9;
		case VK_DIVIDE: return KEY_NUMPAD_DIVIDE;
		case VK_MULTIPLY: return KEY_NUMPAD_MULTIPLY;
		case VK_SUBTRACT: return KEY_NUMPAD_MINUS;
		case VK_ADD: return KEY_NUMPAD_PLUS;
		case VK_DECIMAL: return KEY_NUMPAD_COMMA;
		case VK_LWIN: return KEY_LSYSTEM;
		case VK_RWIN: return KEY_RSYSTEM;
		case VK_OEM_PLUS: return KEY_EQUAL;
		case VK_OEM_COMMA: return KEY_COMMA;
		case VK_OEM_PERIOD: return KEY_PERIOD;
		case VK_OEM_MINUS: return KEY_HYPHEN;
		case VK_OEM_1: return KEY_OEM1;
		case VK_OEM_2: return KEY_OEM2;
		case VK_OEM_3: return KEY_OEM3;
		case VK_OEM_4: return KEY_OEM4;
		case VK_OEM_5: return KEY_OEM5;
		case VK_OEM_6: return KEY_OEM6;
		case VK_OEM_7: return KEY_OEM7;
		case VK_OEM_8: return KEY_OEM8;
		case VK_OEM_102: return KEY_OEM_102;
		case 193: return KEY_FSLASH;
		default:
			break;
		}
		g_CoreLogger.Error("Unregistered key {}", wParam);
		return KEY_F1;
	}
}