#include "VertexBuffer.h"
#include "Platform/Vulkan/VulkanVertexBuffer.h"

namespace Orchis {

	VertexBuffer* VertexBuffer::Create(void* data, size_t size)
	{
		return new VulkanVertexBuffer(data, size);
	}
}
