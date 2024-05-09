#include "Macros.h"
#include "Editor/Editor.h"
#include "Application.h"

using namespace Orchis;

static Editor* editor = nullptr;

OC_EXPORT void OrchisInit(void* parentWindow, const EditorCameraData data)
{
	Application::Init(parentWindow);
	editor = new Editor(data);
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

OC_EXPORT void OrchisEditorRegisterMouseMoveEventCallback(void(*callback)(EditorMouseMoveEventArgs e))
{
	if (editor)
		editor->RegisterMouseMoveCallback(callback);
}


OC_EXPORT void OrchisEditorRegisterCameraMoveEventCallback(void(*callback)(EditorCameraMoveEventArgs e))
{
	if (editor)
		editor->RegisterCameraMoveCallback(callback);
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