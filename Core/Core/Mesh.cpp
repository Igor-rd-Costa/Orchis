#include "OrchisPCH.h"
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Renderer/AssetManager.h"


namespace Orchis {
	
	Mesh::Mesh(const char* filePath, const glm::vec3& position)
		: m_VertexBuffer(nullptr), m_IndexBuffer(nullptr)
	{
		Assimp::Importer importer;
		DefaultVertex* vertices = nullptr;
		IndexType indexType = INDEX_TYPE_NONE;
		void* indicesBase = nullptr;
		
		const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_MakeLeftHanded | aiProcess_FlipUVs);
		
		if (scene)
		{
			uint32_t vertexCount = 0;
			uint32_t highestIndexCount = 0;
			uint32_t faceCount = 0;
			for (uint32_t i = 0; i < scene->mNumMeshes; i++)
			{
				vertexCount += scene->mMeshes[i]->mNumVertices;

				aiFace* lastFace = &scene->mMeshes[i]->mFaces[scene->mMeshes[i]->mNumFaces - 1];
				faceCount += scene->mMeshes[i]->mNumFaces;
				highestIndexCount += lastFace->mIndices[lastFace->mNumIndices - 1];
			}
			vertices = new DefaultVertex[vertexCount];
			if (highestIndexCount > UINT16_MAX)
			{
				indexType = INDEX_TYPE_UINT32;
				indicesBase = reinterpret_cast<void*>(new uint32_t[faceCount * 3]);
			}
			else
			{
				indexType = INDEX_TYPE_UINT16;
				indicesBase = reinterpret_cast<void*>(new uint16_t[faceCount * 3]);
			}
			
			uint32_t indicesCount = 0;
			uint32_t vertOffset= 0;
			uint32_t indOffset = 0;
			void* indices = indicesBase;
			m_SubMeshes.resize(scene->mNumMeshes);
			for (uint32_t i = 0; i < scene->mNumMeshes; i++)
			{
 				aiMesh* mesh = scene->mMeshes[i];
				for (uint32_t j = 0; j < mesh->mNumVertices; j++)
				{
					vertices[j + vertOffset].Position.x = mesh->mVertices[j].x;
					vertices[j + vertOffset].Position.y = mesh->mVertices[j].z;
					vertices[j + vertOffset].Position.z = mesh->mVertices[j].y;
					vertices[j + vertOffset].Normals.x = mesh->mNormals[j].x;
					vertices[j + vertOffset].Normals.y = mesh->mNormals[j].z;
					vertices[j + vertOffset].Normals.z = mesh->mNormals[j].y;
					vertices[j + vertOffset].TexCoord.x = mesh->mTextureCoords[0][j].x;
					vertices[j + vertOffset].TexCoord.y = mesh->mTextureCoords[0][j].y;
				}
				m_SubMeshes[i].VertexOffset = vertOffset;
				vertOffset += mesh->mNumVertices;
				
				uint32_t indexCount = indicesCount;
				m_SubMeshes[i].IndexOffset = indexCount;
				if (indexType == INDEX_TYPE_UINT16)
				{
					for (uint32_t j = 0; j < mesh->mNumFaces; j++)
					{
						indicesCount += mesh->mFaces[j].mNumIndices;
						for (uint32_t k = 0; k < mesh->mFaces[j].mNumIndices; k++)
						{
							*reinterpret_cast<uint16_t*>(indices) = mesh->mFaces[j].mIndices[k] + indOffset;
							indices = (reinterpret_cast<uint16_t*>(indices) + 1);

							if ((j == mesh->mNumFaces - 1) && k == mesh->mFaces[j].mNumIndices - 1) 
								indOffset += mesh->mFaces[j].mIndices[k] + 1;
						}
					}
				}
				else if (indexType == INDEX_TYPE_UINT32)
				{
					for (uint32_t j = 0; j < mesh->mNumFaces; j++)
					{
						indicesCount += mesh->mFaces[j].mNumIndices;
						for (uint32_t k = 0; k < mesh->mFaces[j].mNumIndices; k++)
						{
							*reinterpret_cast<uint32_t*>(indices) = mesh->mFaces[j].mIndices[k] + indOffset;
							indices = (reinterpret_cast<uint32_t*>(indices) + 1);

							if ((j == mesh->mNumFaces - 1) && k == mesh->mFaces[j].mNumIndices - 1)
								indOffset += mesh->mFaces[j].mIndices[k] + 1;
						}
					}
				}
				m_SubMeshes[i].IndexCount = indicesCount - indexCount;
			}

			for (SubMesh& subMesh : m_SubMeshes)
			{
				subMesh.Move(position);
			}

			MeshData* buffers = AssetManager::GetMeshData(filePath);
			if (buffers)
			{
				m_VertexBuffer = buffers->VertexBuffer;
				m_IndexBuffer = buffers->IndexBuffer;
			}
			else
			{
				m_VertexBuffer = VertexBuffer::Create(vertices, vertexCount * sizeof(DefaultVertex));
				if (indexType == INDEX_TYPE_UINT16)
					m_IndexBuffer = IndexBuffer::Create(indicesBase, indicesCount * sizeof(uint16_t), indexType);
				else if (indexType == INDEX_TYPE_UINT32)
					m_IndexBuffer = IndexBuffer::Create(indicesBase, indicesCount * sizeof(uint32_t), indexType);
				
				AssetManager::RegisterBuffers(filePath, m_VertexBuffer, m_IndexBuffer);
			}
		}

		if (vertices) delete[] vertices;
		if (indexType == INDEX_TYPE_UINT16)
			delete[] reinterpret_cast<uint16_t*>(indicesBase);
		else if (indexType == INDEX_TYPE_UINT32)
			delete[] reinterpret_cast<uint32_t*>(indicesBase);
	}

	void Mesh::Bind() const
	{
		m_VertexBuffer->Bind(0);
		m_IndexBuffer->Bind(0);
	}

	Mesh::~Mesh()
	{
		std::cout << "Destructor!\n";
	}

	void SubMesh::Move(const glm::vec3& positionOffset)
	{
		Position += positionOffset;
		Transform = glm::translate(Transform, positionOffset);
	}

	void SubMesh::Rotate(float angleOffset, const glm::vec3& axisOffset)
	{
		RotationAngle += angleOffset;
		Rotation += axisOffset;
		Transform = glm::rotate(Transform, glm::radians(angleOffset), axisOffset);
	}

	void SubMesh::Scale(const glm::vec3& scaleOffset)
	{
		ScaleAxis += scaleOffset;
		Transform += glm::scale(Transform, scaleOffset);
	}

	const glm::mat4& SubMesh::GetTransform() const
	{
		return Transform;
	}

	void SubMesh::SetTexture(const std::string& filePath)
	{
		TexturePath = filePath;
		Texture = AssetManager::GetTexture(filePath);
	}
}