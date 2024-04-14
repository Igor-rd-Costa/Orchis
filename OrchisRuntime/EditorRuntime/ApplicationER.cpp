#include "Macros.h"
#include "Editor.h"
#include "Application.h"

using namespace Orchis;

static Editor* editor = nullptr;

OC_EXPORT void OrchisInit(void* parentWindow)
{
	Application::Init(parentWindow);
	editor = new Editor();
}

OC_EXPORT void OrchisShutdown()
{
	delete editor;
	Application::Shutdown();
}

OC_EXPORT void* OrchisGetMainWindowHandle()
{
	return Application::GetWindow()->GetHandle();
}