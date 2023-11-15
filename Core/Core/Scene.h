#pragma once
#include <vector>
#include "Macros.h"
#include "Mesh.h"
#include "PerspectiveCamera.h"

namespace Orchis {

	class Scene
	{
	public:
		Scene(std::string_view name);
		~Scene();

		void AddMesh(const char* filePath, const glm::vec3& position = { 0.0f, 0.0f, 0.0f });
		std::vector<Mesh> m_Meshes;
	private:
		std::string m_Name;
	};
}