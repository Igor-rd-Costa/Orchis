#include "Orchis.h"
#include "Macro.h"

ORCHIS_API void OrchisRendererSetActiveCamera(Orchis::PerspectiveCamera* camera)
{
	Orchis::Renderer::SetActiveCamera(camera);
}