#include "OrchisPCH.h"
#include "VulkanStagingBuffer.h"
#include "VulkanAPI.h"

namespace Orchis {

	size_t VulkanStagingBuffer::m_Offset = 0;
	size_t VulkanStagingBuffer::m_Capacity = 0;
	VkBuffer VulkanStagingBuffer::m_Buffer;
	VkDeviceMemory VulkanStagingBuffer::m_BufferMemory;
	
	void VulkanStagingBuffer::Init(size_t size)
	{
		m_Capacity = size;
		VkBufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = size;
		createInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkResult result = vkCreateBuffer(VulkanAPI::GetDevice(), &createInfo, nullptr, &m_Buffer);
		OC_ASSERT(result == VK_SUCCESS);

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(VulkanAPI::GetDevice(), m_Buffer, &memoryRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = VulkanAPI::FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		result = vkAllocateMemory(VulkanAPI::GetDevice(), &allocInfo, nullptr, &m_BufferMemory);
		OC_ASSERT(result == VK_SUCCESS);

		vkBindBufferMemory(VulkanAPI::GetDevice(), m_Buffer, m_BufferMemory, 0);
	}

	void VulkanStagingBuffer::Shutdown()
	{
		vkDestroyBuffer(VulkanAPI::GetDevice(), m_Buffer, nullptr);
		vkFreeMemory(VulkanAPI::GetDevice(), m_BufferMemory, nullptr);
	}

	void VulkanStagingBuffer::Allocate(void* data, size_t size, VkBuffer destination)
	{
		void* memoryPtr;
		if (size > m_Capacity)
		{	
			OC_ASSERT(false); // TODO Implement resizing
		}
		else if ((m_Offset + size) > m_Capacity)
		{
			m_Offset = 0;
		}

		vkMapMemory(VulkanAPI::GetDevice(), m_BufferMemory, m_Offset, size, 0, &memoryPtr);
		memcpy(memoryPtr, data, size);
		vkUnmapMemory(VulkanAPI::GetDevice(), m_BufferMemory);
		
		
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();
		
		VkBufferCopy copyInfo{};
		copyInfo.srcOffset = m_Offset;
		copyInfo.dstOffset = 0;
		copyInfo.size = size;
		vkCmdCopyBuffer(commandBuffer, m_Buffer, destination, 1, &copyInfo);

		EndSingleTimeCommands(commandBuffer);
		
		m_Offset += size;
	}

	void VulkanStagingBuffer::Allocate(void* data, size_t size, VkImage destination, uint32_t width, uint32_t height)
	{
		if (size > m_Capacity)
		{
			OC_ASSERT(false); //TODO Implement resizing
		}
		else if ((m_Offset + size) > m_Capacity)
		{
			m_Offset = 0;
		}

		void* memoryPtr;
		vkMapMemory(VulkanAPI::GetDevice(), m_BufferMemory, m_Offset, size, 0, &memoryPtr);
		memcpy(memoryPtr, data, size);
		vkUnmapMemory(VulkanAPI::GetDevice(), m_BufferMemory);

		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkBufferImageCopy copyInfo{};
		copyInfo.bufferOffset = m_Offset;
		copyInfo.bufferImageHeight = 0;
		copyInfo.bufferRowLength = 0;
		copyInfo.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copyInfo.imageSubresource.baseArrayLayer = 0;
		copyInfo.imageSubresource.layerCount = 1;
		copyInfo.imageSubresource.mipLevel = 0;
		copyInfo.imageOffset = { 0, 0, 0 };
		copyInfo.imageExtent = { width, height, 1 };

		vkCmdCopyBufferToImage(commandBuffer, m_Buffer, destination, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyInfo);

		EndSingleTimeCommands(commandBuffer);

		m_Offset += size;
	}

}
