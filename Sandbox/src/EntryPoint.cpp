#include "Application.h"

int main()
{
	Orchis::Application* app = new Orchis::Application();
	app->Run();

	delete app;
}