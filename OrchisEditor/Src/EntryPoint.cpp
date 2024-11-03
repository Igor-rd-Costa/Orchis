#include "EntryPoint.h"
#include "Editor.h"

//OC_ENTRY_POINT(Orchis::Editor::Editor)


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
 {
     Orchis::Editor::Editor app;
     OC_ASSERT(dynamic_cast<Orchis::Application*>(&app));
     app.Run();
     return 0;
}