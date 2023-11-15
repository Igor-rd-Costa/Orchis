#include "VulkanGraphicsPipeline.h"
#include "VulkanAPI.h"
#include "VulkanVertexBuffer.h"
#include "VulkanTexture.h"
#include "VulkanDescriptorSets.h"
namespace Orchis {

	VkPipeline VulkanGraphicsPipeline::s_BoundPipeline;
	VkPipelineLayout VulkanGraphicsPipeline::s_BoundPipelineLayout;

	std::vector<char> ReadFile(std::string_view filePath)
	{
		std::ifstream file(filePath.data(), std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			VulkanAPI::s_Logger.Error("Failed to open file " + std::string(filePath));
			OC_ASSERT(false);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	VulkanGraphicsPipeline::VulkanGraphicsPipeline(GraphicsPipelineCreateInfo* createInfo)
	{
		std::vector<char> vertShaderCode = ReadFile(createInfo->VertexShader);
		std::vector<char> fragShaderCode = ReadFile(createInfo->FragmentShader);

		VkShaderModule vertModule = CreateShaderModule(vertShaderCode);
		VkShaderModule fragModule = CreateShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		std::array<VkDynamicState, 2> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicStateInfo.pDynamicStates = dynamicStates.data();

		VkVertexInputBindingDescription vertexBindingDescriptor;
		std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptor;
		
		if (createInfo->VertexInputLayoutMode == VERTEX_LAYOUT_MODE_DEFAUT)
		{
			vertexBindingDescriptor = GenerateDefaultBindingDescriptors();
			vertexAttributeDescriptor = GenerateDefaultAttributeDescriptors();
		}
		else if (createInfo->VertexInputLayoutMode == VERTEX_LAYOUT_MODE_CUSTOM)
		{
			vertexBindingDescriptor = GenerateCustomBindingDescriptors();
			vertexAttributeDescriptor = GenerateCustomAttributeDescriptors(createInfo->VertexAttribPointerCount, createInfo->pVertexAttribPointers);
		}
		else OC_ASSERT(false);

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &vertexBindingDescriptor;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributeDescriptor.size());
		vertexInputInfo.pVertexAttributeDescriptions = vertexAttributeDescriptor.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizationStateInfo{};
		rasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationStateInfo.depthClampEnable = VK_FALSE;
		rasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizationStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationStateInfo.lineWidth = 1.0f;
		rasterizationStateInfo.cullMode = VK_CULL_MODE_NONE;//VK_CULL_MODE_BACK_BIT;
		rasterizationStateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationStateInfo.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleInfo.sampleShadingEnable = VK_FALSE;
		multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;


		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
		depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilInfo.depthTestEnable = VK_TRUE;
		depthStencilInfo.depthWriteEnable = VK_TRUE;
		depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		depthStencilInfo.stencilTestEnable = VK_FALSE;


		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendInfo.logicOpEnable = VK_FALSE;
		colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		colorBlendInfo.attachmentCount = 1;
		colorBlendInfo.pAttachments = &colorBlendAttachment;
		colorBlendInfo.blendConstants[0] = 0.0f;
		colorBlendInfo.blendConstants[1] = 0.0f;
		colorBlendInfo.blendConstants[2] = 0.0f;
		colorBlendInfo.blendConstants[3] = 0.0f;
		
		std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts = {
			VulkanDescriptorSetManager::CreateUniformBuffersDescriptorSetLayout(createInfo->UniformBufferCount, createInfo->pUniformBuffers),
			VulkanDescriptorSetManager::CreateImageSamplersDescriptorSetLayout()
		};

		std::array<VkPushConstantRange, 2> pushConstants{};
		pushConstants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushConstants[0].size = sizeof(glm::mat4);
		pushConstants[0].offset = 0;
		pushConstants[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstants[1].size = sizeof(glm::vec4) + sizeof(uint32_t);
		pushConstants[1].offset = sizeof(glm::mat4);


		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstants.size());
		pipelineLayoutInfo.pPushConstantRanges = pushConstants.data();

		VkResult result = vkCreatePipelineLayout(VulkanAPI::GetDevice(), &pipelineLayoutInfo, nullptr, &m_Layout);
		OC_ASSERT(result == VK_SUCCESS);

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
		pipelineInfo.pViewportState = &viewportInfo;
		pipelineInfo.pRasterizationState = &rasterizationStateInfo;
		pipelineInfo.pMultisampleState = &multisampleInfo;
		pipelineInfo.pDepthStencilState = &depthStencilInfo;
		pipelineInfo.pColorBlendState = &colorBlendInfo;
		pipelineInfo.pDynamicState = &dynamicStateInfo;
		pipelineInfo.layout = m_Layout;
		pipelineInfo.renderPass = VulkanRenderPassManager::GetRenderPass();
		pipelineInfo.subpass = 0;

		result = vkCreateGraphicsPipelines(VulkanAPI::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline);
		OC_ASSERT(result == VK_SUCCESS);

		vkDestroyShaderModule(VulkanAPI::GetDevice(), vertModule, nullptr);
		vkDestroyShaderModule(VulkanAPI::GetDevice(), fragModule, nullptr);

		VulkanAPI::s_Logger.Info("Graphics pipeline created successfully");
	}

	VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
	{
		vkDestroyPipeline(VulkanAPI::GetDevice(), m_Pipeline, nullptr);
		vkDestroyPipelineLayout(VulkanAPI::GetDevice(), m_Layout, nullptr);
	}

	void VulkanGraphicsPipeline::Bind()
	{
		s_BoundPipeline = m_Pipeline;
		s_BoundPipelineLayout = m_Layout;
	}

	VkVertexInputBindingDescription VulkanGraphicsPipeline::GenerateDefaultBindingDescriptors()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(DefaultVertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::vector<VkVertexInputAttributeDescription> VulkanGraphicsPipeline::GenerateDefaultAttributeDescriptors()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(DefaultVertex, Position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(DefaultVertex, Normals);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(DefaultVertex, TexCoord);
	
		return attributeDescriptions;
	}

	VkVertexInputBindingDescription VulkanGraphicsPipeline::GenerateCustomBindingDescriptors()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(float) * 8;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::vector<VkVertexInputAttributeDescription> VulkanGraphicsPipeline::GenerateCustomAttributeDescriptors(uint32_t count, VertexAttribPointer* pVertexAttribPointers)
	{
		std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions(count);

		uint32_t offset = 0;
		for (uint32_t i = 0; i < vertexAttributeDescriptions.size(); i++)
		{
			std::pair<int, VkFormat> vertexAttribInfo = VertexAttribInfoToVkAttribInfo(pVertexAttribPointers[i]);
			vertexAttributeDescriptions[i].binding = 0;
			vertexAttributeDescriptions[i].location = i;
			vertexAttributeDescriptions[i].offset = offset;
			vertexAttributeDescriptions[i].format = vertexAttribInfo.second;
			offset += vertexAttribInfo.first;
		}

		return vertexAttributeDescriptions;
	}

	VkShaderModule VulkanGraphicsPipeline::CreateShaderModule(const std::vector<char>& code)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		
		VkShaderModule shaderModule;
		VkResult result = vkCreateShaderModule(VulkanAPI::GetDevice(), &createInfo, nullptr, &shaderModule);
		OC_ASSERT(result == VK_SUCCESS);
		return shaderModule;
	}

}