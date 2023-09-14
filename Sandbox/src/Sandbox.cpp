#include "Sandbox.h"
#include <array>

namespace Orchis {

	Sandbox::Sandbox()
	{
		m_Camera = CreateScope<PerspectiveCamera>(glm::vec3(0.0f, -5.0f, 0.0f), 45.0f, 3.0f);
		
		Input::ToggleCursorVisibility();
		Input::ClipCursor();

		m_Scene.AddMesh("../Core/Assets/Models/Shiba.FBX", {  2.0f,  0.0f,  0.0f });
		m_Scene.AddMesh("../Core/Assets/Models/Shiba.FBX", {  4.0f,  1.0f,  0.0f });
		m_Scene.AddMesh("../Core/Assets/Models/Shiba.FBX", {  2.0f,  2.0f, -1.0f });
		m_Scene.AddMesh("../Core/Assets/Models/Shiba.FBX", { -2.0f,  1.0f,  0.0f });
		m_Scene.AddMesh("../Core/Assets/Models/Shiba.FBX", {  2.0f, -4.0f,  1.0f });
		m_Scene.AddMesh("../Core/Assets/Models/Shiba.FBX", {  2.0f, -1.0f,  1.0f });


		for (Mesh* mesh : m_Scene.m_Meshes)
		{
			for (SubMesh& subMesh : mesh->m_SubMeshes)
			{
				subMesh.SetTexture("../Core/Assets/Textures/default_Base_Color.png");
				subMesh.Rotate(-90.0f, { 1.0f, 0.0f, 0.0f });
				subMesh.Rotate(-90.0f, { 0.0f, 1.0f, 0.0f });
			}
		}
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
			Renderer::BeginScene(&m_Scene);
			
				
			Renderer::EndScene();
		}
	}

	Sandbox::~Sandbox()
	{

	}
	
	
}


