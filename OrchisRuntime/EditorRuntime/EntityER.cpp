#include "Macros.h"
#include "Utils/UUID.h"
#include "ECS/Entity.h"
#include "ECS/SceneManager.h"
#include "glm/glm.hpp"

using namespace Orchis;

OC_EXPORT UUID OrchisEntityAddComponent(const UUID sceneId, const UUID entityId, ComponentType type)
{
	Scene* scene = SceneManager::GetScene(sceneId);
	if (!scene)
		return UUID::Null();
	Entity* entity = scene->GetEntity(entityId);
	if (!entity)
		return UUID::Null();
	return entity->AddComponent(type);
}

OC_EXPORT UUID OrchisEntityAddTransformComponent(const UUID sceneId, const UUID entityId, const UUID componentId, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)
{
	Scene* scene = SceneManager::GetScene(sceneId);
	if (!scene)
		return UUID::Null();
	Entity* entity = scene->GetEntity(entityId);
	if (!entity)
		return UUID::Null();
	return entity->AddTransformComponent(componentId, pos, rot, scl);
}

OC_EXPORT void OrchisEntityRemoveComponent(const UUID sceneId, const UUID entityId, const UUID componentId)
{
	Scene* scene = SceneManager::GetScene(sceneId);
	if (!scene)
		return;
	Entity* entity = scene->GetEntity(entityId);
	if (!entity)
		return;
	entity->RemoveComponent(componentId);
}

OC_EXPORT void OrchisEntityDebugComponents(const UUID sceneId, const UUID entityId)
{
	Scene* scene = SceneManager::GetScene(sceneId);
	if (!scene)
		return;
	Entity* entity = scene->GetEntity(entityId);
	if (entity)
	{
		std::cout << "Engine:\nEntity " << entity->Id().ToString() << " has " << entity->GetComponents().size() << " components.\n";
	}
}	
