#include "Entity.h"

namespace Orchis
{
	Entity::Entity()
		: m_Id(UUID::Generate()) {}

	Entity::Entity(const UUID& entityId)
		: m_Id(entityId) {}

	const UUID Entity::AddComponent(ComponentType type)
	{
		Component c = { .id = UUID::Null(), .type = type};
		switch (type)
		{
		case ComponentType::TRANSFORM:
			c = ComponentManager::CreateTransformComponent();
			break;
		case ComponentType::MESH:
			c = ComponentManager::CreateMeshComponent();
			break;
		case ComponentType::TEXTURE:
			c = ComponentManager::CreateTextureComponent();
			break;
		default:
			break;
		}
		m_Components.push_back(c);
		return c.id;
	}

	void Entity::RemoveComponent(const UUID& componentId)
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); it++)
		{
			if (it->id == componentId)
			{
				ComponentManager::RemoveComponent(it->id, it->type);
				m_Components.erase(it);
				return;
			}
		}
	}

	const UUID Entity::AddTransformComponent(const UUID& componentId, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		Component c = ComponentManager::CreateTransformComponent(componentId, position, rotation, scale);
		m_Components.push_back(c);
		return c.id;
	}
	const UUID Entity::AddMeshComponent(const UUID& componentId, const UUID& meshId, MeshType meshType)
	{
		Component c = ComponentManager::CreateMeshComponent(componentId, meshId, meshType);
		m_Components.push_back(c);
		return c.id;
	}
	const UUID Entity::AddTextureComponent(const UUID& componentId, const UUID& textureId)
	{
		Component c = ComponentManager::CreateTextureComponent(componentId, textureId);
		m_Components.push_back(c);
		return c.id;
	}
}
