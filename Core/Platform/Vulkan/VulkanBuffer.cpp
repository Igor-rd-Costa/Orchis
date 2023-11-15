#include "VulkanBuffer.h"
#include "Macros.h"
#include "VulkanAPI.h"
namespace Orchis {

	VulkanBuffer::VulkanBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
	{
		VkBufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = size;
		createInfo.usage = usage;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkResult result = vkCreateBuffer(VulkanAPI::GetDevice(), &createInfo, nullptr, &m_Buffer);
		OC_ASSERT(result == VK_SUCCESS);

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(VulkanAPI::GetDevice(), m_Buffer, &memoryRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = VulkanAPI::FindMemoryType(memoryRequirements.memoryTypeBits, properties);

		result = vkAllocateMemory(VulkanAPI::GetDevice(), &allocInfo, nullptr, &m_BufferMemory);
		OC_ASSERT(result == VK_SUCCESS);

		vkBindBufferMemory(VulkanAPI::GetDevice(), m_Buffer, m_BufferMemory, 0);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		vkQueueWaitIdle(VulkanAPI::GetGraphicsQueue());
		vkDestroyBuffer(VulkanAPI::GetDevice(), m_Buffer, nullptr);
		vkFreeMemory(VulkanAPI::GetDevice(), m_BufferMemory, nullptr);
	}

}
