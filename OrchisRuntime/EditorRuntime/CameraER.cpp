#include "Macros.h"
#include "PerspectiveCamera.h"

using namespace Orchis;

OC_EXPORT void* OrchisCameraCreate()
{
	return (void*)new PerspectiveCamera({ 0.0f, 0.0f, 0.0f }, 45.0f, 5.0f);
}

OC_EXPORT void OrchisCameraDelete(PerspectiveCamera* camera)
{
	delete camera;
}
