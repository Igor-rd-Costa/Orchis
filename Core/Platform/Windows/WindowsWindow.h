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

		virtual void Update() override;

	private:
		static LRESULT CALLBACK WinProc(HWND hWnd, UINT nShowCmd, WPARAM wParam, LPARAM lParam);
		uint32_t m_Width, m_Height;
		HWND m_WindowHandle;
		HDC m_HandleDeviceContext;
	};

	Ref<Window> Window::Create(std::string_view name)
	{
		return CreateRef<WindowsWindow>(name);
	}


}