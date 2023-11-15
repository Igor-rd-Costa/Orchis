#include "Macros.h"
#include "VulkanSwapchain.h"
#include "VulkanAPI.h"
#include "Application.h"
#include <algorithm>

namespace Orchis {

	VulkanSwapchain::VulkanSwapchain()
	{
		CreateSwapchain();
		CreateImageViews();
		VulkanAPI::s_Logger.Info("Swapchain created successfully.");
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		for (VkFramebuffer framebuffer : m_Framebuffers)
		{
			vkDestroyFramebuffer(VulkanAPI::GetDevice(), framebuffer, nullptr);
		}
		for (VkImageView imageView : m_ImageViews)
		{
			vkDestroyImageView(VulkanAPI::GetDevice(), imageView, nullptr);
		}
		for (DepthImage& depthImage : m_DepthImages)
		{
			vkDestroyImageView(VulkanAPI::GetDevice(), depthImage.ImageView, nullptr);
			vkDestroyImage(VulkanAPI::GetDevice(), depthImage.Image, nullptr);
			vkFreeMemory(VulkanAPI::GetDevice(), depthImage.ImageMemory, nullptr);
		}
		vkDestroySwapchainKHR(VulkanAPI::GetDevice(), m_Swapchain, nullptr);
	}

	void VulkanSwapchain::CreateFramebuffers()
	{
		m_Framebuffers.resize(m_ImageViews.size());

		VkResult result = VK_SUCCESS;
		for (uint32_t i = 0; i < m_ImageViews.size(); i++)
		{
			std::array<VkImageView, 2> attachments = {
				m_ImageViews[i],
				m_DepthImages[i].ImageView
			};

			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.renderPass = VulkanRenderPassManager::GetRenderPass();
			createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			createInfo.pAttachments = attachments.data();
			createInfo.width = m_Extent.width;
			createInfo.height = m_Extent.height;
			createInfo.layers = 1;

			result = vkCreateFramebuffer(VulkanAPI::GetDevice(), &createInfo, nullptr, &m_Framebuffers[i]);
			OC_ASSERT(result == VK_SUCCESS);
		}
	}

	void VulkanSwapchain::CreateDepthImages()
	{
		VkFormat depthFormat = FindSuportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

		m_DepthImages.resize(m_ImageViews.size());
		for (uint32_t i = 0; i < m_ImageViews.size(); i++)
		{
			CreateImage(m_Extent.width, m_Extent.height, depthFormat,
				VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				m_DepthImages[i].Image, m_DepthImages[i].ImageMemory);

			m_DepthImages[i].ImageView = CreateImageView(m_DepthImages[i].Image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
			
			TransitionImageLayout(m_DepthImages[i].Image, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
		}

	}

	void VulkanSwapchain::Recreate()
	{
		vkDeviceWaitIdle(VulkanAPI::GetDevice());

		for (VkFramebuffer framebuffer : m_Framebuffers)
		{
			vkDestroyFramebuffer(VulkanAPI::GetDevice(), framebuffer, nullptr);
		}
		for (VkImageView imageView : m_ImageViews)
		{
			vkDestroyImageView(VulkanAPI::GetDevice(), imageView, nullptr);
		}
		for (DepthImage& depthImage : m_DepthImages)
		{
			vkDestroyImageView(VulkanAPI::GetDevice(), depthImage.ImageView, nullptr);
			vkDestroyImage(VulkanAPI::GetDevice(), depthImage.Image, nullptr);
			vkFreeMemory(VulkanAPI::GetDevice(), depthImage.ImageMemory, nullptr);
		}
		vkDestroySwapchainKHR(VulkanAPI::GetDevice(), m_Swapchain, nullptr);
		
		CreateSwapchain();
		CreateImageViews();
		CreateDepthImages();
		CreateFramebuffers();
		VulkanAPI::s_Logger.Info("Swapchain recreated successfully.");
	}

	bool VulkanSwapchain::IsSwapchainAdequate(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details = QuerySwapchainSupportDetails(device);
		return !details.Formats.empty() && !details.PresentModes.empty();
	}

	void VulkanSwapchain::CreateSwapchain()
	{
		SwapChainSupportDetails swapchainSupport = QuerySwapchainSupportDetails(VulkanAPI::GetPhysicalDevice());
		VkSurfaceFormatKHR surfaceFormat = ChooseSwapchainSurfaceFormat(swapchainSupport.Formats);
		VkPresentModeKHR presentMode = ChooseSwapchainPresentMode(swapchainSupport.PresentModes);
		m_Extent = ChooseSwapchainExtent(swapchainSupport.Capabilities);
		uint32_t imageCount = swapchainSupport.Capabilities.minImageCount + 1;
		if (swapchainSupport.Capabilities.maxImageCount > 0 && imageCount > swapchainSupport.Capabilities.maxImageCount)
			imageCount = swapchainSupport.Capabilities.maxImageCount;

		m_ImageFormat = surfaceFormat.format;

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = VulkanAPI::GetSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = m_ImageFormat;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_Extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		VulkanAPI::QueueFamilyIndices indices = VulkanAPI::FindQueueFamilies(VulkanAPI::GetPhysicalDevice());

		if (indices.GraphicsFamily != indices.PresentFamily)
		{
			uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapchainSupport.Capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		VkResult result = vkCreateSwapchainKHR(VulkanAPI::GetDevice(), &createInfo, nullptr, &m_Swapchain);
		OC_ASSERT(result == VK_SUCCESS);

		vkGetSwapchainImagesKHR(VulkanAPI::GetDevice(), m_Swapchain, &imageCount, nullptr);
		m_Images.resize(imageCount);
		vkGetSwapchainImagesKHR(VulkanAPI::GetDevice(), m_Swapchain, &imageCount, m_Images.data());
	}

	void VulkanSwapchain::CreateImageViews()
	{
		m_ImageViews.resize(m_Images.size());
		for (uint32_t i = 0; i < m_Images.size(); i++)
			m_ImageViews[i] = CreateImageView(m_Images[i], m_ImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	VulkanSwapchain::SwapChainSupportDetails VulkanSwapchain::QuerySwapchainSupportDetails(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;

		VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, VulkanAPI::GetSurface(), &details.Capabilities);
		OC_ASSERT(result == VK_SUCCESS);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanAPI::GetSurface(), &formatCount, nullptr);
		if (formatCount != 0)
		{
			details.Formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanAPI::GetSurface(), &formatCount, details.Formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanAPI::GetSurface(), &presentModeCount, nullptr);
		if (presentModeCount != 0)
		{
			details.PresentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanAPI::GetSurface(), &presentModeCount, details.PresentModes.data());
		}

		return details;
	}
	VkSurfaceFormatKHR VulkanSwapchain::ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const VkSurfaceFormatKHR& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return availableFormat;
		}
		return availableFormats[0];
	}
	VkPresentModeKHR VulkanSwapchain::ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const VkPresentModeKHR& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return availablePresentMode;
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}
	VkExtent2D VulkanSwapchain::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
			return capabilities.currentExtent;

		uint32_t width = Application::GetWindow()->GetWidth();
		uint32_t height = Application::GetWindow()->GetHeight();

		VkExtent2D extent{ width, height };
		extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return extent;
	}
}