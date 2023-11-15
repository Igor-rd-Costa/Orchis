#include "Orchis.h"
#include "Macro.h"

ORCHIS_API void OrchisSceneCreate(const char* name)
{
	std::cout << "Creating scene!\n";
	Orchis::SceneManager::CreateScene(name);
}