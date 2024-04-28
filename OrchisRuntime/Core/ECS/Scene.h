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

		const UUID& Id() const { return m_Id; }

		Entity* AddEntity();
		Entity* AddEntity(const UUID& entityId);
		void RemoveEntity(const UUID& entityId);

		Entity* GetEntity(const UUID& entityId);
		const std::vector<Entity>& GetEntities() const { return m_Entities; }
	private:
		UUID m_Id;
		std::vector<Entity> m_Entities;
	};
}