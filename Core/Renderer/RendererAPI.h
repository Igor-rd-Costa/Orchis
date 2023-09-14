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
		virtual void ShutDown() = 0;
	private:
		static API s_API;
	};
}