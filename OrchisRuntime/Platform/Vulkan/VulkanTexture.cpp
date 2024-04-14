#include "VulkanTexture.h"
#include "VulkanAPI.h"
#include "VulkanStagingBuffer.h"
#include "ImageSamplerManager.h"
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "VulkanDescriptorSets.h"

namespace Orchis {

	VulkanTexture::VulkanTexture(const std::string& path, uint32_t index)
		: m_Index(index), m_StageFlags(VK_SHADER_STAGE_FRAGMENT_BIT)
	{
		stbi_uc* pixels = stbi_load(path.c_str(), reinterpret_cast<int*>(&m_Width), reinterpret_cast<int*>(&m_Heigth), &m_ChannelCount, STBI_rgb_alpha);
		if (!pixels)
			OC_ASSERT(false);

		VkDeviceSize imageSize = m_Width * m_Heigth * 4;

		CreateImage(m_Width, m_Heigth,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_Image, m_ImageMemory);

		TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		VulkanStagingBuffer::Allocate(pixels, imageSize, m_Image, m_Width, m_Heigth);
		TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		m_ImageView = CreateImageView(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
		stbi_image_free(pixels);
	}

	VulkanTexture::~VulkanTexture()
	{
		DestroyTexture();
	}

	VkDescriptorImageInfo VulkanTexture::GetDescriptorInfo() const
	{
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = m_ImageView;
		imageInfo.sampler = ImageSamplerManager::GetSampler();
		return imageInfo;
	}

	void VulkanTexture::Load(const char* path)
	{
		DestroyTexture();

		stbi_uc* pixels = stbi_load(path, reinterpret_cast<int*>(&m_Width), reinterpret_cast<int*>(&m_Heigth), &m_ChannelCount, STBI_rgb_alpha);
		if (!pixels)
			OC_ASSERT(false);

		VkDeviceSize imageSize = m_Width * m_Heigth * 4;

		CreateImage(m_Width, m_Heigth,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_Image, m_ImageMemory);

		TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		VulkanStagingBuffer::Allocate(pixels, imageSize, m_Image, m_Width, m_Heigth);
		TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		m_ImageView = CreateImageView(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
		stbi_image_free(pixels);

		VulkanDescriptorSetManager::UpdateImageSampler(this);
	}

	void VulkanTexture::Bind() const
	{
		VulkanDescriptorSetManager::UpdateImageSampler(this);
	}

	void VulkanTexture::DestroyTexture()
	{
		vkDeviceWaitIdle(VulkanAPI::GetDevice());
		vkDestroyImageView(VulkanAPI::GetDevice(), m_ImageView, nullptr);
		vkDestroyImage(VulkanAPI::GetDevice(), m_Image, nullptr);
		vkFreeMemory(VulkanAPI::GetDevice(), m_ImageMemory, nullptr);
	}

}
