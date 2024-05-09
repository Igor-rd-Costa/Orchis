#include "Editor.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "EventDispatcher.h"

using namespace Orchis;

Orchis::PerspectiveCamera* Editor::s_EditorCamera = nullptr;
bool Editor::s_Running = true;
std::thread Editor::s_EngineThread;
void(*Editor::s_MouseMoveCallback)(EditorMouseMoveEventArgs e) = [](EditorMouseMoveEventArgs e){};
void(*Editor::s_CameraMoveCallback)(EditorCameraMoveEventArgs e) = [](EditorCameraMoveEventArgs e) {};

Editor::Editor(const EditorCameraData& data)
{
	this->s_EditorCamera = new PerspectiveCamera(data.position, data.fieldOfView, data.speed, data.lookAt, data.yaw, data.pitch);
	Renderer::SetActiveCamera(this->s_EditorCamera);

	EventDispatcher::OnMouseMove(OnMouseMove);
	EventDispatcher::OnMouseButtonDown(OnMouseButtonDown);
	EventDispatcher::OnMouseButtonUp(OnMouseButtonUp);
	s_EngineThread = std::thread([](Editor* editor) {
		editor->Run();
	}, this);
}

Editor::~Editor()
{
	s_Running = false;
	s_EngineThread.join();
}
	
void Editor::Run()
{
	while (s_Running)
	{
		Application::Update();
		Renderer::BeginFrame();
		Update();
		Renderer::EndFrame();
	}
}

void Editor::Update()
{
	if (Input::IsKeyDown(Key::MOUSE_RBUTTON))
	{
		s_EditorCamera->Move();
		s_CameraMoveCallback({ .data = GetCameraData() });
	}
}

void Editor::RegisterMouseMoveCallback(void(*callback)(EditorMouseMoveEventArgs e))
{
	s_MouseMoveCallback = callback;
}

void Editor::RegisterCameraMoveCallback(void(*callback)(EditorCameraMoveEventArgs e))
{
	s_CameraMoveCallback = callback;
}

void Editor::OnMouseButtonDown(Orchis::MouseButtonDownEvent* event)
{
	if (event->GetButton() == Key::MOUSE_RBUTTON)
	{
		Input::CenterCursor();
		Input::ClipCursor();
	}
}

void Editor::OnMouseButtonUp(Orchis::MouseButtonUpEvent* event)
{
	if (event->GetButton() == Key::MOUSE_RBUTTON)
	{
		Input::CenterCursor();
		Input::ClipCursor();
	}
}

void Editor::OnMouseMove(MouseMoveEvent* event)
{
	s_MouseMoveCallback({});
}

EditorCameraData Editor::GetCameraData()
{
	if (s_EditorCamera)
	{
		return { 
			.position = s_EditorCamera->GetPosition(), 
			.lookAt = s_EditorCamera->GetFront(),
			.speed = s_EditorCamera->GetSpeed(),
			.fieldOfView = s_EditorCamera->GetFieldOfView(),
			.yaw = s_EditorCamera->GetYaw(),
			.pitch = s_EditorCamera->GetPitch()
		};
	}
	return EditorCameraData();
}

void Editor::SetEditorCameraData(EditorCameraData data)
{
	if (!s_EditorCamera)
		return;
	s_EditorCamera->SetPosition(data.position);
	s_EditorCamera->SetFront(data.lookAt);
	s_EditorCamera->SetSpeed(data.speed);
	s_EditorCamera->SetFieldOfView(data.fieldOfView);
	s_EditorCamera->SetYaw(data.yaw);
	s_EditorCamera->SetPitch(data.pitch);
}


OC_EXPORT EditorCameraData OrchisEditorGetEditorCameraData()
{
	return Editor::GetCameraData();
}

OC_EXPORT void OrchisEditorSetEditorCameraData(EditorCameraData data)
{
	Editor::SetEditorCameraData(data);
}