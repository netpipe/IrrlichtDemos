
/*
	Irrlicht Space Invaders contest entry by Gaz Davidson (bitplane)

*/

#include <irrlicht.h>
#include "irrvaders.h"

using namespace irr;
using namespace game;

#pragma comment(lib, "Irrlicht.lib")

int main()
{
	CIrrvaders *iv = new CIrrvaders();

	while(iv->mainLoop())
	{

	}

	iv->drop();

	return 0;
}

