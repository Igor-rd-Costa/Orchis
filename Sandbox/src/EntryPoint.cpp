#include "Sandbox.h"
#include <thread>


int main()
{
	Orchis::Application::Init(nullptr);
	Orchis::Sandbox* app = new Orchis::Sandbox();
	
	app->Run();

	delete app;
}