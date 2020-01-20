// Include the vehement framework
#include "master.hpp"
using namespace vehe;

// Pointer to the vehement factory and its object
dfactory		* vehement;
statemanager	* gamestate;
playermanager	* playerstate;

// Main entry point
int main (int argc, char** argv)
{
	// Create the Vehement Device
	vehement = CreateDevice(800,600,false);
	gamestate = vehement->getStateManager();
	playerstate = vehement->getPlayerManager();

	//Drawing debug infos?
	vehement->setDrawingPhysicsDebug(false);

	// Main Game Loop
	int lastFPS = -1;int fps = 0;
	while(vehement->getIrrDevice()->run() &&
			vehement->getIrrDevice()->getVideoDriver())
	{
		if (vehement->getIrrDevice()->isWindowActive())
		{
			vehement->beginDraw();					//Yeild CPU, Draw 3D
			playerstate->updatePlayerManager();		//Process the player manager
			gamestate->doState();					//Process the game state manager
			vehement->finishDraw();					//Draw GUI, Clean up
		}
		else
			vehement->getIrrDevice()->sleep(1);		//Yeild CPU when unfocused

		
		//Get FPS and change the windows caption
		fps = vehement->getIrrDevice()->getVideoDriver()->getFPS();
		if (lastFPS != fps) 
		{
			core::stringw str = L"Confined [";
			str += vehement->getIrrDevice()->getVideoDriver()->getName();
			str += "] FPS:";
			str += fps;
			vehement->getIrrDevice()->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}

	return 0;
};