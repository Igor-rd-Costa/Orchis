#include "Renderer.h"
#include "Platform/Vulkan/VulkanAPI.h"
#include "VertexBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Application.h"
#include "RenderCommand.h"
#include "AssetManager.h"

namespace Orchis {

	RenderData* Renderer::s_Data = new RenderData;
	PerspectiveCamera* Renderer::s_ActiveCamera = nullptr;
	std::thread Renderer::s_RenderThread;
	std::string Renderer::DefaultVertexPath = "Shaders\\vert.spv";
	std::string Renderer::DefaultFragmentPath = "Shaders\\frag.spv";

	void Renderer::Init()
	{
		RenderCommand::Init();
		RenderCommand::SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		s_Data->TransformsUB = UniformBuffer::Create(sizeof(glm::mat4)*2, 0, SHADER_STAGE_VERTEX, {
			{ "ViewProj", UNIFORM_TYPE_MAT4 }
		});

		s_Data->MaterialUniformBuffer = UniformBuffer::Create(sizeof(Material), 1, SHADER_STAGE_FRAGMENT, {
			{ "Ambient", UNIFORM_TYPE_FLOAT },
			{ "Diffuse", UNIFORM_TYPE_FLOAT },
			{ "Specular", UNIFORM_TYPE_FLOAT },
			{ "Shininess", UNIFORM_TYPE_FLOAT },
		});

		s_Data->LightUniformBuffer = UniformBuffer::Create(sizeof(Light), 2, SHADER_STAGE_FRAGMENT, {
			{ "Position", UNIFORM_TYPE_FLOAT3 },
			{ "Ambient", UNIFORM_TYPE_FLOAT3 },
			{ "Diffuse", UNIFORM_TYPE_FLOAT3 },
			{ "Specular", UNIFORM_TYPE_FLOAT3 },
		});
		
		s_Data->material = { 0.1f, 0.3f, 0.5f, 256.0f };
		s_Data->light = { { 2.0f, 2.0f, 3.0f }, { 0.1f, 0.1f, 0.1f }, { 0.5f, 0.5f, 0.5f }, { 0.7f, 0.7f, 0.7f } };

		UniformBuffer* buffers[] = { s_Data->TransformsUB.get(), s_Data->MaterialUniformBuffer.get(), s_Data->LightUniformBuffer.get() };
		
		GraphicsPipelineCreateInfo info{};
		info.VertexShader = Renderer::DefaultVertexPath.c_str();
		info.FragmentShader = Renderer::DefaultFragmentPath.c_str();
		info.UniformBufferCount = 3;
		info.pUniformBuffers = buffers;

		s_Data->GraphicsPipeline = GraphicsPipeline::Create(&info);
		s_Data->GraphicsPipeline->Bind();

		s_Data->TransformsUB->SetUniformMat4("Model", glm::mat4(1.0f));
		s_Data->MaterialUniformBuffer->Update(&s_Data->material, sizeof(s_Data->material));
		s_Data->LightUniformBuffer->Update(&s_Data->light, sizeof(s_Data->light));
		AssetManager::Init();
	}

	void Renderer::Shutdown()
	{
		delete s_Data;
		AssetManager::ShutDown();
		RenderCommand::ShutDown();
	}

	void Renderer::BeginFrame()
	{
		RenderCommand::BeginFrame();
		if (s_ActiveCamera)
		{
			s_ActiveCamera->Update();
			s_Data->TransformsUB->SetUniformMat4("ViewProj", s_ActiveCamera->GetTransform());
		}
		Scene* scene = SceneManager::GetScene();
		RenderScene(scene);
	}

	void Renderer::EndFrame()
	{
		RenderCommand::SwapBuffers();
	}

	void Renderer::RenderScene(Scene* scene)
	{
		if (scene == nullptr)
			return;

		for (const Entity& entity : scene->GetEntities())
		{
			Renderer::RenderEntity(&entity);
		}
	}

	void Renderer::RenderEntity(const Entity* entity)
	{
		const UUID* transformId = nullptr;
		const UUID* meshId = nullptr;
		const UUID* textureId = nullptr;
		for (const Component& component : entity->GetComponents())
		{
			switch (component.type)
			{
			case ComponentType::TRANSFORM:
			{
				transformId = &component.id;
			} break;
			case ComponentType::MESH:
			{
				meshId = &component.id;
			} break;
			case ComponentType::TEXTURE:
			{
				textureId = &component.id;
			} break;
			}
		}

		if (transformId == nullptr || meshId == nullptr)
			return;

		MeshComponent* mc = ComponentManager::GetMeshComponent(*meshId);
		Texture* texture = AssetManager::LoadDefaultTexture();
		if (textureId)
		{
			TextureComponent* tc = ComponentManager::GetTextureComponent(*textureId);
			if (!tc->textureId.IsNull())
				texture = AssetManager::LoadTexture(tc->textureId);
		}
		Mesh* mesh = AssetManager::LoadMesh(mc->meshId);
		RenderCommand::SetTransform(*transformId);

		texture->Bind();
		RenderCommand::DrawIndexed(mesh);
	}
	
}