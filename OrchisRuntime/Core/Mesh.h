#pragma once
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Texture.h"
#include <glm/glm.hpp>

namespace Orchis {

	struct OC_API SubMesh
	{
		uint32_t VertexOffset = 0;
		uint32_t IndexOffset = 0;
		uint32_t IndexCount = 0;
	};

	class OC_API Mesh
	{
	public:
		Mesh() = default;
		Mesh(const std::string& filePath);
		~Mesh();

		uint32_t GetIndexCount() const;
		void Bind() const;

		std::vector<SubMesh> m_SubMeshes = {};
	private:
		VertexBuffer* m_VertexBuffer = nullptr;
		IndexBuffer* m_IndexBuffer = nullptr;
	};
}