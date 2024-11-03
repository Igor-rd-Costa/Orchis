#pragma once
#include "../../Window.h"
#include <windows.h>


namespace Orchis {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const OHandle& parent);
		~WindowsWindow();

		void Update() override;
		void Close() override;
		void Open() override;
		void Destroy() override;
	private:
		HWND m_PlatformHandle;
		static LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
		static Key WindowsKeyToOrchisKey(WPARAM wParam, LPARAM lParam);
	};
}