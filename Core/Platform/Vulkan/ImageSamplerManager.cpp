#include "OrchisPCH.h"
#include "Macros.h"
#include "ImageSamplerManager.h"
#include "VulkanAPI.h"

namespace Orchis {

	VkSampler ImageSamplerManager::s_ImageSampler;

	void ImageSamplerManager::Init()
	{
		VkPhysicalDeviceProperties physicalDeviceProperties{}; // TODO query at start of program
		vkGetPhysicalDeviceProperties(VulkanAPI::GetPhysicalDevice(), &physicalDeviceProperties);

		VkSamplerCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.magFilter = VK_FILTER_LINEAR;
		createInfo.minFilter = VK_FILTER_LINEAR;
		createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		createInfo.anisotropyEnable = VK_TRUE;
		createInfo.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy;
		createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		createInfo.unnormalizedCoordinates = VK_FALSE;
		createInfo.compareEnable = VK_FALSE;
		createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		VkResult result = vkCreateSampler(VulkanAPI::GetDevice(), &createInfo, nullptr, &s_ImageSampler);
		OC_ASSERT(result == VK_SUCCESS);
	}

	void ImageSamplerManager::Shutdown()
	{
		vkDestroySampler(VulkanAPI::GetDevice(), s_ImageSampler, nullptr);
	}
}