#include "Orchis.h"
#include "Macro.h"

ORCHIS_API void* OrchisSceneCreate(const char* name)
{
	return Orchis::SceneManager::CreateScene(name);
}