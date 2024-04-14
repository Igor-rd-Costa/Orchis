#include "ComponentManager.h"

namespace Orchis
{
	std::vector<TransformComponent> ComponentManager::s_TransformComponents = {};


	Component ComponentManager::CreateTransformComponent()
	{
		return CreateTransformComponent(UUID::Generate(), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
	}

	Component ComponentManager::CreateTransformComponent(const UUID& componentId, const glm::vec3& position, 
		const glm::vec3& rotation, const glm::vec3& scale)
	{
		Component component = { .type = ComponentType::TRANSFORM, .id = componentId };
		s_TransformComponents.emplace_back(componentId, position, rotation, scale);
		return component;
	}

}