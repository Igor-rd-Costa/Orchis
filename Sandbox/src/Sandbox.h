#pragma once
#include <Orchis.h>

namespace Orchis {

	class Sandbox : public Application
	{
	public:
		Sandbox();
		virtual void OnUpdate() override;
		~Sandbox();
	private:
		bool m_IsPaused = false;
		Scope<PerspectiveCamera> m_Camera;
		Scene m_Scene;
	};


}