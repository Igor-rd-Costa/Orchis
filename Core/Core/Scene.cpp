#include "Scene.h"

namespace Orchis {
	
	Scene::Scene(std::string_view name)
		: m_Name(name)
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::AddMesh(const char* filePath, const glm::vec3& position)
	{
		m_Meshes.emplace_back(filePath, position);
	}

}