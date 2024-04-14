#include "Macros.h"
#include "PerspectiveCamera.h"
#include "Renderer/Renderer.h"

using namespace Orchis;

OC_EXPORT void OrchisRendererSetActiveCamera(PerspectiveCamera* camera)
{
	Renderer::SetActiveCamera(camera);
}