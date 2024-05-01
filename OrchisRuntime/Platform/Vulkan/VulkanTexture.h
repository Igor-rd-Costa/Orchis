#pragma once
#include <vulkan/vulkan.h>
#include "Renderer/Texture.h"

namespace Orchis {

	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture(const std::string& path, uint32_t index);
		VulkanTexture(const std::vector<char>& bytes, uint32_t width, uint32_t height, uint32_t channelCount, uint32_t index);
		~VulkanTexture();

		VkDescriptorImageInfo GetDescriptorInfo() const;

		virtual void Load(const char* path) override;
		virtual void Bind() const override;

		VkShaderStageFlags GetStageFlags() const { return m_StageFlags; }
	private:
		void DestroyTexture();
		uint32_t m_Width, m_Heigth;
		int m_ChannelCount;
		VkShaderStageFlags m_StageFlags;
		VkImage m_Image;
		VkDeviceMemory m_ImageMemory;
		VkImageView m_ImageView;
	};
}