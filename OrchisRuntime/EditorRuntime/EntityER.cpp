#include "Macros.h"
#include "Utils/UUID.h"
#include "ECS/Entity.h"
#include "ECS/SceneManager.h"

using namespace Orchis;

OC_EXPORT UUID OrchisEntityAddComponent(UUID sceneId, UUID entityId, ComponentType type)
{
	Scene* scene = SceneManager::GetScene(sceneId);
	if (!scene)
		return UUID::Null();
	Entity* entity = scene->GetEntity(entityId);
	if (!entity)
		return UUID::Null();
	return entity->AddComponent(type);
}
