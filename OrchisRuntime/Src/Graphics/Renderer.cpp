#include "Renderer.h"
#include "Core/Macros.h"

namespace Orchis {

	Renderer::~Renderer()
	{
	}

	void Renderer::Init(RenderAPIType type)
	{
		//TODO load backend dll
	}

	void Renderer::Run()
	{
		OC_ASSERT(m_IsRunning == false);
		m_IsRunning = true;
		m_RenderThread = std::thread([this] {
			while (m_IsRunning)
			{

			}
		});
	}

	void Renderer::Shutdown()
	{
		m_IsRunning = false;
		m_RenderThread.join();
	}

}