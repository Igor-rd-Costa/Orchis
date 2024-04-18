#include "Macros.h"
#include "Utils/UUID.h"
#include "ECS/SceneManager.h"

using namespace Orchis;

OC_EXPORT UUID OrchisSceneAddEntity(const UUID sceneId, const UUID entityId)
{
	std::cout << "Creating entity in scene " << sceneId.ToString() << "\n";
	Scene* scene = SceneManager::GetScene(sceneId);

	if (!scene)
	{
		std::cout << "Scene is null!\n";
		return UUID::Null();
	}

	if (!entityId)
	{
		Entity* entity = scene->AddEntity();
		return entity->Id();
	}
	Entity* entity = scene->AddEntity(entityId);
	return entity->Id();
}

OC_EXPORT void OrchisSceneRemoveEntity(const UUID sceneId, const UUID entityId)
{
	Scene* scene = SceneManager::GetScene(sceneId);
	if (!scene)
		return;

	scene->RemoveEntity(entityId);
}

OC_EXPORT void OrchisSceneDebugEntities(const UUID sceneId)
{
	Scene* scene = SceneManager::GetScene(sceneId);
	if (!scene)
		return;

	auto entities = scene->GetEntities();
	std::cout << "Engine:\nScene " << scene->Id().ToString() << ": " << entities.size() << " entities.";
}
