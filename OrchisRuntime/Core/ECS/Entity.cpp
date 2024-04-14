#include "Entity.h"

namespace Orchis
{
	Entity::Entity()
		: m_Id(UUID::Generate()) {}

	Entity::Entity(const UUID& entityId)
		: m_Id(entityId) {}

	const UUID Entity::AddComponent(ComponentType type)
	{
		Component c = { .type = type, .id = UUID::Null()};
		switch (type)
		{
		case ComponentType::TRANSFORM:
			c = ComponentManager::CreateTransformComponent();
			break;
		case ComponentType::TEXTURE:
			break;
		case ComponentType::MESH:
			break;
		default:
			break;
		}
		m_Components.push_back(c);
		return c.id;
	}

	const UUID Entity::AddTransformComponent(const UUID& componentId, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		Component c = ComponentManager::CreateTransformComponent(componentId, position, rotation, scale);
		m_Components.push_back(c);
		return c.id;
	}
}
