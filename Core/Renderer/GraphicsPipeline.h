#pragma once
#include "Macros.h"
#include "UniformBuffer.h"
#include "Texture.h"

namespace Orchis {

	enum VertexLayoutMode
	{
		VERTEX_LAYOUT_MODE_DEFAUT, VERTEX_LAYOUT_MODE_CUSTOM
	};

	enum VertexAttributeType
	{
		VERTEX_ATTRIBUTE_TYPE_FLOAT, VERTEX_ATTRIBUTE_TYPE_FLOAT2, VERTEX_ATTRIBUTE_TYPE_FLOAT3, VERTEX_ATTRIBUTE_TYPE_FLOAT4,
		VERTEX_ATTRIBUTE_TYPE_INT
	};

	struct VertexAttribPointer
	{
		VertexAttributeType Type;

		uint32_t GetVertexAttribSize() const;
	};

	struct GraphicsPipelineCreateInfo
	{
		const char* VertexShader = nullptr;
		const char* FragmentShader = nullptr;
		uint32_t UniformBufferCount = 0;
		UniformBuffer** pUniformBuffers;
		VertexLayoutMode VertexInputLayoutMode = VERTEX_LAYOUT_MODE_DEFAUT;
		uint32_t VertexAttribPointerCount = 0;
		VertexAttribPointer* pVertexAttribPointers = nullptr;
	};

	class GraphicsPipeline
	{
	public:
		GraphicsPipeline() = default;
		virtual ~GraphicsPipeline() = default;

		virtual void Bind() = 0;

		static Ref<GraphicsPipeline> Create(GraphicsPipelineCreateInfo* createInfo);
	};
}