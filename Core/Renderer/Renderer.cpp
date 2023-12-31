#include "OrchisPCH.h"
#include "Renderer.h"
#include "Platform/Vulkan/VulkanAPI.h"
#include "VertexBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Application.h"
#include "RenderCommand.h"
#include "AssetManager.h"
namespace Orchis {

	RendererAPI* Renderer::s_RenderAPI = new VulkanAPI;
	RenderData* Renderer::s_Data = new RenderData;

	void Renderer::Init()
	{
		s_RenderAPI->Init();
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
		info.VertexShader = "../Core/Assets/Shaders/bin/vert.spv";
		info.FragmentShader = "../Core/Assets/Shaders/bin/frag.spv";
		info.UniformBufferCount = 3;
		info.pUniformBuffers = buffers;

		s_Data->GraphicsPipeline = GraphicsPipeline::Create(&info);
		s_Data->GraphicsPipeline->Bind();

		s_Data->TransformsUB->SetUniformMat4("Model", glm::mat4(1.0f));
		s_Data->MaterialUniformBuffer->Update(&s_Data->material, sizeof(s_Data->material));
		s_Data->LightUniformBuffer->Update(&s_Data->light, sizeof(s_Data->light));
	}

	void Renderer::Shutdown()
	{
		delete s_Data;
		AssetManager::ShutDown();
		RenderCommand::ShutDown();
		s_RenderAPI->ShutDown();

	}

	void Renderer::BeginScene(Scene* scene)
	{
		RenderCommand::BeginFrame(scene);
		scene->GetCamera()->Update();
		s_Data->TransformsUB->SetUniformMat4("ViewProj", scene->GetCamera()->GetTransform());

		for (const Mesh* mesh : scene->m_Meshes)
		{
			RenderCommand::DrawIndexed(mesh);
		}
	}

	void Renderer::EndScene()
	{
		RenderCommand::SwapBuffers();
	}

}