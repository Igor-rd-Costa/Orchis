#pragma once
#include "Macros.h"
#include <glm/glm.hpp>
#include "Renderer/Flags.h"
#include <vector>

namespace Orchis {

	enum UniformType
	{
		UNIFORM_TYPE_FLOAT, UNIFORM_TYPE_FLOAT2, UNIFORM_TYPE_FLOAT3, UNIFORM_TYPE_FLOAT4,
		UNIFORM_TYPE_MAT4
	};

	uint32_t GetUniformTypeSize(UniformType type);
	uint32_t GetGLSLTypeOffset(UniformType type);
	struct UniformInfo
	{
		std::string Name;
		UniformType Type;
	};

	class UniformBuffer
	{
	public:
		UniformBuffer() = default;
		virtual ~UniformBuffer() = default;

		virtual void Update(void* data, size_t size) = 0;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& data) = 0;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& data) = 0;
		virtual void SetUniformFloat(const std::string& name, float data) = 0;

		static Ref<UniformBuffer> Create(size_t size, uint32_t binding, ShaderStageFlags stageFlags, const std::vector<UniformInfo>& uniformInfo);
	};
}