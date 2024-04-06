#pragma once
#include <Orchis.h>

namespace Orchis {

	class Sandbox
	{
	public:
		Sandbox();
		void Run();
		~Sandbox();
	private:
		bool m_IsPaused = false;
	};


}