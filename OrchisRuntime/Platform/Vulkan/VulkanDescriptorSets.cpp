#include "VulkanDescriptorSets.h"
#include "VulkanUniformBuffer.h"
#include "VulkanTexture.h"
#include "VulkanAPI.h"
#include "VulkanRenderCommand.h"
#include "ImageSamplerManager.h"

namespace Orchis {

	std::array<VkDescriptorSet, 2> VulkanDescriptorSetManager::s_UniformBuffersDescriptorSets;
	VkDescriptorSetLayout VulkanDescriptorSetManager::s_UniformBuffersDescriptorSetLayout;
	std::array<VkDescriptorSet, 2> VulkanDescriptorSetManager::s_ImageSamplersDescriptorSets;
	VkDescriptorSetLayout VulkanDescriptorSetManager::s_ImageSamplersDescriptorSetLayout;
	VkDescriptorPool VulkanDescriptorSetManager::s_UniformBuffersPool, VulkanDescriptorSetManager::s_ImageSamplersPool;

	VkDescriptorSetLayout VulkanDescriptorSetManager::CreateUniformBuffersDescriptorSetLayout(uint32_t uniformBufferCount, UniformBuffer** pUniformBuffers)
	{
		std::vector<VkDescriptorSetLayoutBinding> bindings(uniformBufferCount);
		for (uint32_t i = 0; i < uniformBufferCount; i++)
		{
			VulkanUniformBuffer* uniformBuffer = reinterpret_cast<VulkanUniformBuffer*>(pUniformBuffers[i]);
			bindings[i].binding = uniformBuffer->GetBinding();
			bindings[i].descriptorCount = 1;
			bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			bindings[i].pImmutableSamplers = nullptr;
			bindings[i].stageFlags = uniformBuffer->GetStageFlags();
		}
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = uniformBufferCount;
		layoutInfo.flags = 0;
		layoutInfo.pBindings = bindings.data();
		layoutInfo.pNext = nullptr;

		VkResult result = vkCreateDescriptorSetLayout(VulkanAPI::GetDevice(), &layoutInfo, nullptr, &s_UniformBuffersDescriptorSetLayout);
		OC_ASSERT(result == VK_SUCCESS);

		std::vector<VkDescriptorPoolSize> poolSizes(uniformBufferCount);
		for (uint32_t i = 0; i < poolSizes.size(); i++)
		{
			poolSizes[i].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSizes[i].descriptorCount = VulkanAPI::s_MaxFramesInFlight;
		}

		VkDescriptorPoolCreateInfo poolCreateInfo{};
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolCreateInfo.pPoolSizes = poolSizes.data();
		poolCreateInfo.maxSets = VulkanAPI::s_MaxFramesInFlight;

		vkCreateDescriptorPool(VulkanAPI::GetDevice(), &poolCreateInfo, nullptr, &s_UniformBuffersPool);

		std::array<VkDescriptorSetLayout, VulkanAPI::s_MaxFramesInFlight> layouts = { 
			s_UniformBuffersDescriptorSetLayout,
			s_UniformBuffersDescriptorSetLayout 
		};
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = s_UniformBuffersPool;
		allocInfo.descriptorSetCount = VulkanAPI::s_MaxFramesInFlight;
		allocInfo.pSetLayouts = layouts.data();

		vkAllocateDescriptorSets(VulkanAPI::GetDevice(), &allocInfo, s_UniformBuffersDescriptorSets.data());

		for (uint32_t i = 0; i < VulkanAPI::s_MaxFramesInFlight; i++)
		{
			std::vector<VkDescriptorBufferInfo> bufferInfos(uniformBufferCount);
			std::vector<VkWriteDescriptorSet> descriptorWrites(uniformBufferCount);
			for (uint32_t j = 0; j < descriptorWrites.size(); j++)
			{
				bufferInfos[j].offset = 0;
				bufferInfos[j].buffer = reinterpret_cast<VulkanUniformBuffer*>(pUniformBuffers[j])->GetBuffers()[i].Buffer;
				bufferInfos[j].range = reinterpret_cast<VulkanUniformBuffer*>(pUniformBuffers[j])->GetSize();

				descriptorWrites[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrites[j].dstSet = s_UniformBuffersDescriptorSets[i];
				descriptorWrites[j].dstBinding = reinterpret_cast<VulkanUniformBuffer*>(pUniformBuffers[j])->GetBinding();
				descriptorWrites[j].dstArrayElement = 0;
				descriptorWrites[j].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				descriptorWrites[j].descriptorCount = 1;
				descriptorWrites[j].pBufferInfo = &bufferInfos[j];
			}

			vkUpdateDescriptorSets(VulkanAPI::GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}

		return s_UniformBuffersDescriptorSetLayout;
	}


	VkDescriptorSetLayout VulkanDescriptorSetManager::CreateImageSamplersDescriptorSetLayout()
	{
		VkDescriptorSetLayoutBinding binding{};
		binding.binding = 0;
		binding.descriptorCount = 100;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		binding.pImmutableSamplers = nullptr;
		binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		
		VkDescriptorBindingFlags bindingFlag = VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT | VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT;

		VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsCreateInfo{};
		bindingFlagsCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
		bindingFlagsCreateInfo.bindingCount = 1;
		bindingFlagsCreateInfo.pBindingFlags = &bindingFlag;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
		layoutInfo.pBindings = &binding;
		layoutInfo.pNext = &bindingFlagsCreateInfo;

		VkResult result = vkCreateDescriptorSetLayout(VulkanAPI::GetDevice(), &layoutInfo, nullptr, &s_ImageSamplersDescriptorSetLayout);
		OC_ASSERT(result == VK_SUCCESS);

		CreateImageSamplersDescriptorPool();
		CreateImageSamplersDescriptorSet();

		return s_ImageSamplersDescriptorSetLayout;
	}

	void VulkanDescriptorSetManager::CreateImageSamplersDescriptorPool()
	{
		VkDescriptorPoolSize poolSize;
		poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSize.descriptorCount = 32 * VulkanAPI::s_MaxFramesInFlight;

		VkDescriptorPoolCreateInfo poolCreateInfo{};
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolCreateInfo.poolSizeCount = 1;
		poolCreateInfo.pPoolSizes = &poolSize;
		poolCreateInfo.maxSets = VulkanAPI::s_MaxFramesInFlight * 50;
		poolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

		vkCreateDescriptorPool(VulkanAPI::GetDevice(), &poolCreateInfo, nullptr, &s_ImageSamplersPool);
	}

	void VulkanDescriptorSetManager::CreateImageSamplersDescriptorSet()
	{
		std::array<VkDescriptorSetLayout, VulkanAPI::s_MaxFramesInFlight> layouts = {
			s_ImageSamplersDescriptorSetLayout,
			s_ImageSamplersDescriptorSetLayout
		};

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = s_ImageSamplersPool;
		allocInfo.descriptorSetCount = VulkanAPI::s_MaxFramesInFlight;
		allocInfo.pSetLayouts = layouts.data();

		vkAllocateDescriptorSets(VulkanAPI::GetDevice(), &allocInfo, s_ImageSamplersDescriptorSets.data());
	}

	void VulkanDescriptorSetManager::DeleteUniformBuffersDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(VulkanAPI::GetDevice(), s_UniformBuffersDescriptorSetLayout, nullptr);
	}

	bool a = false;
	void VulkanDescriptorSetManager::UpdateImageSampler(const Texture* texture, uint32_t index)
	{
		vkDeviceWaitIdle(VulkanAPI::GetDevice());
		VkDescriptorImageInfo imageInfo = reinterpret_cast<const VulkanTexture*>(texture)->GetDescriptorInfo();
		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = s_ImageSamplersDescriptorSets[VulkanAPI::s_CurrentFrame];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = index;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(VulkanAPI::GetDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	void VulkanDescriptorSetManager::DeleteUniformBuffersDescriptorPool()
	{
		vkDestroyDescriptorPool(VulkanAPI::GetDevice(), s_UniformBuffersPool, nullptr);
	}

	void VulkanDescriptorSetManager::DeleteImageSamplersDescriptorPool()
	{
		vkDestroyDescriptorPool(VulkanAPI::GetDevice(), s_ImageSamplersPool, nullptr);
	}

	void VulkanDescriptorSetManager::DeleteImageSamplersDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(VulkanAPI::GetDevice(), s_ImageSamplersDescriptorSetLayout, nullptr);
	}

	void VulkanDescriptorSetManager::BindDescriptorSets()
	{
		vkCmdBindDescriptorSets(VulkanRenderCommand::GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanGraphicsPipeline::GetBoundPipelineLayout(), 0, 1, VulkanDescriptorSetManager::GetUniformBuffersDescriptorSet(VulkanAPI::s_CurrentFrame), 0, nullptr);
		vkCmdBindDescriptorSets(VulkanRenderCommand::GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanGraphicsPipeline::GetBoundPipelineLayout(), 1, 1, VulkanDescriptorSetManager::GetImageSamplersDescriptorSet(VulkanAPI::s_CurrentFrame), 0, nullptr);
	}
}