
//#ifndef WIN32
#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <GL/glfw.h>
//#endif

#include "core/GameEngine.h"

int main(int argc, char **argv)
{
	#ifndef WIN32
	glfwInit();
	if(!glfwOpenWindow(640, 480, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		printf("Could not open window!\n");
		glfwTerminate();
		return 1;
	}

	Horde3D::init();
	#endif

	if (argc == 1)
	{
		printf("Usage: %s path-to-game-data\n", argv[0]);
	}
	else
	{
		// Run engine
		return !peak::GameEngine::get()->run(argv[1]);
	}
}

