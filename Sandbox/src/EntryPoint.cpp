#include "Sandbox.h"


int main()
{
	Orchis::Sandbox* app = new Orchis::Sandbox();
	
	app->Run(); 

	delete app;
}