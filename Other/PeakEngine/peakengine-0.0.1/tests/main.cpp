
#include "core/GameEngine.h"

int main(int argc, char **argv)
{
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

