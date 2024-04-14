#include "Editor.h"
#include "Application.h"
#include "Renderer/Renderer.h"

using namespace Orchis;

Editor::Editor()
{
	m_EngineThread = std::thread([](Editor* editor) {
		editor->Run();
	}, this);
}

Editor::~Editor()
{
	m_Running = false;
	m_EngineThread.join();
}
	
void Editor::Run()
{
	while (m_Running)
	{
		Application::Update();
		Renderer::BeginFrame();
		Renderer::EndFrame();
	}
}


