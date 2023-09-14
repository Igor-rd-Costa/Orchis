#include "OrchisPCH.h"
#include "IndexBuffer.h"
#include "Platform/Vulkan/VulkanIndexBuffer.h"

namespace Orchis {

	IndexBuffer* IndexBuffer::Create(void* data, size_t size, IndexType type)
	{
		return new VulkanIndexBuffer(data, size, type);
	}

}