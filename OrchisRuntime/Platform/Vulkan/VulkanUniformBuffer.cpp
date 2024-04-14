#include "VulkanUniformBuffer.h"
#include "VulkanBuffer.h"
#include "VulkanDescriptorSets.h"
#include "VulkanAPI.h"

namespace Orchis {

	VulkanUniformBuffer::VulkanUniformBuffer(size_t size, uint32_t binding, ShaderStageFlags stageFlags, const std::vector<UniformInfo>& uniformInfo)
		: m_Binding(binding), m_Size(size), m_StageFlags(VulkanAPI::ShaderStageFlagsToVkShaderStageFlags(stageFlags))
	{
		uint32_t offset = 0;
		for (uint32_t i = 0; i < uniformInfo.size(); i++)
		{
			uint32_t typeSize = GetUniformTypeSize(uniformInfo[i].Type);
			m_Uniforms.insert({ uniformInfo[i].Name, { typeSize, offset } });
			offset += GetGLSLTypeOffset(uniformInfo[i].Type);
		}

		m_BufferData.resize(VulkanAPI::s_MaxFramesInFlight);
		VkBufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = size;
		createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkResult result = VK_SUCCESS;
		for (uint32_t i = 0; i < VulkanAPI::s_MaxFramesInFlight; i++)
		{
			result = vkCreateBuffer(VulkanAPI::GetDevice(), &createInfo, nullptr, &m_BufferData[i].Buffer);
			OC_ASSERT(result == VK_SUCCESS);

			VkMemoryRequirements memoryRequirements;
			vkGetBufferMemoryRequirements(VulkanAPI::GetDevice(), m_BufferData[i].Buffer, &memoryRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memoryRequirements.size;
			allocInfo.memoryTypeIndex = VulkanAPI::FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			result = vkAllocateMemory(VulkanAPI::GetDevice(), &allocInfo, nullptr, &m_BufferData[i].Memory);
			OC_ASSERT(result == VK_SUCCESS);

			vkBindBufferMemory(VulkanAPI::GetDevice(), m_BufferData[i].Buffer, m_BufferData[i].Memory, 0);
		
			vkMapMemory(VulkanAPI::GetDevice(), m_BufferData[i].Memory, 0, size, 0, &m_BufferData[i].WritePtr);
		}

	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		vkDeviceWaitIdle(VulkanAPI::GetDevice());
		for (uint32_t i = 0; i < VulkanAPI::s_MaxFramesInFlight; i++)
		{
			vkDestroyBuffer(VulkanAPI::GetDevice(), m_BufferData[i].Buffer, nullptr);
			vkFreeMemory(VulkanAPI::GetDevice(), m_BufferData[i].Memory, nullptr);
		}
	}

	void VulkanUniformBuffer::Update(void* data, size_t size)
	{
		memcpy(m_BufferData[VulkanAPI::s_CurrentFrame].WritePtr, data, size);
	}

	void VulkanUniformBuffer::SetUniformMat4(const std::string& name, const glm::mat4& data)
	{
		memcpy((char*)m_BufferData[VulkanAPI::s_CurrentFrame].WritePtr + m_Uniforms[name].Offset, &data, m_Uniforms[name].Size);
	}

	void VulkanUniformBuffer::SetUniformFloat3(const std::string& name, const glm::vec3& data)
	{
		memcpy((char*)m_BufferData[VulkanAPI::s_CurrentFrame].WritePtr + m_Uniforms[name].Offset, &data, m_Uniforms[name].Size);
	}

	void VulkanUniformBuffer::SetUniformFloat(const std::string& name, float data)
	{
		memcpy((char*)m_BufferData[VulkanAPI::s_CurrentFrame].WritePtr + m_Uniforms[name].Offset, &data, m_Uniforms[name].Size);
	}

}
