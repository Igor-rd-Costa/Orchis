#include "RenderContext.h"
#include "Platform/Vulkan/VulkanRenderContext.h"
#include "Platform/Vulkan/VulkanAPI.h"
#include "Renderer.h"

namespace Orchis {

    RenderContext::RenderContext(void* parentWindow)
    {
        m_Window = Window::Create(parentWindow);
        m_RendererAPI = new VulkanAPI(m_Window->GetHandle());
        Renderer::Init();
    }

    RenderContext::~RenderContext()
    {
        Renderer::Shutdown();
        delete m_RendererAPI;
        delete m_Window;
    }
}
