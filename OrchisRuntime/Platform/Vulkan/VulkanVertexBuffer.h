#pragma once
#include "Renderer/VertexBuffer.h"
#include <vulkan/vulkan.h>
#include "VulkanBuffer.h"

namespace Orchis {

	class VulkanVertexBuffer : public VertexBuffer, public VulkanBuffer
	{
	public:
		VulkanVertexBuffer(void* data, size_t size);
		~VulkanVertexBuffer();

		virtual void Bind(size_t offset = 0) override;
	};
}