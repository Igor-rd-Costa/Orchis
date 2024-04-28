#include "Macros.h"
#include "Utils/UUID.h"
#include "SceneManager.h"

using namespace Orchis;

OC_EXPORT UUID OrchisSceneManagerCreateScene(const UUID sceneId)
{
	if (sceneId.IsNull())
		return SceneManager::CreateScene()->Id();
	return SceneManager::CreateScene(sceneId)->Id();
}