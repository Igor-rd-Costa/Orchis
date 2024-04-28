#include "Sandbox.h"
#include <array>

namespace Orchis 
{
	static PerspectiveCamera* camera = nullptr;
	Sandbox::Sandbox()
	{
		//Input::ToggleCursorVisibility();
		//Input::ClipCursor();
	
		camera = new Orchis::PerspectiveCamera({ 0.0f, -5.0f, 0.0f }, 45.0f, 5.0f);
		Renderer::SetActiveCamera(camera);

		Scene* scene = SceneManager::CreateScene();
		Entity* dog = scene->AddEntity();
		dog->AddComponent(ComponentType::TRANSFORM);
		dog->AddComponent(ComponentType::MESH);
		
		/*scene->AddMesh("Assets/Models/Shiba.FBX", { 0.0f,  6.0f,  0.0f });

		for (Mesh& mesh : scene->m_Meshes)
		{
			for (SubMesh& subMesh : mesh.m_SubMeshes)
			{
				subMesh.SetTexture("Assets/Textures/default_Base_Color.png");
				subMesh.Rotate(-90.0f, { 1.0f, 0.0f, 0.0f });
				subMesh.Rotate(-40.0f, { 0.0f, 1.0f, 0.0f });
			}
		}*/
	}

	void Sandbox::Run()
	{
		while (Application::s_IsRunning)
		{
			Application::Update();
			if (Input::KeyPressed(Key::KEY_ESCAPE))
			{	
				m_IsPaused = !m_IsPaused;
			}


			if (!m_IsPaused && Application::s_IsRunning)	
			{
				Renderer::BeginFrame();
				
				if (Input::KeyPressed(Key::MOUSE_RBUTTON))
				{

					Input::ClipCursor();
					Input::CenterCursor();
					Input::ToggleCursorVisibility();
				}
				if (Input::KeyReleased(Key::MOUSE_RBUTTON))
				{
					Input::ClipCursor();
					Input::CenterCursor();
					Input::ToggleCursorVisibility();
				}
				if (Input::IsKeyDown(Key::MOUSE_RBUTTON))
				{
					camera->Move();
				}
					
				Renderer::EndFrame();
			}
		}
	}

	Sandbox::~Sandbox()
	{
		Orchis::Application::Shutdown();
	}
	
	
}


