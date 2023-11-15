#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"
namespace Orchis {

	Window* Window::Create(void* parentHandle)
	{
		return new WindowsWindow(parentHandle);
	}

}