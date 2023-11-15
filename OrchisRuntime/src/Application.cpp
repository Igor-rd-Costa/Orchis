#include "Orchis.h"
#include "Macro.h"

ORCHIS_API void OrchisInit(void* parentWindow)
{
	Orchis::Application::Init(parentWindow);
}

ORCHIS_API void OrchisShutdown()
{
	Orchis::Application::Shutdown();
}

ORCHIS_API void* OrchisGetMainWindowHandle()
{
	return Orchis::Application::GetWindow()->GetHandle();
}

