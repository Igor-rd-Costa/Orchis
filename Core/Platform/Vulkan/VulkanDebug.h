#pragma once
#if OC_DEBUG_BUILD || OC_RELEASE_BUILD
#include <vulkan/vulkan.h>
#include "../Debug/Logger.h"

namespace Orchis {

	class VulkanDebug
	{
	public:
		VulkanDebug() = delete;
		VulkanDebug(const VulkanDebug&) = delete;
		VulkanDebug(VulkanDebug&&) = delete;
		~VulkanDebug() = delete;

		static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
		{
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = /* VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
				| */ VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
			createInfo.pfnUserCallback = DebugCallback;
			createInfo.pUserData = nullptr;
		}

		static void SetupDebugMessenger(VkInstance instance)
		{
			VkDebugUtilsMessengerCreateInfoEXT createInfo{};
			PopulateDebugMessengerCreateInfo(createInfo);

			auto CreateDebugUtilFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (!CreateDebugUtilFunc)
			{
				m_Logger.Error("Failed to retrieve vkCreateDebugUtilsMessengerEXT function.");
				return;
			}

			VkResult result = CreateDebugUtilFunc(instance, &createInfo, nullptr, &m_Messenger);
			if (result != VK_SUCCESS) m_Logger.Error("Failed to create debug messenger.");
			else m_Logger.Info("Debug messenger created successfully.");
		}

		static void DestroyDebugMessenger(VkInstance instance)
		{
			auto DestroyDebugMessengerFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (!DestroyDebugMessengerFunc)
			{
				m_Logger.Error("Failed to retrieve vkDestroyDebugUtilsMessengerEXT function.");
				return;
			}

			DestroyDebugMessengerFunc(instance, m_Messenger, nullptr);
		}

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageTypes,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData)
		{
			switch (messageSeverity)
			{
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				{
					m_Logger.Trace(pCallbackData->pMessage);
				} break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				{
					m_Logger.Info(pCallbackData->pMessage);
				} break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				{
					m_Logger.Warning(pCallbackData->pMessage);
				} break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				{
					m_Logger.Error(pCallbackData->pMessage);
				} break;
			}
			return VK_FALSE;
		}
	private:
		static VkDebugUtilsMessengerEXT m_Messenger;
		static Logger m_Logger;
	};
	VkDebugUtilsMessengerEXT VulkanDebug::m_Messenger;
	Logger VulkanDebug::m_Logger{"VulkanDebug"};
}
	#define OC_VK_SETUP_DEBUGGER(instance) ::Orchis::VulkanDebug::SetupDebugMessenger(instance)
	#define OC_VK_DESTROY_DEBUGGER(instance) ::Orchis::VulkanDebug::DestroyDebugMessenger(instance)
#else
	#define OC_VK_SETUP_DEBUGGER(instance)
	#define OC_VK_DESTROY_DEBUGGER(instance) 
#endif