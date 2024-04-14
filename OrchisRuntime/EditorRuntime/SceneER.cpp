#include "Macros.h"
#include "Utils/UUID.h"
#include "ECS/SceneManager.h"

using namespace Orchis;

OC_EXPORT UUID OrchisSceneAddEntity(const UUID sceneId, const UUID entityId)
{
	Scene* scene = SceneManager::GetScene(sceneId);
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
