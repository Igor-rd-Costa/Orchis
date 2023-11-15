#include "Sandbox.h"
#include <array>

namespace Orchis {

	Sandbox::Sandbox()
	{		
		Input::ToggleCursorVisibility();
		Input::ClipCursor();
		Renderer::SetActiveCamera(new Orchis::PerspectiveCamera({ 0.0f, 0.0f, 0.0f }, 45.0f, 5.0f));

		SceneManager::CreateScene("NewScene");
		Scene* scene = SceneManager::GetScene();
		scene->AddMesh("Assets/Models/Shiba.FBX", { 0.0f,  6.0f,  0.0f });

		for (Mesh& mesh : scene->m_Meshes)
		{
			for (SubMesh& subMesh : mesh.m_SubMeshes)
			{
				subMesh.SetTexture("Assets/Textures/default_Base_Color.png");
				subMesh.Rotate(-90.0f, { 1.0f, 0.0f, 0.0f });
				subMesh.Rotate(-40.0f, { 0.0f, 1.0f, 0.0f });
			}
		}
	}

	void Sandbox::Run()
	{
		while (Application::s_IsRunning)
		{
			Application::Update();
			if (Input::KeyPressed(Key::KEY_ESCAPE))
			{	
				Input::ClipCursor();
				Input::CenterCursor();
				Input::ToggleCursorVisibility();
				m_IsPaused = !m_IsPaused;
			}
			if (!m_IsPaused)	
			{
				Renderer::BeginScene();
				
					
				Renderer::EndScene();
			}
		}
	}

	Sandbox::~Sandbox()
	{
		Orchis::Application::Shutdown();
	}
	
	
}


