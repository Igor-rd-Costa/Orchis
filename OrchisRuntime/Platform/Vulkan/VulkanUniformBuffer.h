#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "Renderer/UniformBuffer.h"
#include <unordered_map>

namespace Orchis {

	class VulkanUniformBuffer : public UniformBuffer
	{
	private:
		struct VulkanUniformBufferData
		{
			VkBuffer Buffer;
			VkDeviceMemory Memory;
			void* WritePtr;
		};
	public:
		VulkanUniformBuffer(size_t size, uint32_t binding, ShaderStageFlags stageFlags, const std::vector<UniformInfo>& uniformInfo);
		~VulkanUniformBuffer();

		virtual void Update(void* data, size_t size) override;
	
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& data) override;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& data) override;
		virtual void SetUniformFloat(const std::string& name, float data) override;

		const std::vector<VulkanUniformBufferData>& GetBuffers() { return m_BufferData; }
		size_t GetSize() const { return m_Size; }
		uint32_t GetBinding() const { return m_Binding; }
		VkShaderStageFlags GetStageFlags() const { return m_StageFlags; }

	private:
		struct UniformData
		{
			uint32_t Size;
			uint32_t Offset;
		};
		uint32_t m_Binding;
		size_t m_Size;
		VkShaderStageFlags m_StageFlags;
		std::vector<VulkanUniformBufferData> m_BufferData;
		std::unordered_map<std::string, UniformData> m_Uniforms;
	};
}