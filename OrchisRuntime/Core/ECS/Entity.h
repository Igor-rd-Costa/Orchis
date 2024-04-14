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

		const UUID AddComponent(ComponentType type);
		const UUID AddTransformComponent(const UUID& componentId, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	private:
		UUID m_Id;
		std::vector<Component> m_Components = {};
	};
}