#pragma once
#include "Macros.h"
#include <glm/glm.hpp>

namespace Orchis {

	struct DefaultVertex
	{
		glm::vec3 Position = {0.0f, 0.0f, 0.0f};
		glm::vec3 Normals = { 0.0f, 0.0f, 0.0f };
		glm::vec2 TexCoord = { 0.0f, 0.0f };
	};

	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		virtual ~VertexBuffer() = default;

		virtual void Bind(size_t offset = 0) = 0;
		

		static VertexBuffer* Create(void* data, size_t size);
	};
}