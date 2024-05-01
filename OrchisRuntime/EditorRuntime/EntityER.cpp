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
		return UUID::Null();
	Entity* entity = scene->GetEntity(entityId);
	if (!entity)
		return UUID::Null();
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

OC_EXPORT UUID OrchisEntityAddTextureComponent(const UUID entityId, const UUID componentId, const UUID textureId)
{
	Scene* scene = SceneManager::GetScene();
	if (!scene)
		return UUID::Null();
	Entity* entity = scene->GetEntity(entityId);
	if (!entity)
		return UUID::Null();
	return entity->AddTextureComponent(componentId, textureId);
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