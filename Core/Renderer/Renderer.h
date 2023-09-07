#pragma once
#include "Macros.h"
#include "Renderer/RendererAPI.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "Texture.h"
#include "Core/PerspectiveCamera.h"
#include <glm/glm.hpp>

namespace Orchis {

	struct RenderData
	{
		Ref<GraphicsPipeline> GraphicsPipeline;
		Ref<VertexBuffer> VertexBuffer;
		Ref<IndexBuffer> IndexBuffer;
		Ref<UniformBuffer> TransformsUB;
		Ref<UniformBuffer> ColorUB;
		Ref<Texture> Texture;
		DefaultVertex Vertices[24] = {
			//front
			{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, {0.0f, 1.0f} },
			{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, {1.0f, 1.0f} },
			{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }, {1.0f, 0.0f} },
			{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
			//back
			{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, {0.0f, 1.0f} },
			{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, {1.0f, 1.0f} },
			{ { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }, {1.0f, 0.0f} },
			{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
			//left
			{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, {0.0f, 1.0f} },
			{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, {1.0f, 1.0f} },
			{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }, {1.0f, 0.0f} },
			{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
			//right
			{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, {0.0f, 1.0f} },
			{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, {1.0f, 1.0f} },
			{ {  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }, {1.0f, 0.0f} },
			{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
			//top
			{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, {0.0f, 1.0f} },
			{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }, {1.0f, 1.0f} },
			{ {  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }, {1.0f, 0.0f} },
			{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
			//bottom
			{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, {0.0f, 1.0f} },
			{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, {1.0f, 1.0f} },
			{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, {1.0f, 0.0f} },
			{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
		};
		uint16_t Indices[36] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};
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
		
		static void DrawCube(glm::vec3 position);

		static void BeginScene(PerspectiveCamera* camera);
		static void EndScene();


	private:
		static RendererAPI* s_RenderAPI;
		static RenderData* s_Data;
	};
}