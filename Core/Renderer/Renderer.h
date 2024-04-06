#pragma once
#include "Macros.h"
#include <thread>
#include <glm/glm.hpp>
#include "Renderer/RendererAPI.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "Texture.h"
#include "Core/PerspectiveCamera.h"
#include "Mesh.h"
#include "Scene.h"
#include "SceneManager.h"

namespace Orchis {

	struct OC_API Material
	{
		float Ambient;
		float Diffuse;
		float Specular;
		float Shininess;
	};

	struct OC_API Light
	{
		alignas(16) glm::vec3 Position;
		alignas(16) glm::vec3 Ambient;
		alignas(16) glm::vec3 Diffuse;
		alignas(16) glm::vec3 Specular;
	};

	struct OC_API RenderData
	{
		Ref<GraphicsPipeline> GraphicsPipeline;
		Ref<UniformBuffer> TransformsUB;
		Ref<UniformBuffer> MaterialUniformBuffer;
		Ref<UniformBuffer> LightUniformBuffer;
		Material material;
		Light light;
	};

	class OC_API Renderer
	{
	public:
		Renderer() = delete;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		~Renderer() = delete;

		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();
			
		static void SetActiveCamera(PerspectiveCamera* camera) { s_ActiveCamera = camera; }
		static const PerspectiveCamera* GetActiveCamera() { return s_ActiveCamera; }
	private:
		static std::thread s_RenderThread;
		static RenderData* s_Data;
		static PerspectiveCamera* s_ActiveCamera;


		static std::string DefaultVertexPath;
		static std::string DefaultFragmentPath;
	};
}