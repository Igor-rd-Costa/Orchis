#include "OrchisPCH.h"
#include "VulkanIndexBuffer.h"
#include "VulkanAPI.h"
#include "VulkanStagingBuffer.h"
namespace Orchis {

	VulkanIndexBuffer::VulkanIndexBuffer(void* data, size_t size, IndexType type)
		: VulkanBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT), m_Type(VK_INDEX_TYPE_MAX_ENUM)
	{
		if (type == INDEX_TYPE_UINT16)
			m_Type = VK_INDEX_TYPE_UINT16;
		else if (type == INDEX_TYPE_UINT32)
			m_Type = VK_INDEX_TYPE_UINT32;
		else OC_ASSERT(false);

		VulkanStagingBuffer::Allocate(data, size, m_Buffer);
	}
	void VulkanIndexBuffer::Bind()
	{
		vkCmdBindIndexBuffer(VulkanAPI::GetCommandBuffer(), m_Buffer, 0, m_Type);
	}
}