#include "ComponentManager.h"

namespace Orchis
{
	std::vector<TransformComponent> ComponentManager::s_TransformComponents = {};
	std::vector<MeshComponent> ComponentManager::s_MeshComponents = {};
	std::vector<TextureComponent> ComponentManager::s_TextureComponents = {};

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
		return CreateTransformComponent(UUID::Generate(), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f));
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

	Component ComponentManager::CreateTextureComponent()
	{
		return CreateTextureComponent(UUID::Generate(), UUID::Null());
	}

	Component ComponentManager::CreateTextureComponent(const UUID& componentId, const UUID& textureId)
	{
		Component c = { .id = componentId, .type = ComponentType::TEXTURE };
		s_TextureComponents.emplace_back(componentId, textureId);
		return c;
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

	void ComponentManager::RemoveTextureComponent(const UUID& componentId)
	{
		for (auto it = s_TextureComponents.begin(); it != s_TextureComponents.end(); it++)
		{
			if (it->id == componentId)
			{
				s_TextureComponents.erase(it);
				return;
			}
		}
	}

	TransformComponent* ComponentManager::GetTranformComponent(const UUID& componentId)
	{
		for (TransformComponent& tc : s_TransformComponents)
		{
			if (tc.id == componentId)
				return &tc;
		}
		return nullptr;
	}

	MeshComponent* ComponentManager::GetMeshComponent(const UUID& componentId)
	{
		for (MeshComponent& mc : s_MeshComponents)
		{
			if (mc.id == componentId)
				return &mc;
		}
		return nullptr;
	}

	TextureComponent* ComponentManager::GetTextureComponent(const UUID& componentId)
	{
		for (TextureComponent& tc : s_TextureComponents)
		{
			if (tc.id == componentId)
				return &tc;
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
				return;
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
				return;
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
				return;
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
				return;
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
				return;
			}
		}
	}


	void ComponentManager::UpdateTextureComponentTextureId(const UUID& componentId, const UUID& newTextureId)
	{
		for (TextureComponent& tc : s_TextureComponents)
		{
			if (tc.id == componentId)
			{
				tc.textureId = newTextureId;
				return;
			}
		}
	}
}