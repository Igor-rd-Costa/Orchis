#include "OrchisPCH.h"
#include "Scene.h"

namespace Orchis {
	
	Scene::Scene()
	{
		m_ActiveCamera = CreateRef<PerspectiveCamera>(glm::vec3(0.0f, -5.0f, 0.0f), 45.0f, 5.0f);
	}

	Scene::~Scene()
	{
		for (Mesh* mesh : m_Meshes)
			delete mesh;
	}

	void Scene::AddMesh(const char* filePath, const glm::vec3& position)
	{
		m_Meshes.emplace_back(new Mesh(filePath, position ));
	}

}