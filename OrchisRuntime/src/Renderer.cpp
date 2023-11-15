#include "Orchis.h"
#include "Macro.h"

ORCHIS_API void OrchisRendererSetActiveCamera(Orchis::PerspectiveCamera* camera)
{
	Orchis::Renderer::SetActiveCamera(camera);
}

ORCHIS_API void OrchisRendererSetDefaultShaders(const char* vertexPath, const char* fragPath) 
{
	Orchis::Renderer::SetDefaultShaderPaths(vertexPath, fragPath);
}