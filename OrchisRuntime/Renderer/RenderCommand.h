#pragma once
#include "Mesh.h"
#include "Scene.h"
namespace Orchis {

	class RenderCommand
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginFrame();
		static void DrawIndexed(const Mesh* mesh);
		static void SetClearColor(float r, float g, float b, float a);
		static void SwapBuffers();

	protected:
		RenderCommand() = default;
		virtual ~RenderCommand() = default;

	private:
		static RenderCommand* s_Instance;
		virtual void DrawIndexedImpl(const Mesh* mesh) = 0;
		virtual void SetClearColorImpl(float r, float g, float b, float a) = 0;
		virtual void BeginFrameImpl() const = 0;
		virtual void SwapBuffersImpl() const = 0;
	};
}