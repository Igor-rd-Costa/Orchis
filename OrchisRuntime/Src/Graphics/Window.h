#pragma once
//#include "RenderContext.h"
#include "../Core/Handles.h"
#include "../Core/Macros.h"
#include "../Core/IO/Input.h"
#include <functional>

namespace Orchis {
	class Window;

	typedef std::function<void(Window*)> OnCloseCallbackFn;

	enum OC_API WindowSubdivisions {
		WINDOW_SUBDIVISIONS_ONE,
		WINDOW_SUBDIVISIONS_TWO_VERT,
		WINDOW_SUBDIVISIONS_TWO_HORZ,
		WINDOW_SUBDIVISIONS_FOUR,
	};

	class OC_API Window
	{
	public:
		virtual ~Window() = default;

		virtual void Update() = 0;
		virtual void Close() = 0;
		virtual void Open() = 0;
		virtual void Destroy() = 0;

		void OnClose(OnCloseCallbackFn callback);

		const OHandle& Handle() const { return m_Handle; }
		unsigned int Width() const { return m_Width; }
		unsigned int Height() const { return m_Height; }
	protected:
		unsigned int m_Width = 0;
		unsigned int m_Height = 0;
		bool m_IsChild = false;
	protected:
		Window() = default;
		Window(const Window&) = default;
		Window(Window&&) = default;

		std::vector<OnCloseCallbackFn> m_OnCloseCallbacks;
		//std::vector<RenderContext> m_RenderContexes;
		Input m_Input;
		OHandle m_Handle;
		WindowSubdivisions m_Subdivisions;
		std::vector<OHandle> m_Children;
	};

}