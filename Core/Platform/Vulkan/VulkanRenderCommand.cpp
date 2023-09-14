#include "OrchisPCH.h"
#include "VulkanRenderCommand.h"
#include "VulkanAPI.h"
#include "VulkanDescriptorSets.h"
#include "VulkanTexture.h"
#include "Scene.h"

namespace Orchis {
	VkCommandBuffer VulkanRenderCommand::s_CommandBuffer;

	VkCommandPool VulkanRenderCommand::s_CommandPool;
	std::vector<VkCommandBuffer> VulkanRenderCommand::s_CommandBuffers;
	std::array<VkClearValue, 2> VulkanRenderCommand::s_ClearValues{};

	VulkanRenderCommand::VulkanRenderCommand()
	{
		s_ClearValues[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };
		s_ClearValues[1].depthStencil = { 1.0f, 0 };
		s_CommandBuffers.resize(VulkanAPI::s_MaxFramesInFlight);
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = VulkanAPI::GetQueueFamilies().GraphicsFamily.value();

		VkResult result = vkCreateCommandPool(VulkanAPI::GetDevice(), &poolInfo, nullptr, &s_CommandPool);
		OC_ASSERT(result == VK_SUCCESS);

		VkCommandBufferAllocateInfo bufferAllocInfo{};
		bufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		bufferAllocInfo.commandPool = s_CommandPool;
		bufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		bufferAllocInfo.commandBufferCount = static_cast<uint32_t>(s_CommandBuffers.size());

		result = vkAllocateCommandBuffers(VulkanAPI::GetDevice(), &bufferAllocInfo, s_CommandBuffers.data());
		OC_ASSERT(result == VK_SUCCESS);
	}

	VulkanRenderCommand::~VulkanRenderCommand()
	{
		vkDestroyCommandPool(VulkanAPI::GetDevice(), s_CommandPool, nullptr);
	}

	const VkCommandBuffer& VulkanRenderCommand::GetCommandBuffer()
	{
		return s_CommandBuffers[VulkanAPI::s_CurrentFrame];
	}

	void VulkanRenderCommand::Begin()
	{
		static VkResult result = VK_SUCCESS;
		//s_CommandBuffer = BeginSingleTimeCommands();

		vkResetCommandBuffer(s_CommandBuffers[VulkanAPI::s_CurrentFrame], 0);
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;

		result = vkBeginCommandBuffer(s_CommandBuffers[VulkanAPI::s_CurrentFrame], &beginInfo);
		OC_ASSERT(result == VK_SUCCESS);
	}

	void VulkanRenderCommand::BeginFrameImpl(Scene* scene) const
	{
		VulkanAPI::BeginFrame();
		Begin();

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = VulkanRenderPassManager::GetRenderPass();
		renderPassInfo.framebuffer = VulkanAPI::GetSwapchain()->GetFramebuffer(VulkanAPI::s_ImageIndex);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VulkanAPI::GetSwapchain()->GetExtent();
		renderPassInfo.clearValueCount = static_cast<uint32_t>(s_ClearValues.size());
		renderPassInfo.pClearValues = s_ClearValues.data();

		vkCmdBeginRenderPass(s_CommandBuffers[VulkanAPI::s_CurrentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(s_CommandBuffers[VulkanAPI::s_CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanGraphicsPipeline::GetBoundPipeline());
		
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(VulkanAPI::GetSwapchain()->GetExtentWidth());
		viewport.height = static_cast<float>(VulkanAPI::GetSwapchain()->GetExtentHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vkCmdSetViewport(s_CommandBuffers[VulkanAPI::s_CurrentFrame], 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VulkanAPI::GetSwapchain()->GetExtent();
		vkCmdSetScissor(s_CommandBuffers[VulkanAPI::s_CurrentFrame], 0, 1, &scissor);

		VulkanDescriptorSetManager::BindDescriptorSets();

		vkCmdPushConstants(s_CommandBuffers[VulkanAPI::s_CurrentFrame], VulkanGraphicsPipeline::GetBoundPipelineLayout(),
			SHADER_STAGE_FRAGMENT, sizeof(glm::mat4), sizeof(glm::vec3), &scene->GetCamera()->GetPosition());
	}

	void VulkanRenderCommand::SwapBuffersImpl() const
	{
		static VkResult result = VK_SUCCESS;

		vkCmdEndRenderPass(s_CommandBuffers[VulkanAPI::s_CurrentFrame]);

		result = vkEndCommandBuffer(s_CommandBuffers[VulkanAPI::s_CurrentFrame]);
		OC_ASSERT(result == VK_SUCCESS);

		VulkanAPI::SwapBuffers();

	}

	void VulkanRenderCommand::DrawIndexedImpl(const Mesh* mesh)
	{
		mesh->Bind();
		for (const SubMesh& subMesh : mesh->m_SubMeshes)
		{
			subMesh.GetTexture()->Bind();
			uint32_t textureIndex = reinterpret_cast<const VulkanTexture*>(subMesh.GetTexture())->GetIndex();
			vkCmdPushConstants(s_CommandBuffers[VulkanAPI::s_CurrentFrame], VulkanGraphicsPipeline::GetBoundPipelineLayout(),
				VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &subMesh.GetTransform());
			vkCmdPushConstants(s_CommandBuffers[VulkanAPI::s_CurrentFrame], VulkanGraphicsPipeline::GetBoundPipelineLayout(),
				VK_SHADER_STAGE_FRAGMENT_BIT, (sizeof(glm::mat4) + sizeof(glm::vec4)), sizeof(uint32_t), &textureIndex);
			vkCmdDrawIndexed(s_CommandBuffers[VulkanAPI::s_CurrentFrame], subMesh.IndexCount, 1, subMesh.IndexOffset, 0, 0);
		}
	}

	void VulkanRenderCommand::SetClearColorImpl(float r, float g, float b, float a)
	{
		s_ClearValues[0] = { r, g, b, a };
	}
}