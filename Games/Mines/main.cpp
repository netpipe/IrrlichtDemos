
#include "MineSweeper.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif


int main()
{
	MineSweeper *m = new MineSweeper();

	m->run();

	delete m;

	return 0;
}

