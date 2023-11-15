#include "WindowsWindow.h"
#include <windowsx.h>
#include "KeyCodes.h"
#include "EventDispatcher.h"
#include  <string>


namespace Orchis {

	struct {
		const char* Extensions[2] = { "VK_KHR_surface", "VK_KHR_win32_surface" };
	} vk;

	WindowsWindow::WindowsWindow(void* parentHandle)
		: m_Width(1600), m_Height(900)
	{
		uint32_t windowFlags = 0;
		WNDCLASS windowClass{};
		windowClass.lpszClassName = L"OrchisWindows";

		windowClass.lpfnWndProc = WinProc;
		windowClass.hInstance = GetModuleHandle(NULL);
		RegisterClass(&windowClass);

		int flags = 0;

		if (parentHandle)
		{
			flags |= WS_CHILD | WS_BORDER;
			m_IsChild = true;
		}
		else
		{
			flags |= WS_OVERLAPPEDWINDOW;
		}

		this->m_WindowHandle = CreateWindowEx(
			CS_OWNDC,
			L"OrchisWindows",
			L"Window",
			flags,
			0, 0,
			m_Width, m_Height,
			(HWND)parentHandle,
			NULL,
			GetModuleHandleA(NULL),
			NULL
		);
		SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		m_HandleDeviceContext = GetDC(m_WindowHandle);
		ShowWindow(m_WindowHandle, SW_NORMAL);
		GetWindowRect(m_WindowHandle, &m_WindowRect);
		Update();
	}

	WindowsWindow::~WindowsWindow()
	{
		DestroyWindow(m_WindowHandle);
	}

	void WindowsWindow::UpdateRect()
	{
		GetWindowRect(m_WindowHandle, &m_WindowRect);
	}

	void WindowsWindow::Update()
	{
		static MSG msg;
		UpdateInputState();
		if (!m_IsChild)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	const char** Window::GetRequiredVKExtensions(uint32_t* outExtensionCount)
	{
		if (outExtensionCount)
			*outExtensionCount = 2;

		return vk.Extensions;
	}

	LRESULT CALLBACK WindowsWindow::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

		switch (uMsg)
		{
		case WM_MOUSEMOVE:
		{
			EventDispatcher::Dispatch<MouseMoveEvent>({ (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam) });
		} break;
		case WM_LBUTTONDOWN:
		{	
			EventDispatcher::Dispatch<MouseButtonDownEvent>({ Key::MOUSE_LBUTTON, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam) });
		} break;
		case WM_LBUTTONUP:
		{
			EventDispatcher::Dispatch<MouseButtonUpEvent>({ Key::MOUSE_LBUTTON, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam) });
		} break;
		case WM_RBUTTONDOWN:
		{
			EventDispatcher::Dispatch<MouseButtonDownEvent>({ Key::MOUSE_RBUTTON, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam) });
		} break;
		case WM_RBUTTONUP:
		{
			EventDispatcher::Dispatch<MouseButtonUpEvent>({ Key::MOUSE_RBUTTON, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam) });
		} break;
		case WM_MBUTTONDOWN:
		{
			EventDispatcher::Dispatch<MouseButtonDownEvent>({ Key::MOUSE_MBUTTON, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam) });
		} break;
		case WM_MBUTTONUP:
		{
			EventDispatcher::Dispatch<MouseButtonUpEvent>({ Key::MOUSE_MBUTTON, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam) });
		} break;
		case WM_XBUTTONDOWN:
		{
			UINT button = GET_XBUTTON_WPARAM(wParam);
			if (button == XBUTTON1)
			{
				EventDispatcher::Dispatch<MouseButtonDownEvent>({ Key::MOUSE_EXTRA1, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam) });
			}
			else if (button == XBUTTON2)
			{
				EventDispatcher::Dispatch<MouseButtonDownEvent>({ Key::MOUSE_EXTRA2, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam) });
			}
		} break;
		case WM_XBUTTONUP:
		{
			UINT button = GET_XBUTTON_WPARAM(wParam);
			if (button == XBUTTON1)
			{
				EventDispatcher::Dispatch<MouseButtonUpEvent>({ Key::MOUSE_EXTRA1, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam) });
			}
			else if (button == XBUTTON2)
			{
				EventDispatcher::Dispatch<MouseButtonUpEvent>({ Key::MOUSE_EXTRA2, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam) });
			}
		} break;
		case WM_MOUSEWHEEL:
		{
			EventDispatcher::Dispatch<MouseScrollEvent>({ static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) });
		} break;
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			EventDispatcher::Dispatch<KeyDownEvent>({ TranslateKeyCode(wParam, lParam) });

			if (wParam == VK_SHIFT) EventDispatcher::Dispatch<KeyDownEvent>({ Key::KEY_SHIFT });
			if (wParam == VK_CONTROL) EventDispatcher::Dispatch<KeyDownEvent>({ Key::KEY_CTRL });
			if (wParam == VK_MENU) EventDispatcher::Dispatch<KeyDownEvent>({ Key::KEY_ALT });
		} break;
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			EventDispatcher::Dispatch<KeyUpEvent>({ TranslateKeyCode(wParam, lParam) });
		} break;
		case WM_SIZE:
		{
			WindowsWindow* window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			if (window)
				window->UpdateRect();

			EventDispatcher::Dispatch<WindowResizeEvent>({ (float)LOWORD(lParam), (float)HIWORD(lParam) });
			if (wParam == SIZE_MINIMIZED)
				EventDispatcher::Dispatch<WindowMinimizeEvent>(WindowMinimizeEvent());
		} break;
		case WM_MOVE:
		{
			WindowsWindow* window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			if (window)
				window->UpdateRect();
		} break;
		case WM_CLOSE:
		{
			EventDispatcher::Dispatch<WindowCloseEvent>(WindowCloseEvent());
			DestroyWindow(hWnd);
		} break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		} break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		return 0;
	}

}
