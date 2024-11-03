#pragma once
#include "../Src/Core/Macros.h"
#include <thread>

namespace Orchis {

	enum OC_API RenderAPIType {
		RENDER_API_TYPE_INVALID, RENDER_API_TYPE_VULKAN
	};

	class OC_API Renderer
	{
	public:
		Renderer() = default;
		~Renderer();

		void Init(RenderAPIType type);
		void Run();
		void Shutdown();
	private:
		std::thread m_RenderThread;
		bool m_IsRunning = false;
	};
}