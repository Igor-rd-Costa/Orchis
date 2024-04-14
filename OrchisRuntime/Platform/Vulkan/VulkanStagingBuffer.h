#pragma once
#include <vulkan/vulkan.h>
#include "VulkanBuffer.h"

namespace Orchis {

	class VulkanStagingBuffer
	{
	public:
		static void Init(size_t size);
		static void Shutdown();

		static void Allocate(void* data, size_t size, VkBuffer destination);
		static void Allocate(void* data, size_t size, VkImage destination, uint32_t width, uint32_t height);

	private:
		static size_t m_Capacity;
		static size_t m_Offset;
		static VkBuffer m_Buffer;
		static VkDeviceMemory m_BufferMemory;
	};
}