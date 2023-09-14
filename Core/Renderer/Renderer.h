#pragma once
#include "Macros.h"
#include "Renderer/RendererAPI.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "Texture.h"
#include "Core/PerspectiveCamera.h"
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Scene.h"

namespace Orchis {

	struct Material
	{
		float Ambient;
		float Diffuse;
		float Specular;
		float Shininess;
	};

	struct Light
	{
		alignas(16) glm::vec3 Position;
		alignas(16) glm::vec3 Ambient;
		alignas(16) glm::vec3 Diffuse;
		alignas(16) glm::vec3 Specular;
	};

	struct RenderData
	{
		Ref<GraphicsPipeline> GraphicsPipeline;
		Ref<UniformBuffer> TransformsUB;
		Ref<UniformBuffer> MaterialUniformBuffer;
		Ref<UniformBuffer> LightUniformBuffer;
		Material material;
		Light light;
	};



	class ORCHIS_API Renderer
	{
	public:
		Renderer() = delete;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		~Renderer() = delete;

		static void Init();
		static void Shutdown();

		static void BeginScene(Scene* scene);
		static void EndScene();

	private:
		static RendererAPI* s_RenderAPI;
		static RenderData* s_Data;
	};
}