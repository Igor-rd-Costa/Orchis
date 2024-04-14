#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "Renderer/GraphicsPipeline.h"
#include "VulkanUniformBuffer.h"

namespace Orchis {

	std::vector<char> ReadFile(std::string_view filePath);

	class VulkanGraphicsPipeline : public GraphicsPipeline
	{
	public:
		VulkanGraphicsPipeline(GraphicsPipelineCreateInfo* createInfo);
		~VulkanGraphicsPipeline();

		VkPipeline Get() { return m_Pipeline; }

		virtual void Bind() override;

		static VkPipeline GetBoundPipeline() { return s_BoundPipeline; }
		static VkPipelineLayout GetBoundPipelineLayout() { return s_BoundPipelineLayout; }
	private:
		VkVertexInputBindingDescription GenerateDefaultBindingDescriptors();
		VkVertexInputBindingDescription GenerateCustomBindingDescriptors();
		std::vector<VkVertexInputAttributeDescription> GenerateDefaultAttributeDescriptors();
		std::vector<VkVertexInputAttributeDescription> GenerateCustomAttributeDescriptors(uint32_t count, VertexAttribPointer* pVertexAttribPointers);
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
	private:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_Layout;
		static VkPipeline s_BoundPipeline;
		static VkPipelineLayout s_BoundPipelineLayout;
	};
}