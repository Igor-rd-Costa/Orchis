#pragma once
#include <vulkan/vulkan.h>

namespace Orchis {

	class VulkanSwapchain
	{
	public:
		VulkanSwapchain();
		~VulkanSwapchain();

		VkExtent2D GetExtent() const { return m_Extent; }
		VkFormat GetImageFormat() const { return m_ImageFormat; }
		uint32_t GetExtentWidth() const { return m_Extent.width; }
		uint32_t GetExtentHeight() const { return m_Extent.height; }
		const VkFramebuffer& GetFramebuffer(uint32_t index) const { return m_Framebuffers[index]; };
		VkSwapchainKHR Get() { return m_Swapchain; }

		void CreateFramebuffers();
		void CreateDepthImages();
		void Recreate();

		static bool IsSwapchainAdequate(VkPhysicalDevice device);
	private:
		inline void CreateSwapchain();
		inline void CreateImageViews();

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR Capabilities;
			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;
		};
	private:
		static SwapChainSupportDetails QuerySwapchainSupportDetails(VkPhysicalDevice device);
		VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		VkSwapchainKHR m_Swapchain;
		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;
		std::vector<VkFramebuffer> m_Framebuffers;
		VkFormat m_ImageFormat;
		VkExtent2D m_Extent;
	private:
		struct DepthImage
		{
			VkImage Image;
			VkDeviceMemory ImageMemory;
			VkImageView ImageView;
		};
		std::vector<DepthImage> m_DepthImages;
	};
}