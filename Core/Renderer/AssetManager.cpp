#include "AssetManager.h"
#include "Renderer/Flags.h"

namespace Orchis {

	std::unordered_map<std::string, Texture*> AssetManager::s_Textures;
	std::unordered_map<std::string, MeshData> AssetManager::s_MeshData;
	const uint32_t AssetManager::s_MaxIndexedTextures;
	uint32_t AssetManager::s_CurrentIndex = 0;

	void AssetManager::ShutDown()
	{
		for (auto& data : s_MeshData)
		{
			delete data.second.VertexBuffer;
			delete data.second.IndexBuffer;
		}

		for (auto& texture : s_Textures)
			delete texture.second;
	}

	void AssetManager::RegisterBuffers(const std::string& modelPath, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer)
	{
		s_MeshData.insert({ modelPath, { vertexBuffer, indexBuffer } });
	}

	MeshData* AssetManager::GetMeshData(const std::string& modelPath)
	{
		auto buffers = s_MeshData.find(modelPath);
		if (buffers == s_MeshData.end())
		{
			return nullptr;
		}

		return &buffers->second;
	}

	Texture* AssetManager::GetTexture(const std::string& filePath)
	{
		auto texture = s_Textures.find(filePath);
		if (texture == s_Textures.end())
		{
			return s_Textures.insert({ filePath, Texture::Create(filePath, s_CurrentIndex++)}).first->second;
		}

		return texture->second;
	}

}