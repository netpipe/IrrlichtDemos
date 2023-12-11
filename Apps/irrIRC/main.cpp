#include "Application.h"

int main()
{
	Application app;

	if (app.init())
		app.run();

	return 0;
}
