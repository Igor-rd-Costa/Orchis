#include "Macros.h"
#include "Utils/UUID.h"
#include "ECS/SceneManager.h"

using namespace Orchis;

OC_EXPORT UUID OrchisSceneAddEntity(const UUID entityId)
{
	Scene* scene = SceneManager::GetScene();
	if (!scene)
		return UUID::Null();

	if (!entityId)
	{
		Entity* entity = scene->AddEntity();
		return entity->Id();
	}
	Entity* entity = scene->AddEntity(entityId);
	return entity->Id();
}

OC_EXPORT void OrchisSceneRemoveEntity(const UUID entityId)
{
	Scene* scene = SceneManager::GetScene();
	if (!scene)
		return;

	scene->RemoveEntity(entityId);
}
