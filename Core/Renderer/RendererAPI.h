#pragma once
#include "Macros.h"
#include "Renderer/GraphicsPipeline.h"

namespace Orchis {

	class ORCHIS_API RendererAPI
	{
	public:
		enum API
		{
			NONE, VULKAN
		};
		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void DrawCube() = 0;

		virtual void StartFrame() = 0;
		virtual void SwapBuffers() = 0;

	private:
		static API s_API;
	};
}