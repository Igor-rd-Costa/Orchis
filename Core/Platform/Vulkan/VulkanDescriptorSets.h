#pragma once
#include <vulkan/vulkan.h>
#include "Renderer/UniformBuffer.h"
#include "Renderer/Texture.h"

#include <array>

namespace Orchis {


	class VulkanDescriptorSetManager
	{
	public:
		VulkanDescriptorSetManager() = delete;
		VulkanDescriptorSetManager(const VulkanDescriptorSetManager&) = delete;
		VulkanDescriptorSetManager(VulkanDescriptorSetManager&&) = delete;
		~VulkanDescriptorSetManager() = delete;

		static VkDescriptorSetLayout CreateUniformBuffersDescriptorSetLayout(uint32_t uniformBufferCount, UniformBuffer** pUniformBuffers);
		static VkDescriptorSetLayout CreateImageSamplersDescriptorSetLayout();
		static void DeleteUniformBuffersDescriptorSetLayout();
		static void DeleteImageSamplersDescriptorSetLayout();
		static void UpdateImageSampler(const Texture* texture);
		static void DeleteUniformBuffersDescriptorPool();
		static void DeleteImageSamplersDescriptorPool();



		static void BindDescriptorSets();

		static VkDescriptorSet* GetUniformBuffersDescriptorSet(uint32_t index) { return &s_UniformBuffersDescriptorSets[index]; }
		static VkDescriptorSet* GetImageSamplersDescriptorSet(uint32_t index) { return &s_ImageSamplersDescriptorSets[index]; }
	private:
		static std::array<VkDescriptorSet, 2> s_UniformBuffersDescriptorSets;
		static VkDescriptorSetLayout s_UniformBuffersDescriptorSetLayout;
		static std::array<VkDescriptorSet, 2> s_ImageSamplersDescriptorSets;
		static VkDescriptorSetLayout s_ImageSamplersDescriptorSetLayout;
		static VkDescriptorPool s_UniformBuffersPool, s_ImageSamplersPool;
	};
}