#pragma once
#include "Renderer/IndexBuffer.h"
#include "VulkanBuffer.h"

namespace Orchis {

	class VulkanIndexBuffer : public IndexBuffer, public VulkanBuffer
	{
	public:
		VulkanIndexBuffer(void* data, size_t size, IndexType type);
		~VulkanIndexBuffer() = default;

		virtual void Bind(size_t offset) const override;
		virtual uint32_t GetIndexCount() const override { return m_IndexCount; }
	private:
		VkIndexType m_Type;
		uint32_t m_IndexCount;
	};
}