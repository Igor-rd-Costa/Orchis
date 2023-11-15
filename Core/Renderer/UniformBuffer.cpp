#include "UniformBuffer.h"
#include "Platform/Vulkan/VulkanUniformBuffer.h"
#include <glm/glm.hpp>
namespace Orchis {

	uint32_t GetUniformTypeSize(UniformType type)
	{
		switch (type)
		{
		case Orchis::UNIFORM_TYPE_FLOAT: return sizeof(float);
		case Orchis::UNIFORM_TYPE_FLOAT2: return sizeof(glm::vec2);
		case Orchis::UNIFORM_TYPE_FLOAT3: return sizeof(glm::vec3);
		case Orchis::UNIFORM_TYPE_FLOAT4: return sizeof(glm::vec4);
		case Orchis::UNIFORM_TYPE_MAT4: return sizeof(glm::mat4);
		}
		return 0;
	}

	uint32_t GetGLSLTypeOffset(UniformType type)
	{
		// based on glsl alignments
		switch (type)
		{
			case Orchis::UNIFORM_TYPE_FLOAT: return sizeof(float);
			case Orchis::UNIFORM_TYPE_FLOAT2: return sizeof(glm::vec2);
			case Orchis::UNIFORM_TYPE_FLOAT3:
			case Orchis::UNIFORM_TYPE_FLOAT4: return sizeof(glm::vec4);
			case Orchis::UNIFORM_TYPE_MAT4: return sizeof(glm::mat4);
		}
		return 0;
	}

	Ref<UniformBuffer> UniformBuffer::Create(size_t size, uint32_t binding, ShaderStageFlags stageFlags, const std::vector<UniformInfo>& uniformInfo)
	{
		return CreateRef<VulkanUniformBuffer>(size, binding, stageFlags, uniformInfo);
	}

}