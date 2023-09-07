#include "OrchisPCH.h"
#include "VulkanDescriptorSets.h"
#include "VulkanUniformBuffer.h"
#include "VulkanTexture.h"
#include "VulkanAPI.h"

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


	VkDescriptorSetLayout VulkanDescriptorSetManager::CreateImageSamplersDescriptorSetLayout(uint32_t textureCount, Texture** pTextures)
	{
		std::vector<VkDescriptorSetLayoutBinding> bindings(textureCount);
		for (uint32_t i = 0; i < textureCount; i++)
		{
			VulkanTexture* texture = reinterpret_cast<VulkanTexture*>(pTextures[i]);
			bindings[i].binding = texture->GetBinding();
			bindings[i].descriptorCount = 1;
			bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			bindings[i].pImmutableSamplers = nullptr;
			bindings[i].stageFlags = texture->GetStageFlags();
		}
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = textureCount;
		layoutInfo.flags = 0;
		layoutInfo.pBindings = bindings.data();
		layoutInfo.pNext = nullptr;

		VkResult result = vkCreateDescriptorSetLayout(VulkanAPI::GetDevice(), &layoutInfo, nullptr, &s_ImageSamplersDescriptorSetLayout);
		OC_ASSERT(result == VK_SUCCESS);

		std::vector<VkDescriptorPoolSize> poolSizes(textureCount);
		for (uint32_t i = 0; i < poolSizes.size(); i++)
		{
			poolSizes[i].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			poolSizes[i].descriptorCount = VulkanAPI::s_MaxFramesInFlight;
		}

		VkDescriptorPoolCreateInfo poolCreateInfo{};
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolCreateInfo.pPoolSizes = poolSizes.data();
		poolCreateInfo.maxSets = VulkanAPI::s_MaxFramesInFlight;

		vkCreateDescriptorPool(VulkanAPI::GetDevice(), &poolCreateInfo, nullptr, &s_ImageSamplersPool);

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

		for (uint32_t i = 0; i < VulkanAPI::s_MaxFramesInFlight; i++)
		{
			std::vector<VkDescriptorImageInfo> imageInfos(textureCount);
			std::vector<VkWriteDescriptorSet> descriptorWrites(textureCount);
			for (uint32_t j = 0; j < descriptorWrites.size(); j++)
			{
				imageInfos[j] = reinterpret_cast<VulkanTexture*>(pTextures[j])->GetDescriptorInfo();

				descriptorWrites[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrites[j].dstSet = s_ImageSamplersDescriptorSets[i];
				descriptorWrites[j].dstBinding = reinterpret_cast<VulkanTexture*>(pTextures[j])->GetBinding();
				descriptorWrites[j].dstArrayElement = 0;
				descriptorWrites[j].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				descriptorWrites[j].descriptorCount = 1;
				descriptorWrites[j].pImageInfo = &imageInfos[j];
			}

			vkUpdateDescriptorSets(VulkanAPI::GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}

		return s_ImageSamplersDescriptorSetLayout;
	}

	void VulkanDescriptorSetManager::DeleteUniformBuffersDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(VulkanAPI::GetDevice(), s_UniformBuffersDescriptorSetLayout, nullptr);
	}

	void VulkanDescriptorSetManager::UpdateImageSampler(Texture* texture)
	{
		vkDeviceWaitIdle(VulkanAPI::GetDevice());
		for (uint32_t i = 0; i < VulkanAPI::s_MaxFramesInFlight; i++)
		{
			VkDescriptorImageInfo imageInfo = reinterpret_cast<VulkanTexture*>(texture)->GetDescriptorInfo();

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = s_ImageSamplersDescriptorSets[i];
			descriptorWrite.dstBinding = reinterpret_cast<VulkanTexture*>(texture)->GetBinding();
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &imageInfo;		
			
			vkUpdateDescriptorSets(VulkanAPI::GetDevice(), 1, &descriptorWrite, 0, nullptr);
		}
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
		vkCmdBindDescriptorSets(VulkanAPI::GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanGraphicsPipeline::GetBoundPipelineLayout(), 0, 1, VulkanDescriptorSetManager::GetUniformBuffersDescriptorSet(VulkanAPI::s_CurrentFrame), 0, nullptr);
		vkCmdBindDescriptorSets(VulkanAPI::GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanGraphicsPipeline::GetBoundPipelineLayout(), 1, 1, VulkanDescriptorSetManager::GetImageSamplersDescriptorSet(VulkanAPI::s_CurrentFrame), 0, nullptr);
	}
}