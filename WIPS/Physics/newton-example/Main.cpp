#include "StdAfx.h"

// the game
class CGame Game;

int main(int argc, char* argv[])
{
	// Init game
	Game.Init();

	// Create scene
	Game.Prepare();

	// Main Loop
	while(Game.device->run())
	{
		Game.MainLoop();
	}

	// Clean up memory
	Game.Finish();

	return 0;
}
