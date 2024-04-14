#pragma once
#include "Window.h"
#include "RendererAPI.h"

namespace Orchis {

	class RenderContext
	{
	public:
		RenderContext(void* parentWindow);
		virtual ~RenderContext();

		Window* GetWindow() { return m_Window; }
		RendererAPI* GetAPI() { return m_RendererAPI; }

	protected:
		Window* m_Window;
		RendererAPI* m_RendererAPI;
	};
}