#include "Macros.h"
#include "Utils/UUID.h"
#include "SceneManager.h"

using namespace Orchis;

OC_EXPORT UUID OrchisSceneManagerCreateScene(const UUID sceneId, bool makeActive)
{
	Scene* scene;
	std::cout << "Creating Scene " << (const char*)sceneId << "\n";
	if (!sceneId)
	{
		scene = SceneManager::CreateScene(makeActive);
		return scene->Id();
	}
	scene = SceneManager::CreateScene(sceneId, makeActive);
	return scene->Id();
}

OC_EXPORT void OrchisSceneManagerDeleteScene(const UUID sceneId)
{
	SceneManager::DeleteScene(sceneId);
}

OC_EXPORT void OrchisSceneManagerLoadScene(const UUID sceneId)
{
	SceneManager::LoadScene(sceneId);
}

OC_EXPORT void OrchisSceneManagerUnloadScene(const UUID sceneId)
{
	SceneManager::UnloadScene(sceneId);
}

OC_EXPORT void OrchisSceneManagerDebugScenes()
{
	SceneManager::DebugScenes();
}