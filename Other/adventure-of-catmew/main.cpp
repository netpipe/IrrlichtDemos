#include "AdvCat.h"

#ifdef WIN32
#include <windows.h>
#endif

#ifdef WIN32
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)
#else
int main(int argc, char* argv[])
#endif
{
	AdvCat game;// = new AdvCat();

	while(game.core->update())
	{	
		game.update();
	}

	//delete game;
	
	return 0;
}
