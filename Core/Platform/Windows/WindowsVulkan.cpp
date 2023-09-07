#include "OrchisPCH.h"
#include "../Platform/Vulkan/VulkanAPI.h"
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#include "Application.h"

namespace Orchis {

	void VulkanAPI::CreateSurface()
	{
		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = *reinterpret_cast<HWND*>(Application::GetWindow()->GetHandle());
		createInfo.hinstance = GetModuleHandle(NULL);

		VkResult result = vkCreateWin32SurfaceKHR(s_Instance, &createInfo, nullptr, &s_Surface);
		OC_ASSERT(result == VK_SUCCESS);
	}
}