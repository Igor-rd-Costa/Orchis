#include "ComponentManager.h"

namespace Orchis
{
	std::vector<TransformComponent> ComponentManager::s_TransformComponents = {};
	std::vector<MeshComponent> ComponentManager::s_MeshComponents = {};

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
		case Orchis::ComponentType::MESH:
			RemoveMeshComponent(componentId);
			break;
		case Orchis::ComponentType::TEXTURE:
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

	Component ComponentManager::CreateMeshComponent()
	{
		return CreateMeshComponent(UUID::Generate(), UUID::Null(), MeshType::STATIC);
	}

	Component ComponentManager::CreateMeshComponent(const UUID& componentId, const UUID& meshId, MeshType meshType)
	{
		Component component = { .id = componentId, .type = ComponentType::MESH };
		s_MeshComponents.emplace_back(componentId, meshId, meshType);
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

	void ComponentManager::RemoveMeshComponent(const UUID& componentId)
	{
		for (auto it = s_MeshComponents.begin(); it != s_MeshComponents.end(); it++)
		{
			if (it->id == componentId)
			{
				s_MeshComponents.erase(it);
				return;
			}
		}
	}

	TransformComponent* ComponentManager::GetTranformComponent(const UUID& componentId)
	{
		for (TransformComponent& component : s_TransformComponents)
		{
			if (component.id == componentId)
				return &component;
		}
		return nullptr;
	}

	MeshComponent* ComponentManager::GetMeshComponent(const UUID& componentId)
	{
		for (MeshComponent& component : s_MeshComponents)
		{
			if (component.id == componentId)
				return &component;
		}
		return nullptr;
	}

	void ComponentManager::UpdateTransformComponentPosition(const UUID& componentId, glm::vec3 position)
	{
		for (TransformComponent& tc : s_TransformComponents)
		{
			if (tc.id == componentId)
			{
				tc.position = position;
				break;
			}
		}
	}

	void ComponentManager::UpdateTransformComponentRotation(const UUID& componentId, glm::vec3 rotation)
	{
		for (TransformComponent& tc : s_TransformComponents)
		{
			if (tc.id == componentId)
			{
				tc.rotation = rotation;
				break;
			}
		}
	}

	void ComponentManager::UpdateTransformComponentScale(const UUID& componentId, glm::vec3 scale)
	{
		for (TransformComponent& tc : s_TransformComponents)
		{
			if (tc.id == componentId)
			{
				tc.scale = scale;
				break;
			}
		}
	}

	void ComponentManager::UpdateMeshComponentMeshId(const UUID& componentId, const UUID& meshId)
	{
		for (MeshComponent& mc : s_MeshComponents)
		{
			if (mc.id == componentId)
			{
				mc.meshId = meshId;
				break;
			}
		}
	}

	void ComponentManager::UpdateMeshComponentMeshType(const UUID& componentId, MeshType meshType)
	{
		for (MeshComponent& mc : s_MeshComponents)
		{
			if (mc.id == componentId)
			{
				mc.meshType = meshType;
				break;
			}
		}
	}
}