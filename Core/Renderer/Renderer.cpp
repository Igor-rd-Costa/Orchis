#include "OrchisPCH.h"
#include "Renderer.h"
#include "Platform/Vulkan/VulkanAPI.h"
#include "VertexBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Application.h"
#include <bitset>
namespace Orchis {

	RendererAPI* Renderer::s_RenderAPI = new VulkanAPI;
	RenderData* Renderer::s_Data = new RenderData;

	static bool textureSwitch = false;

	void Renderer::Init()
	{
		s_RenderAPI->Init();
		s_Data->VertexBuffer = VertexBuffer::Create(s_Data->Vertices, sizeof(s_Data->Vertices));
		s_Data->IndexBuffer = IndexBuffer::Create(s_Data->Indices, sizeof(s_Data->Indices), INDEX_TYPE_UINT16);

		s_Data->TransformsUB = UniformBuffer::Create(sizeof(glm::mat4)*2, 0, SHADER_STAGE_VERTEX, {
			{ "ViewProj", UNIFORM_TYPE_MAT4 },
			{ "Model", UNIFORM_TYPE_MAT4 },
		});
		s_Data->ColorUB = UniformBuffer::Create(sizeof(glm::vec3), 1, SHADER_STAGE_FRAGMENT, {
			{ "ColorTint", UNIFORM_TYPE_FLOAT3 }
		});
		
		s_Data->Texture = Texture::Create("../Core/Assets/Textures/texture.jpg", 0, SHADER_STAGE_FRAGMENT);

		UniformBuffer* buffers[] = { s_Data->TransformsUB.get(), s_Data->ColorUB.get() };
		Texture* textures[] = { s_Data->Texture.get() };
		
		GraphicsPipelineCreateInfo info{};
		info.VertexShader = "../Core/Assets/Shaders/bin/vert.spv";
		info.FragmentShader = "../Core/Assets/Shaders/bin/frag.spv";
		info.UniformBufferCount = 2;
		info.pUniformBuffers = buffers;
		info.TexturesCount = 1;
		info.pTextures = textures;

		s_Data->GraphicsPipeline = GraphicsPipeline::Create(&info);
		s_Data->GraphicsPipeline->Bind();

		s_Data->TransformsUB->SetUniformMat4("Model", glm::mat4(1.0f));
		s_Data->ColorUB->SetUniformFloat3("ColorTint", { 1.0f, 0.5f, 0.5f });
	}

	void Renderer::Shutdown()
	{
		delete s_Data;
		s_RenderAPI->Shutdown();
	}

	void Renderer::BeginScene(PerspectiveCamera* camera)
	{
		if (Input::KeyPressed(Key::KEY_F))
		{
			if (textureSwitch)
			{
				s_Data->Texture->Load("../Core/Assets/Textures/texture.jpg");
			}
			else
			{
				s_Data->Texture->Load("../Core/Assets/Textures/CheckerBoard.png");
			}
			textureSwitch = !textureSwitch;
		}
		s_RenderAPI->StartFrame();
		camera->Update();
		s_Data->VertexBuffer->Bind();
		s_Data->IndexBuffer->Bind();
		s_Data->TransformsUB->SetUniformMat4("ViewProj", camera->GetTransform());
	}

	void Renderer::DrawCube(glm::vec3 position)
	{
		s_Data->TransformsUB->SetUniformMat4("Model", glm::translate(glm::mat4(1.0f), position));
		s_RenderAPI->DrawCube();
	}

	void Renderer::EndScene()
	{
		s_RenderAPI->SwapBuffers();
	}
}