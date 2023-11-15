#include "Orchis.h"
#include "Macro.h"

ORCHIS_API void OrchisSceneCreate(const char* name)
{
	Orchis::SceneManager::CreateScene(name);
}