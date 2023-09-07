#include "OrchisPCH.h"
#include "VulkanVertexBuffer.h"
#include "VulkanAPI.h"
#include "VulkanStagingBuffer.h"
namespace Orchis {

	VulkanVertexBuffer::VulkanVertexBuffer(void* data, size_t size)
		:VulkanBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	{
		VulkanStagingBuffer::Allocate(data, size, m_Buffer);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{

	}

	void VulkanVertexBuffer::Bind()
	{
		static VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(VulkanAPI::GetCommandBuffer(), 0, 1, &m_Buffer, &offset);
	}

}
