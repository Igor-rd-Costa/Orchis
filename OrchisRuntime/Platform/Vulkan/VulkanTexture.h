#pragma once
#include <vulkan/vulkan.h>
#include "Renderer/Texture.h"

namespace Orchis {

	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture(const std::string& path, uint32_t index);
		~VulkanTexture();

		VkDescriptorImageInfo GetDescriptorInfo() const;

		virtual void Load(const char* path) override;
		virtual void Bind() const override;

		uint32_t GetIndex() const { return m_Index; }
		VkShaderStageFlags GetStageFlags() const { return m_StageFlags; }
	private:
		void DestroyTexture();
		uint32_t m_Width, m_Heigth;
		int m_ChannelCount;
		uint32_t m_Index;
		VkShaderStageFlags m_StageFlags;
		VkImage m_Image;
		VkDeviceMemory m_ImageMemory;
		VkImageView m_ImageView;
	};
}