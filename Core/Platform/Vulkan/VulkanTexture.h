#pragma once
#include <vulkan/vulkan.h>
#include "Renderer/Texture.h"

namespace Orchis {

	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture(const char* path, uint32_t binding, ShaderStageFlags stageFlags);
		~VulkanTexture();

		VkDescriptorImageInfo GetDescriptorInfo();

		virtual void Load(const char* path) override;

		virtual uint32_t GetBinding() const override { return m_Binding; }
		VkShaderStageFlags GetStageFlags() const { return m_StageFlags; }
	private:
		void DestroyTexture();
		uint32_t m_Width, m_Heigth;
		int m_ChannelCount;
		uint32_t m_Binding;
		VkShaderStageFlags m_StageFlags;
		VkImage m_Image;
		VkDeviceMemory m_ImageMemory;
		VkImageView m_ImageView;
	};
}