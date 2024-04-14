#pragma once
#include <vulkan/vulkan.h>

namespace Orchis {

	class VulkanBuffer
	{
	public:
		VulkanBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		virtual ~VulkanBuffer();

		
	protected:
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;

	private:
	};
}