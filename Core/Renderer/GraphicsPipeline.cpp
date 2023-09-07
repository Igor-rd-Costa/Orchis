#include "OrchisPCH.h"
#include "GraphicsPipeline.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include <glm/glm.hpp>

namespace Orchis {

	Ref<GraphicsPipeline> GraphicsPipeline::Create(GraphicsPipelineCreateInfo* createInfo)
	{
		return CreateRef<VulkanGraphicsPipeline>(createInfo);
	}

	uint32_t VertexAttribPointer::GetVertexAttribSize() const
	{
		switch (Type)
		{
			case VERTEX_ATTRIBUTE_TYPE_FLOAT: return sizeof(float);
			case VERTEX_ATTRIBUTE_TYPE_FLOAT2: return sizeof(glm::vec2);
			case VERTEX_ATTRIBUTE_TYPE_FLOAT3: return sizeof(glm::vec3);
			case VERTEX_ATTRIBUTE_TYPE_FLOAT4: return sizeof(glm::vec4);
			case VERTEX_ATTRIBUTE_TYPE_INT: return sizeof(int);
		}
		OC_ASSERT(false);
		return 0;
	}

}
