#pragma once
#include <vector>
#include "Macros.h"
#include "Mesh.h"
#include "PerspectiveCamera.h"

namespace Orchis {

	class Scene
	{
	public:
		Scene();
		~Scene();

		void AddMesh(const char* filePath, const glm::vec3& position = { 0.0f, 0.0f, 0.0f });
		Ref<PerspectiveCamera> GetCamera() const { return m_ActiveCamera; }
		std::vector<Mesh*> m_Meshes;
	private:
		Ref<PerspectiveCamera> m_ActiveCamera;
	};
}