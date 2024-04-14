#pragma once
#include <vector>
#include "Macros.h"
#include "Mesh.h"
#include "PerspectiveCamera.h"
#include "Utils/UUID.h"
#include "Entity.h"

namespace Orchis {

	class OC_API Scene
	{
	public:
		Scene();
		Scene(const UUID& id);
		~Scene() = default;

		bool IsActive() const { return m_IsActive; }
		void SetActive(bool active) { m_IsActive = active; }
		const UUID& Id() const { return m_Id; }

		Entity* AddEntity();
		Entity* AddEntity(const UUID& entityId);

		Entity* GetEntity(const UUID& entityId);

		void AddMesh(const char* filePath, const glm::vec3& position = { 0.0f, 0.0f, 0.0f });
		std::vector<Mesh> m_Meshes;
	private:
		UUID m_Id;
		std::vector<Entity> m_Entities;
		bool m_IsActive = false;
	};
}