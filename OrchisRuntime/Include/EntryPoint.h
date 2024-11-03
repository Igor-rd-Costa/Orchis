#include "../Src/Core/Application.h"

#ifdef OC_WINDOWS
#include <windows.h>
#define OC_ENTRY_POINT(appClass) \
	int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)\
	{\
		appClass app = appClass();\
		OC_ASSERT(dynamic_cast<Orchis::Application*>(&app));\
		app.Run();\
		return 0;\
	}
#else // !OC_WINDOWS
#define OC_ENTRY_POINT(appClass) \
	int main()\
	{\
		appClass app = appClass();\
		OC_ASSERT(Orchis::Application*>(&app));\
		app.run()\
	}
#endif // OC_WINDOWS