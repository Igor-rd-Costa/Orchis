#pragma once
#include "Macros.h"
#include <glm/glm.hpp>

namespace Orchis {

	struct DefaultVertex
	{
		glm::vec3 Position;
		glm::vec3 Normals;
		glm::vec2 TexCoord;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		virtual ~VertexBuffer() = default;

		virtual void Bind() = 0;
		

		static Ref<VertexBuffer> Create(void* data, size_t size);
	};
}