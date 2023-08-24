#pragma once
#include "Macros.h"
#include "KeyCodes.h"

namespace Orchis {

	class ORCHIS_API Input
	{
	public:
		static bool KeyPressed(Key key);
		static bool KeyReleased(Key key);
		static bool IsKeyDown(Key key);

	private:
		static char m_InputBuffer[96];
		friend class EventDispatcher;
		friend class Application;

		static void RegisterKeyDown(Key key);
		static void RegisterKeyUp(Key key);
		static void UpdateState();
	};
}