#include "Application.h"
irr::IrrlichtDevice *device2;
int main()
{
	Application app(device2);

	if (app.init())
		app.run();

	return 0;
}
