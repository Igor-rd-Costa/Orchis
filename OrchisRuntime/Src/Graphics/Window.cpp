#include "Window.h"

namespace Orchis {

	void Window::OnClose(OnCloseCallbackFn callback)
	{
		m_OnCloseCallbacks.push_back(callback);
	}

}