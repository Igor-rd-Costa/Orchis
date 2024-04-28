#include "Macros.h"
#include "Editor/Editor.h"
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

OC_EXPORT void OrchisEditorRegisterEventCallback(void(*callback)(EditorEventArgs e))
{
	editor->RegisterEventCallback(callback);
}

OC_EXPORT bool OrchisIsMouseHovering()
{
	Window* window = Application::GetWindow();
	if (!window)
		return false;
	return window->IsMouseHovering();
}

OC_EXPORT void OrchisMainWindowUpdateRect()
{
	Window* window = Application::GetWindow();
	if (window != nullptr)
		window->UpdateRect();
}