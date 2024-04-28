#include "Editor.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "EventDispatcher.h"

using namespace Orchis;

Orchis::PerspectiveCamera* Editor::s_EditorCamera = nullptr;
bool Editor::s_Running = true;
std::thread Editor::s_EngineThread;
std::vector<void(*)(EditorEventArgs e)> Editor::s_Callbacks = {};

Editor::Editor()
{
	this->s_EditorCamera = new PerspectiveCamera({ 0.0f, -5.0f, 0.0f }, 45.0f, 5.0f);
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

static bool clipped = false;
void Editor::Update()
{
	if (Input::IsKeyDown(Key::MOUSE_RBUTTON))
	{
		s_EditorCamera->Move();
	}
}

void Editor::RegisterEventCallback(void(*callback)(EditorEventArgs e))
{
	s_Callbacks.push_back(callback);
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

static EditorEventArgs args = { .type = EditorEventType::MOUSE_MOVE };
void Editor::OnMouseMove(MouseMoveEvent* event)
{
	for (void(*callback)(EditorEventArgs) : s_Callbacks)
		callback(args);
}


