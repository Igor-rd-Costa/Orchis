#pragma once
#include "Macros.h"
#include "Utils/UUID.h"
#include "ComponentManager.h"

namespace Orchis
{
	class OC_API Entity
	{
	public:
		Entity();
		Entity(const UUID& entityId);
		~Entity() = default;

		const UUID& Id() const { return m_Id; }

		const std::vector<Component>& GetComponents() const { return m_Components; }

		const UUID AddComponent(ComponentType type);
		void RemoveComponent(const UUID& componentId);

		const UUID AddTransformComponent(const UUID& componentId, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		const UUID AddMeshComponent(const UUID& componentId, const UUID& meshId, MeshType meshType);
		const UUID AddTextureComponent(const UUID& componentId, const UUID& textureId);
	private:
		UUID m_Id;
		std::vector<Component> m_Components = {};
	};
}