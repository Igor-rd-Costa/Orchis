#include <filesystem>
#include "AssetManager.h"
#include "Renderer/Flags.h"
#include "VFS/FS.h"
namespace Orchis {

	Texture* AssetManager::s_DefaultTexture = nullptr;
	std::unordered_map<UUID, Texture*> AssetManager::s_Textures;
	std::unordered_map<UUID, Mesh*> AssetManager::s_Meshes;
	const uint32_t AssetManager::s_MaxIndexedTextures;
	uint32_t AssetManager::s_CurrentIndex = 0;

	void AssetManager::Init()
	{
		CreateDefaultTexture();
	}

	void AssetManager::ShutDown()
	{
		for (auto& mesh : s_Meshes)
			delete mesh.second;
		s_Meshes.clear();
		for (auto& texture : s_Textures)
			delete texture.second;
		s_Textures.clear();
		delete s_DefaultTexture;
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
		size_t tag = fileContent.find(meshTag, 0);
		if (tag == fileContent.npos)
			return nullptr;
		do
		{
			tag += meshTag.length();
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
			tag = fileContent.find(meshTag, tag);
		} while (tag != fileContent.npos);
		return nullptr;
	}

	Texture* AssetManager::LoadTexture(const UUID& textureId)
	{
		if (s_Textures.contains(textureId))
		{
			return s_Textures[textureId];
		}

		std::ifstream file = FS::Open("Assets\\info.oai", std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			std::cout << "Engine: Failed to open info.oai.\n";
			if (!s_DefaultTexture)
				CreateDefaultTexture();
			return s_DefaultTexture;
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::string fileContent(fileSize, 0);
		file.seekg(0);
		file.read(fileContent.data(), fileSize);
		file.close();
		std::string_view textureTag = "<Texture Id=\"";
		size_t tag = fileContent.find(textureTag, 0) + textureTag.length();
		if (tag == fileContent.npos)
		{
			if (s_DefaultTexture)
				CreateDefaultTexture();
			return s_DefaultTexture;
		}
		do
		{
			UUID id = UUID::FromString(std::string_view(fileContent.c_str() + tag, 36));
			if (id == textureId)
			{
				std::string_view pathString = "Path=\"";
				size_t pathPos = fileContent.find(pathString, tag) + pathString.length();
				size_t sdPos = fileContent.find('\"', pathPos + 1);
				std::string path = std::string(fileContent.c_str() + pathPos, sdPos - pathPos);
				s_Textures[id] = Texture::Create(path, s_CurrentIndex++);
				return s_Textures[id];
			}
			tag = fileContent.find(textureTag, tag) + textureTag.length();
		} while (tag != fileContent.npos);
		if (!s_DefaultTexture)
			CreateDefaultTexture();
		return s_DefaultTexture;
	}

	Texture* AssetManager::LoadDefaultTexture()
	{
		if (!s_DefaultTexture)
			CreateDefaultTexture();
		return s_DefaultTexture;
	}

	void AssetManager::CreateDefaultTexture()
	{
		std::vector<char> bytes = { 
			CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX
		};
		s_DefaultTexture = Texture::Create(bytes, 1, 1, 4, s_CurrentIndex++);
	}
}