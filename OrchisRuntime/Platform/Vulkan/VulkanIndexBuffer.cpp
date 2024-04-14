#include "VulkanIndexBuffer.h"
#include "VulkanAPI.h"
#include "VulkanStagingBuffer.h"
#include "VulkanRenderCommand.h"
namespace Orchis {

	VulkanIndexBuffer::VulkanIndexBuffer(void* data, size_t size, IndexType type)
		: VulkanBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		m_Type(VK_INDEX_TYPE_MAX_ENUM), m_IndexCount(0)
	{
		if (type == INDEX_TYPE_UINT16)
		{
			m_Type = VK_INDEX_TYPE_UINT16;
			m_IndexCount = static_cast<uint32_t>(size / sizeof(uint16_t));
		}
		else if (type == INDEX_TYPE_UINT32)
		{
			m_Type = VK_INDEX_TYPE_UINT32;
			m_IndexCount = static_cast<uint32_t>(size / sizeof(uint32_t));
		}
		else OC_ASSERT(false);

		VulkanStagingBuffer::Allocate(data, size, m_Buffer);
	}
	void VulkanIndexBuffer::Bind(size_t offset) const
	{
		vkCmdBindIndexBuffer(VulkanRenderCommand::GetCommandBuffer(), m_Buffer, static_cast<VkDeviceSize>(offset), m_Type);
	}
}