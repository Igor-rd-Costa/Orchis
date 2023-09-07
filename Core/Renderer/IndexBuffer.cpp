#include "OrchisPCH.h"
#include "IndexBuffer.h"
#include "Platform/Vulkan/VulkanIndexBuffer.h"

namespace Orchis {

	Ref<IndexBuffer> IndexBuffer::Create(void* data, size_t size, IndexType type)
	{
		return CreateRef<VulkanIndexBuffer>(data, size, type);
	}

}