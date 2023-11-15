#include "VulkanAPI.h"
#include "VulkanDebug.h"
#include "Window.h"
#include "Application.h"
#include <optional>
#include "VulkanStagingBuffer.h"
#include "VulkanDescriptorSets.h"
#include "ImageSamplerManager.h"
#include "VulkanRenderCommand.h"

namespace Orchis {

	static const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	static const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkInstance VulkanAPI::s_Instance;
	VkPhysicalDevice VulkanAPI::s_PhysDevice = VK_NULL_HANDLE;
	VkDevice VulkanAPI::s_Device;
	VkSurfaceKHR VulkanAPI::s_Surface;
	VulkanAPI::QueueFamilyIndices VulkanAPI::s_QueueFamilies;
	VkQueue VulkanAPI::s_GraphicsQueue;
	VkQueue VulkanAPI::s_PresentQueue;
	VulkanSwapchain* VulkanAPI::s_Swapchain = nullptr;
	VulkanGraphicsPipeline* VulkanAPI::s_GraphicsPipeline = nullptr;
	uint32_t VulkanAPI::s_ImageIndex = 0;
	uint32_t VulkanAPI::s_CurrentFrame = 0;
	VkCommandPool VulkanAPI::s_CommandPool;
	std::array<VkCommandBuffer, 2> VulkanAPI::s_CommandBuffers;
	std::array<VkSemaphore, 2> VulkanAPI::s_ImageAvailableSemaphores;
	std::array<VkSemaphore, 2> VulkanAPI::s_RenderFinishedSemaphores;
	std::array<VkFence, 2> VulkanAPI::s_InFlightFences;
	Logger VulkanAPI::s_Logger{ "Vulkan" };

	VulkanAPI::VulkanAPI(void* windowHandle)
	{
		CreateInstance();
		OC_VK_SETUP_DEBUGGER(s_Instance);
		CreateSurface(windowHandle);
		PickPhysicalDevice();
		CreateLogicalDevice();
		s_Swapchain = new VulkanSwapchain();
		VulkanRenderPassManager::CreateRenderPasses();
		ImageSamplerManager::Init();
		CreateCommandPool();
		s_Swapchain->CreateDepthImages();
		s_Swapchain->CreateFramebuffers();
		CreateCommandBuffer();
		CreateSyncObjects();
		VulkanStagingBuffer::Init(128'000'000);
	}

	VulkanAPI::~VulkanAPI()
	{
		vkDeviceWaitIdle(s_Device);

		VulkanStagingBuffer::Shutdown();
		for (uint32_t i = 0; i < s_MaxFramesInFlight; i++)
		{
			vkDestroySemaphore(s_Device, s_ImageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(s_Device, s_RenderFinishedSemaphores[i], nullptr);
			vkDestroyFence(s_Device, s_InFlightFences[i], nullptr);
		}
		vkDestroyCommandPool(s_Device, s_CommandPool, nullptr);
		VulkanDescriptorSetManager::DeleteUniformBuffersDescriptorSetLayout();
		VulkanDescriptorSetManager::DeleteImageSamplersDescriptorSetLayout();
		VulkanDescriptorSetManager::DeleteUniformBuffersDescriptorPool();
		VulkanDescriptorSetManager::DeleteImageSamplersDescriptorPool();
		ImageSamplerManager::Shutdown();
		delete s_GraphicsPipeline;
		VulkanRenderPassManager::DeleteRenderPasses();
		delete s_Swapchain;
		vkDestroyDevice(s_Device, nullptr);
		OC_VK_DESTROY_DEBUGGER(s_Instance);
		vkDestroySurfaceKHR(s_Instance, s_Surface, nullptr);
 		vkDestroyInstance(s_Instance, nullptr);
	}

	void VulkanAPI::BeginFrame()
	{
		vkWaitForFences(s_Device, 1, &s_InFlightFences[s_CurrentFrame], VK_TRUE, UINT64_MAX);

		static VkResult result = VK_SUCCESS;
		result = vkAcquireNextImageKHR(s_Device, s_Swapchain->Get(), UINT64_MAX, s_ImageAvailableSemaphores[s_CurrentFrame], VK_NULL_HANDLE, &VulkanAPI::s_ImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			s_Swapchain->Recreate();
			return;
		}
		else OC_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR);

		vkResetFences(s_Device, 1, &s_InFlightFences[s_CurrentFrame]);
	}

	void VulkanAPI::SwapBuffers()
	{
		static VkResult result = VK_SUCCESS;

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &s_ImageAvailableSemaphores[s_CurrentFrame];
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &VulkanRenderCommand::GetCommandBuffer();
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &s_RenderFinishedSemaphores[s_CurrentFrame];

		result = vkQueueSubmit(s_GraphicsQueue, 1, &submitInfo, s_InFlightFences[s_CurrentFrame]);
		OC_ASSERT(result == VK_SUCCESS);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &s_RenderFinishedSemaphores[s_CurrentFrame];
		presentInfo.swapchainCount = 1;
		VkSwapchainKHR swapchains[] = { s_Swapchain->Get() };
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &VulkanAPI::s_ImageIndex;
		presentInfo.pResults = nullptr;

		result = vkQueuePresentKHR(s_PresentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			s_Swapchain->Recreate();
		else OC_ASSERT(result == VK_SUCCESS)

		s_CurrentFrame = (s_CurrentFrame + 1) & s_MaxFramesInFlight;
	}

	void VulkanAPI::CreateInstance()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Sandbox";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Orchis";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_2;
		appInfo.pNext = nullptr;

		uint32_t extensionCount;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensionProperties(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());

		s_Logger.Info("Available extensions:");
		for (VkExtensionProperties& extension : extensionProperties)
		{
			std::cout << "\t" << extension.extensionName << "\n";
		}

		uint32_t windowExtensionsCount;
		const char** windowExtensions = Window::GetRequiredVKExtensions(&windowExtensionsCount);
		std::vector<const char*> requiredExtensions(windowExtensions, windowExtensions + windowExtensionsCount);

		uint32_t layerCount = 0;
		const char* const* layerNames = nullptr;
		void* pNext = nullptr;
#if OC_DEBUG_BUILD || OC_RELEASE_BUILD
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (CheckValidationLayerSupport())
		{
			layerCount = static_cast<uint32_t>(validationLayers.size());
			layerNames = validationLayers.data();
			requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			VulkanDebug::PopulateDebugMessengerCreateInfo(debugCreateInfo);
			pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

		}
		else
			s_Logger.Error("Failed to initialize validation layers. Initializing Vulkan without validation layers.");
#endif
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();
		createInfo.enabledLayerCount = layerCount;
		createInfo.ppEnabledLayerNames = layerNames;
		createInfo.pNext = pNext;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &s_Instance);
		OC_ASSERT(result == VK_SUCCESS);
	}

	void VulkanAPI::PickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(s_Instance, &deviceCount, nullptr);
		OC_ASSERT(deviceCount != 0);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(s_Instance, &deviceCount, devices.data());

		for (const VkPhysicalDevice& device : devices)
		{
			if (IsDeviceSuitable(device)) {
				s_PhysDevice = device;
				break;
			}
		}

		OC_ASSERT(s_PhysDevice != VK_NULL_HANDLE);

		VkPhysicalDeviceDescriptorIndexingProperties indexingProperties{};
		indexingProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES;
		VkPhysicalDeviceProperties2 properties;
		properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		properties.pNext = &indexingProperties;
		vkGetPhysicalDeviceProperties2(s_PhysDevice, &properties);

		int a = 5;
	}

	void VulkanAPI::CreateLogicalDevice()
	{
		float queuePriority = 1.0f;

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { s_QueueFamilies.GraphicsFamily.value(), s_QueueFamilies.PresentFamily.value() };

		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		
		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures{};
		descriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
		descriptorIndexingFeatures.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = &descriptorIndexingFeatures;

#if OC_DEBUG_BUILD || OC_RELEASE_BUILD
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
#endif


		VkResult result = vkCreateDevice(s_PhysDevice, &createInfo, nullptr, &s_Device);
		OC_ASSERT(result == VK_SUCCESS);

		vkGetDeviceQueue(s_Device, s_QueueFamilies.GraphicsFamily.value(), 0, &s_GraphicsQueue);
		vkGetDeviceQueue(s_Device, s_QueueFamilies.PresentFamily.value(), 0, &s_PresentQueue);
	}


	bool VulkanAPI::CheckValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> supportedLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, supportedLayers.data());

		for (const char* layer : validationLayers)
		{
			bool foundLayer = false;

			for (const VkLayerProperties& layerProperty : supportedLayers)
			{
				if (strcmp(layer, layerProperty.layerName) == 0) {
					foundLayer = true;
					break;
				}
			}

			if (!foundLayer) return false;
		}

	return true;
	}

	bool VulkanAPI::CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const VkExtensionProperties& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	bool VulkanAPI::IsDeviceSuitable(VkPhysicalDevice device)
	{
		bool supportsExtensions = CheckDeviceExtensionSupport(device);
		s_QueueFamilies = FindQueueFamilies(device);

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
		
		bool swapchainAdequate = false;
		if (supportsExtensions)
		{
			swapchainAdequate = VulkanSwapchain::IsSwapchainAdequate(device);
		}

		return s_QueueFamilies.IsComplete() && supportsExtensions && swapchainAdequate && supportedFeatures.samplerAnisotropy;
	}

	void VulkanAPI::CreateCommandPool()
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = s_QueueFamilies.GraphicsFamily.value();

		VkResult result = vkCreateCommandPool(s_Device, &poolInfo, nullptr, &s_CommandPool);
		OC_ASSERT(result == VK_SUCCESS);

		s_Logger.Info("Command pool created successfully.");

	}

	void VulkanAPI::CreateCommandBuffer()
	{
		VkCommandBufferAllocateInfo bufferAllocInfo{};
		bufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		bufferAllocInfo.commandPool = s_CommandPool;
		bufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		bufferAllocInfo.commandBufferCount = static_cast<uint32_t>(s_CommandBuffers.size());

		VkResult result = vkAllocateCommandBuffers(s_Device, &bufferAllocInfo, s_CommandBuffers.data());
	}

	void VulkanAPI::CreateSyncObjects()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (uint32_t i = 0; i < s_MaxFramesInFlight; i++)
		{
			VkResult result = vkCreateSemaphore(s_Device, &semaphoreInfo, nullptr, &s_ImageAvailableSemaphores[i]);
			OC_ASSERT(result == VK_SUCCESS);
			result = vkCreateSemaphore(s_Device, &semaphoreInfo, nullptr, &s_RenderFinishedSemaphores[i]);
			OC_ASSERT(result == VK_SUCCESS);
			result = vkCreateFence(s_Device, &fenceInfo, nullptr, &s_InFlightFences[i]);
			OC_ASSERT(result == VK_SUCCESS);
		}

	}

	void VulkanAPI::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;

		VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		OC_ASSERT(result == VK_SUCCESS);

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };
		clearValues[1].depthStencil = { 1.0f, 0 };
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = VulkanRenderPassManager::GetRenderPass();
		renderPassInfo.framebuffer = s_Swapchain->GetFramebuffer(imageIndex);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = s_Swapchain->GetExtent();
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		
		vkCmdBindPipeline(VulkanAPI::GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanGraphicsPipeline::GetBoundPipeline());

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(s_Swapchain->GetExtentWidth());
		viewport.height = static_cast<float>(s_Swapchain->GetExtentHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = s_Swapchain->GetExtent();
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	VulkanAPI::QueueFamilyIndices VulkanAPI::FindQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;

		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());
		int i = 0;
		for (const VkQueueFamilyProperties& queueFamily : queueFamilyProperties)
		{
			VkBool32 presentSupport = false;
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				indices.GraphicsFamily = i;

			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, s_Surface, &presentSupport);
			
			if (presentSupport)
				indices.PresentFamily = i;

			if (indices.IsComplete())
				break;

			i++;
		}

		return indices;
	}

	VkFormat VertexAttribFormatToVkFormat(VertexAttributeType type)
	{
		switch (type)
		{
			case VERTEX_ATTRIBUTE_TYPE_FLOAT: return VK_FORMAT_R32_SFLOAT;
			case VERTEX_ATTRIBUTE_TYPE_FLOAT2: return VK_FORMAT_R32G32_SFLOAT;
			case VERTEX_ATTRIBUTE_TYPE_FLOAT3: return VK_FORMAT_R32G32B32_SFLOAT;
			case VERTEX_ATTRIBUTE_TYPE_FLOAT4: return VK_FORMAT_R32G32B32A32_SFLOAT;
			case VERTEX_ATTRIBUTE_TYPE_INT: return VK_FORMAT_R32_SINT;
		}
		OC_ASSERT(false);
		return VK_FORMAT_MAX_ENUM;
	}

	VkFormat FindSuportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(VulkanAPI::GetPhysicalDevice(), format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
				return format;
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
				return format;
		}
		OC_ASSERT(false);
		return VK_FORMAT_MAX_ENUM;
	}

	std::pair<uint32_t, VkFormat> VertexAttribInfoToVkAttribInfo(const VertexAttribPointer& vertexAttribInfo)
	{
		std::pair<uint32_t, VkFormat> info;
		info.first = vertexAttribInfo.GetVertexAttribSize();
		info.second = VertexAttribFormatToVkFormat(vertexAttribInfo.Type);
		return info;
	}

	VkCommandBuffer BeginSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = VulkanAPI::GetCommandPool();
		allocInfo.commandBufferCount = 1;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(VulkanAPI::GetDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
		return commandBuffer;
	}

	void EndSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(VulkanAPI::GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(VulkanAPI::GetGraphicsQueue());

		vkFreeCommandBuffers(VulkanAPI::GetDevice(), VulkanAPI::GetCommandPool(), 1, &commandBuffer);
	}

	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		VkAccessFlags srcAccess;
		VkAccessFlags dstAccess;
		VkPipelineStageFlags srcStage;
		VkPipelineStageFlags dstStage;

		VkImageMemoryBarrier barrier{};
		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT)
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		else
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			srcAccess = 0;
			dstAccess = VK_ACCESS_TRANSFER_WRITE_BIT;
			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			srcAccess = VK_ACCESS_TRANSFER_WRITE_BIT;
			dstAccess = VK_ACCESS_SHADER_READ_BIT;
			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			srcAccess = 0;
			dstAccess = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else OC_ASSERT(false);


		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();
		

		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;
		barrier.srcAccessMask = srcAccess; 
		barrier.dstAccessMask = dstAccess;

		vkCmdPipelineBarrier(commandBuffer,
			srcStage, dstStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		EndSingleTimeCommands(commandBuffer);
	}

	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, 
		VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkImage& image, VkDeviceMemory& imageMemory)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

		VkResult result = vkCreateImage(VulkanAPI::GetDevice(), &imageInfo, nullptr, &image);
		OC_ASSERT(result == VK_SUCCESS);

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(VulkanAPI::GetDevice(), image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanAPI::FindMemoryType(memRequirements.memoryTypeBits, memoryProperties);

		result = vkAllocateMemory(VulkanAPI::GetDevice(), &allocInfo, nullptr, &imageMemory);
		OC_ASSERT(result == VK_SUCCESS);

		vkBindImageMemory(VulkanAPI::GetDevice(), image, imageMemory, 0);
	}

	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = image;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = format;
		createInfo.subresourceRange.aspectMask = aspectFlags;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.layerCount = 1;
		createInfo.subresourceRange.levelCount = 1;

		VkImageView imageView;
		VkResult result = vkCreateImageView(VulkanAPI::GetDevice(), &createInfo, nullptr, &imageView);
		OC_ASSERT(result == VK_SUCCESS);
		return imageView;
	}

	uint32_t VulkanAPI::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags flags)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(VulkanAPI::GetPhysicalDevice(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & flags) == flags)
				return i;
		}

		OC_ASSERT(false);
		return 0;
	}

	VkShaderStageFlags VulkanAPI::ShaderStageFlagsToVkShaderStageFlags(ShaderStageFlags flags)
	{
		return static_cast<VkShaderStageFlags>(flags);
	}
}