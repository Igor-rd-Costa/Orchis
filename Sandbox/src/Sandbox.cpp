#include "Sandbox.h"

namespace Orchis {

	Sandbox::Sandbox()
	{
		m_Camera = CreateScope<PerspectiveCamera>(glm::vec3(0.0f, -5.0f, 0.0f), 45.0f, 5.0f);
		Input::ToggleCursorVisibility();
		Input::ClipCursor();
	}

	void Sandbox::OnUpdate()
	{
		if (Input::KeyPressed(Key::KEY_ESCAPE))
		{	
			Input::ClipCursor();
			Input::CenterCursor();
			Input::ToggleCursorVisibility();
			m_IsPaused = !m_IsPaused;
		}
		if (!m_IsPaused)
		{
			Renderer::BeginScene(m_Camera.get());
			
			
			Renderer::DrawCube({ 0.0f, 0.0f, 0.0f });
			
			
			Renderer::EndScene();
		}
	}

	Sandbox::~Sandbox()
	{

	}
	
	
}


