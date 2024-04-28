#include <filesystem>
#include "AssetManager.h"
#include "Renderer/Flags.h"
#include "VFS/FS.h"
namespace Orchis {

	std::unordered_map<std::string, Texture*> AssetManager::s_Textures;
	std::unordered_map<UUID, Mesh*> AssetManager::s_Meshes;
	const uint32_t AssetManager::s_MaxIndexedTextures;
	uint32_t AssetManager::s_CurrentIndex = 0;

	void AssetManager::ShutDown()
	{
		for (auto& mesh : s_Meshes)
		{
			delete mesh.second;
		}
		s_Meshes.clear();
		for (auto& texture : s_Textures)
			delete texture.second;
	}

	Mesh* AssetManager::LoadMesh(const UUID& meshId)
	{
		if (s_Meshes.contains(meshId))
		{
			return s_Meshes[meshId];
		}

		std::ifstream file = FS::Open("Assets\\info.oai", std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			std::cout << "Engine: Failed to open info.oai.\n";
			return nullptr;
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::string fileContent(fileSize, 0);
		file.seekg(0);
		file.read(fileContent.data(), fileSize);
		file.close();
		std::string_view meshTag = "<Mesh Id=\"";
		size_t tag = fileContent.find(meshTag, 0) + meshTag.length();
		if (tag == fileContent.npos)
			return nullptr;
		do
		{
			UUID id = UUID::FromString(std::string_view(fileContent.c_str() + tag, 36));
			if (id == meshId)
			{
				std::string_view pathString = "Path=\"";
				size_t pathPos = fileContent.find(pathString, tag) + pathString.length();
				size_t sdPos = fileContent.find('\"', pathPos + 1);
				std::string path = std::string(fileContent.c_str() + pathPos, sdPos - pathPos);
				s_Meshes[id] = new Mesh(path);
				return s_Meshes[id];
			}
			tag = fileContent.find(meshTag, tag) + meshTag.length();
		} while (tag != fileContent.npos);
		return nullptr;
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