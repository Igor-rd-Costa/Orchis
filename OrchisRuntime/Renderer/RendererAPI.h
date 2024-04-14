#pragma once
#include "Macros.h"
#include "Renderer/GraphicsPipeline.h"

namespace Orchis {

	class RendererAPI
	{
	public:
		RendererAPI() = default;
		virtual ~RendererAPI() = default;

		enum API
		{
			NONE, VULKAN
		};

	private:
		static API s_API;
	};
}