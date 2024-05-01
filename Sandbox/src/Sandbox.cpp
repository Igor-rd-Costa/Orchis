#include "Sandbox.h"
#include <array>
#include "VFS/FS.h"
namespace Orchis 
{
	static PerspectiveCamera* camera = nullptr;
	Sandbox::Sandbox()
	{
		Input::ClipCursor();
		Input::CenterCursor();
		Input::ToggleCursorVisibility();
		FS::Mount("D:\\OrchisTestProjects\\NewProject\\Assets", "Assets");
		camera = new Orchis::PerspectiveCamera({ 0.0f, -5.0f, 0.0f }, 45.0f, 5.0f);
		Renderer::SetActiveCamera(camera);

		Scene* scene = SceneManager::CreateScene();
		Entity* sword = scene->AddEntity();
		sword->AddComponent(ComponentType::TRANSFORM);
		UUID meshId = sword->AddComponent(ComponentType::MESH);
		UUID textId = sword->AddComponent(ComponentType::TEXTURE);
		ComponentManager::UpdateMeshComponentMeshId(meshId, UUID::FromString(std::string_view("66B97E2E-5333-4E13-A9BD-07AAA5E2DB52")));
		ComponentManager::UpdateTextureComponentTextureId(textId, UUID::FromString(std::string_view("3A33A956-5180-49A6-BAA2-2D3485B37B7F")));
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


			if (!m_IsPaused && Application::s_IsRunning)	
			{
				Renderer::BeginFrame();
				
				camera->Move();
			
				Renderer::EndFrame();
			}
		}
	}

	Sandbox::~Sandbox()
	{
		Orchis::Application::Shutdown();
	}
	
	
}


