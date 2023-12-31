#pragma once
#include <vulkan/vulkan.h>
#include "Renderer/RenderCommand.h"
namespace Orchis {

	class VulkanRenderCommand : public RenderCommand
	{
	public:
		VulkanRenderCommand();
		~VulkanRenderCommand();

		static const VkCommandBuffer& GetCommandBuffer();
	private:
		static VkCommandPool s_CommandPool;
		static std::vector<VkCommandBuffer> s_CommandBuffers;
		static VkCommandBuffer s_CommandBuffer;
		static std::array<VkClearValue, 2> s_ClearValues;
		
		static void StartRenderPass();
		static void EndRenderPass();

		static void Begin();
		virtual void DrawIndexedImpl(const Mesh* mesh) override;
		virtual void SetClearColorImpl(float r, float g, float b, float a) override;
		virtual void BeginFrameImpl(Scene* scene) const override;
		virtual void SwapBuffersImpl() const override;

	};
}