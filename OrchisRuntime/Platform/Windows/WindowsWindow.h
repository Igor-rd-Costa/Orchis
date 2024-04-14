#pragma once
#include "Window.h"
#include "OrchisWindows.h"

namespace Orchis {

	

	class __declspec(dllexport) WindowsWindow : public Window
	{
	public:
		WindowsWindow(void* parentHandle = nullptr);
		virtual ~WindowsWindow();

		virtual uint32_t GetWidth() const override { return m_Width; };
		virtual uint32_t GetHeight() const override { return m_Height; };
		virtual std::pair<long, long> GetCenter() const override { return { ((m_WindowRect.left + m_WindowRect.right) / 2), ((m_WindowRect.bottom + m_WindowRect.top) / 2) }; }
		void UpdateRect();

		virtual void Update() override;
		virtual void* GetHandle() override {
			return (void*)m_WindowHandle;
		}
	private:
		static LRESULT CALLBACK WinProc(HWND hWnd, UINT nShowCmd, WPARAM wParam, LPARAM lParam);
	private:
		uint32_t m_Width, m_Height;
		HWND m_WindowHandle;
		HDC m_HandleDeviceContext;
		RECT m_WindowRect;
		bool m_IsChild = false;
	};
}