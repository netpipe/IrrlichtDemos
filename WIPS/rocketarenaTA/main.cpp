/*

ROCKET ARENA

*/

#include <irrlicht.h>

using namespace irr;

#pragma comment(lib, "Irrlicht.lib")

#include <Init.h>

#include <GameL.h>

#include <Logic.h>


int main() {

	cInit init;
	
	cLogic logic(&init);
	
	IrrlichtDevice *device = init.getDevice();
	
	cGameL gameL(&init);
	

	while (device->run() && gameL.getExit()==false) {
		if (device->isWindowActive()) {

			gameL.drawGame();

		}
	}

	device->drop();
	return 0;    
}
