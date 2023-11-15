#include "Orchis.h"
#include "Macro.h"

ORCHIS_API void* OrchisCameraCreate()
{
	return (void*)new Orchis::PerspectiveCamera({ 0.0f, 0.0f, 0.0f }, 45.0f, 5.0f);
}

ORCHIS_API void OrchisCameraDelete(Orchis::PerspectiveCamera* camera)
{
	delete camera;
}

