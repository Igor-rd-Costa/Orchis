#pragma once
#include <vulkan/vulkan.h>
#include "Macros.h"
#include "Renderer/RendererAPI.h"
#include "../Debug/Logger.h"
#include "VulkanSwapchain.h"
#include <glm/glm.hpp>
#include <optional>
#include "VulkanGraphicsPipeline.h"
#include "VulkanRenderPassManager.h"


namespace Orchis {

	VkFormat VertexAttribFormatToVkFormat(VertexAttributeType type);
	VkFormat FindSuportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	std::pair<uint32_t, VkFormat> VertexAttribInfoToVkAttribInfo(const VertexAttribPointer& vertexAttribInfo);
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkImage& image, VkDeviceMemory& imageMemory);
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	class VulkanAPI : public RendererAPI
	{
	public:
		struct QueueFamilyIndices
		{
			std::optional<uint32_t> GraphicsFamily;
			std::optional<uint32_t> PresentFamily;

			bool IsComplete()
			{
				return GraphicsFamily.has_value() && PresentFamily.has_value();
			}
		};
	public:
		VulkanAPI(void* windowHandle);
		~VulkanAPI();

		static void BeginFrame();
		static void SwapBuffers();

		inline static VkDevice GetDevice() { return s_Device; }
		inline static VkPhysicalDevice GetPhysicalDevice() { return s_PhysDevice; }
		inline static VkSurfaceKHR GetSurface() { return s_Surface; }
		inline static const VulkanSwapchain* GetSwapchain() { return s_Swapchain; }
		inline static const VulkanGraphicsPipeline* GetGraphicsPipeline() { return s_GraphicsPipeline; }
		inline static const QueueFamilyIndices& GetQueueFamilies() { return s_QueueFamilies; }
		inline static VkCommandPool GetCommandPool() { return s_CommandPool; }
		inline static VkCommandBuffer GetCommandBuffer() { return s_CommandBuffers[s_CurrentFrame]; }
		inline static VkQueue GetGraphicsQueue() { return s_GraphicsQueue; }
		inline static VkQueue GetPresentQueue() { return s_PresentQueue; }

		static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags flags);
		static VkShaderStageFlags ShaderStageFlagsToVkShaderStageFlags(ShaderStageFlags flags);
		

		static Logger s_Logger;
		static const uint32_t s_MaxFramesInFlight = 2;
		static uint32_t s_CurrentFrame;
		static uint32_t s_ImageIndex;
	private:


		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR Capabilities;
			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;
		};
	private:
		void CreateInstance();
		// Implemented in platform specific files
		void CreateSurface(void* windowHandle);
		void PickPhysicalDevice();
		void CreateLogicalDevice();

		void CreateCommandPool();
		void CreateCommandBuffer();
		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void CreateSyncObjects();


		bool CheckValidationLayerSupport();
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		bool IsDeviceSuitable(VkPhysicalDevice device);
	private:
		static VkInstance s_Instance;
		static VkPhysicalDevice s_PhysDevice;
		static VkDevice s_Device;
		static VkSurfaceKHR s_Surface;
		static QueueFamilyIndices s_QueueFamilies;
		static VkQueue s_GraphicsQueue;
		static VkQueue s_PresentQueue;
		static VulkanSwapchain* s_Swapchain;
		static VulkanGraphicsPipeline* s_GraphicsPipeline;
		static VkCommandPool s_CommandPool;
		static std::array<VkCommandBuffer, s_MaxFramesInFlight> s_CommandBuffers;
		static std::array<VkSemaphore, s_MaxFramesInFlight> s_ImageAvailableSemaphores;
		static std::array<VkSemaphore, s_MaxFramesInFlight> s_RenderFinishedSemaphores;
		static std::array<VkFence, s_MaxFramesInFlight> s_InFlightFences;
	};
}