#include "RenderCommand.h"
#include "Platform/Vulkan/VulkanRenderCommand.h"
namespace Orchis {
	
	RenderCommand* RenderCommand::s_Instance = nullptr;

	void RenderCommand::Init()
	{
		s_Instance = new VulkanRenderCommand;
	}

	void RenderCommand::ShutDown()
	{
		delete s_Instance;
	}

	void RenderCommand::BeginFrame()
	{
		s_Instance->BeginFrameImpl();
	}

	void RenderCommand::DrawIndexed(const Mesh* mesh)
	{
		s_Instance->DrawIndexedImpl(mesh);
	}

	void RenderCommand::SetClearColor(float r, float g, float b, float a)
	{
		s_Instance->SetClearColorImpl(r, g, b, a);
	}

	void RenderCommand::SwapBuffers()
	{
		s_Instance->SwapBuffersImpl();
	}
	
}
