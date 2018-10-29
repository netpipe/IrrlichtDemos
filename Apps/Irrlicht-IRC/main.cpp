#include "Application.h"

int main(int argc, char** argv)
{
	g_app = new Application();

	if (g_app->init()) {
		g_app->run();
	}
		

	return 0;
}
