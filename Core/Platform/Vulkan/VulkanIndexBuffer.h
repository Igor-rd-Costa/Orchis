#pragma once
#include "Renderer/IndexBuffer.h"
#include "VulkanBuffer.h"

namespace Orchis {

	class VulkanIndexBuffer : public IndexBuffer, public VulkanBuffer
	{
	public:
		VulkanIndexBuffer(void* data, size_t size, IndexType type);
		~VulkanIndexBuffer() = default;

		virtual void Bind() override;
	private:
		VkIndexType m_Type;
	};
}