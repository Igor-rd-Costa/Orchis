#pragma once
#include <unordered_map>
#include <string>
#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Utils/UUID.h"
#include "Mesh.h"

namespace Orchis {

	struct MeshData
	{
		VertexBuffer* VertexBuffer;
		IndexBuffer* IndexBuffer;
	};


	class AssetManager
	{
	public:
		AssetManager() = delete;
		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;

		static void Init();
		static void ShutDown();
		static Mesh* LoadMesh(const UUID& meshId);
		static Texture* LoadTexture(const UUID& textureId);
		static Texture* LoadDefaultTexture();

	private:
		static void CreateDefaultTexture();
		static Texture* s_DefaultTexture;
		static std::unordered_map<UUID, Texture*> s_Textures;
		static std::unordered_map<UUID, Mesh*> s_Meshes;
		static const uint32_t s_MaxIndexedTextures = 100;
		static uint32_t s_CurrentIndex;
	};
}