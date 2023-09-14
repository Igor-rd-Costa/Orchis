#pragma once
#include "Macros.h"

namespace Orchis {

	enum IndexType
	{
		INDEX_TYPE_NONE, INDEX_TYPE_UINT16, INDEX_TYPE_UINT32
	};

	class IndexBuffer
	{
	public:
		IndexBuffer() = default;
		virtual ~IndexBuffer() = default;
		
		virtual void Bind(size_t offset) const = 0;
		virtual uint32_t GetIndexCount() const = 0;

		static IndexBuffer* Create(void* data, size_t size, IndexType type);
	};
}