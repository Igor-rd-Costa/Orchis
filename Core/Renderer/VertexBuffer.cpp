#include "OrchisPCH.h"
#include "VertexBuffer.h"
#include "Platform/Vulkan/VulkanVertexBuffer.h"

namespace Orchis {

	Ref<VertexBuffer> VertexBuffer::Create(void* data, size_t size)
	{
		return CreateRef<VulkanVertexBuffer>(data, size);
	}
}
