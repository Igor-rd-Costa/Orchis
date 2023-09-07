#pragma once
#include <vulkan/vulkan.h>

namespace Orchis {

	class VulkanRenderPassManager
	{
	public:
		static void CreateRenderPasses();
		static void DeleteRenderPasses();
		static VkRenderPass GetRenderPass() { return m_RenderPass; }
	private:
		static VkRenderPass m_RenderPass;
	};
}