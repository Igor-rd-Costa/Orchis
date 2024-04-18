#include "ComponentManager.h"

namespace Orchis
{
	std::vector<TransformComponent> ComponentManager::s_TransformComponents = {};

	Component ComponentManager::CreateComponent(ComponentType type)
	{
		switch (type)
		{
		case Orchis::ComponentType::TRANSFORM:
			return CreateTransformComponent();
			break;
		case Orchis::ComponentType::TEXTURE:
			break;
		case Orchis::ComponentType::MESH:
			break;
		case Orchis::ComponentType::INVALID:
			break;
		default:
			break;
		}
		return Component{ .id = UUID::Null(), .type = ComponentType::INVALID };
	}

	void ComponentManager::RemoveComponent(const UUID& componentId, ComponentType type)
	{
		switch (type)
		{
		case Orchis::ComponentType::TRANSFORM:
			RemoveTransformComponent(componentId);
			break;
		case Orchis::ComponentType::TEXTURE:
			break;
		case Orchis::ComponentType::MESH:
			break;
		case Orchis::ComponentType::INVALID:
			break;
		default:
			break;
		}
	}

	Component ComponentManager::CreateTransformComponent()
	{
		return CreateTransformComponent(UUID::Generate(), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
	}

	Component ComponentManager::CreateTransformComponent(const UUID& componentId, const glm::vec3& position, 
		const glm::vec3& rotation, const glm::vec3& scale)
	{
		Component component = { .id = componentId, .type = ComponentType::TRANSFORM };
		s_TransformComponents.emplace_back(componentId, position, rotation, scale);
		return component;
	}

	void ComponentManager::RemoveTransformComponent(const UUID& componentId)
	{
		for (auto it = s_TransformComponents.begin(); it != s_TransformComponents.end(); it++)
		{
			if (it->id == componentId)
			{
				s_TransformComponents.erase(it);
				return;
			}
		}
	}

	TransformComponent* ComponentManager::GetTranformComponent(const UUID componentId)
	{
		for (TransformComponent& component : s_TransformComponents)
		{
			if (component.id == componentId)
				return &component;
		}
		return nullptr;
	}

}