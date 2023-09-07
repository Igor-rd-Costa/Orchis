#pragma once
#include "Window.h"
#include "OrchisWindows.h"

namespace Orchis {

	

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(std::string_view name);
		virtual ~WindowsWindow();

		virtual uint32_t GetWidth() const override { return m_Width; };
		virtual uint32_t GetHeight() const override { return m_Height; };
		virtual std::pair<long, long> GetCenter() const override { return { ((m_WindowRect.left + m_WindowRect.right) / 2), ((m_WindowRect.bottom + m_WindowRect.top) / 2) }; }

		virtual void Update() override;
		virtual void* GetHandle() override {
			return reinterpret_cast<void*>(&m_WindowHandle);
		}
	private:
		static LRESULT CALLBACK WinProc(HWND hWnd, UINT nShowCmd, WPARAM wParam, LPARAM lParam);
	private:
		uint32_t m_Width, m_Height;
		HWND m_WindowHandle;
		HDC m_HandleDeviceContext;
		RECT m_WindowRect;
	};

	Scope<Window> Window::Create(std::string_view name)
	{
		return CreateScope<WindowsWindow>(name);
	}


}