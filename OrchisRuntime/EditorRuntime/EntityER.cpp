#include "Macros.h"
#include "Utils/UUID.h"
#include "ECS/Entity.h"
#include "ECS/SceneManager.h"
#include "glm/glm.hpp"

using namespace Orchis;

OC_EXPORT UUID OrchisEntityAddComponent(const UUID entityId, ComponentType type)
{
	Scene* scene = SceneManager::GetScene();
	if (!scene)
	{
		std::cout << "AddComponent: Scene is null\n";
		return UUID::Null();
	}
	Entity* entity = scene->GetEntity(entityId);
	if (!entity)
	{
		std::cout << "AddComponent: Entity is null\n";
		return UUID::Null();
	}
	return entity->AddComponent(type);
}

OC_EXPORT UUID OrchisEntityAddTransformComponent(const UUID entityId, const UUID componentId, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)
{
	Scene* scene = SceneManager::GetScene();
	if (!scene)
		return UUID::Null();
	Entity* entity = scene->GetEntity(entityId);
	if (!entity)
		return UUID::Null();
	return entity->AddTransformComponent(componentId, pos, rot, scl);
}

OC_EXPORT UUID OrchisEntityAddMeshComponent(const UUID entityId, const UUID componentId, const UUID meshId, MeshType meshType)
{
	Scene* scene = SceneManager::GetScene();
	if (!scene)
		return UUID::Null();
	Entity* entity = scene->GetEntity(entityId);
	if (!entity)
		return UUID::Null();
	return entity->AddMeshComponent(componentId, meshId, meshType);
}

OC_EXPORT void OrchisEntityRemoveComponent(const UUID entityId, const UUID componentId)
{
	Scene* scene = SceneManager::GetScene();
	if (!scene)
		return;
	Entity* entity = scene->GetEntity(entityId);
	if (!entity)
		return;
	entity->RemoveComponent(componentId);
}

OC_EXPORT void OrchisEntityDebugComponents(const UUID entityId)
{
	Scene* scene = SceneManager::GetScene();
	if (!scene)
		return;
	Entity* entity = scene->GetEntity(entityId);
	if (entity)
	{
		std::cout << "Debug entity " << entity->Id().ToString() << ":\n";
		for (const Component& c : entity->GetComponents())
		{
			switch (c.type)
			{
			case ComponentType::TRANSFORM:
			{
				TransformComponent* tc = ComponentManager::GetTranformComponent(c.id);
				if (tc == nullptr)
				{
					std::cout << " Transform component " << c.id.ToString() << " not found in engine.\n";
				}
				else
				{
					std::cout << " Transform component " << tc->id.ToString() << ":\n";
					std::cout << "  Pos: { X: " << tc->position.x << ", Y: " << tc->position.y << ", Z: " << tc->position.z << " }\n";
					std::cout << "  Rot: { X: " << tc->rotation.x << ", Y: " << tc->rotation.y << ", Z: " << tc->rotation.z << " }\n";
					std::cout << "  Scl: { X: " << tc->scale.x << ", Y: " << tc->scale.y << ", Z: " << tc->scale.z << " }\n";
				}
			} break;
			case ComponentType::MESH:
			{
				MeshComponent* mc = ComponentManager::GetMeshComponent(c.id);
				if (mc == nullptr)
				{
					std::cout << " Mesh component " << c.id.ToString() << " not found in engine.\n";
				}
				else
				{
					std::cout << " Mesh component " << mc->id.ToString() << ":\n";
					std::cout << "  MeshId: " << mc->meshId.ToString() << ".\n";
					std::cout << "  MeshType: Static.\n";
				}
			} break;
			}
		}
	}
}	
