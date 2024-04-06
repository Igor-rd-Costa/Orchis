#pragma once
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Texture.h"
#include <glm/glm.hpp>

namespace Orchis {

	struct OC_API SubMesh
	{
		const glm::mat4& GetTransform() const;

		Texture* GetTexture() const { return Texture; }

		void SetTexture(const std::string& filePath);
		void Move(const glm::vec3& positionOffset);
		void Rotate(float angleOffset, const glm::vec3& axisOffset);
		void Scale(const glm::vec3& scaleOffset);


		uint32_t VertexOffset = 0;
		uint32_t IndexOffset = 0;
		uint32_t IndexCount = 0;
		Texture* Texture = nullptr;
		std::string TexturePath;
		glm::vec3 Position = { 0.0f, 0.0f , 0.0f };
		float RotationAngle = 0.0f;
		glm::vec3 Rotation = { 1.0f, 1.0f, 1.0f };
		glm::vec3 ScaleAxis = { 1.0f, 1.0f, 1.0f };
		glm::mat4 Transform = glm::mat4(1.0f);
	};

	class OC_API Mesh
	{
	public:
		Mesh(const char* filePath, const glm::vec3& position = { 0.0f, 0.0f, 0.0f });
		~Mesh();

		uint32_t GetIndexCount() const { return m_IndexBuffer->GetIndexCount(); };
		const std::string& GetPath() const { return m_MeshPath; }
		void Bind() const;

		std::vector<SubMesh> m_SubMeshes;
	private:
		std::string m_MeshPath;
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;
	};
}