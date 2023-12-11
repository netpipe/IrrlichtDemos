#include <iostream>
#include <exception>

#include "Game.h"

// General
using std::cerr;
using std::endl;

int main(int argc, char *argv[])
{
	Game *game = new Game();
	
	try
	{
		game->init();
		game->run();
	}
	catch (std::exception e)
	{
		cerr << e.what() << endl;
	}
	
	return 0;
}