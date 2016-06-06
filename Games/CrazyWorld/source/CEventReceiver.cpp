#include "CEventReceiver.h"
using namespace std;
CEventReceiver::CEventReceiver(IrrlichtDevice* device, Player* p, AI* g)
{
	this->device = device;
	this->p = p;
	this->g = g;
	cx = device->getVideoDriver()->getScreenSize().Width/2;
	cy = device->getVideoDriver()->getScreenSize().Height/2;
	sideDir = core::vector3df(0,0,0);
	//keys init
	for(int x=0; x<irr::KEY_KEY_CODES_COUNT; x++) keys[x] = false;
}

 bool CEventReceiver::OnEvent(const SEvent &event)
{
	device->getSceneManager()->getActiveCamera()->OnEvent(event);

	if(event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		keys[event.KeyInput.Key] = event.KeyInput.PressedDown;

		if(this->IsKeyDown(irr::KEY_ESCAPE))
		{
			this->device->closeDevice();
		}
		if(this->IsKeyDown(irr::KEY_F1))
		{

		}
	}

	if(event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
	{
		if(!p->isFiring)
			p->isFiring = true;
			//std::cout << "LMD" << endl;
	}else{
		if(event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP)
		{
			if(p->isFiring)
				p->isFiring = false;
				//std::cout << "LMU" << endl;
		}
	}

	//Set cursor back to center
	//if(device->getCursorControl()->getPosition().X != cx || device->getCursorControl()->getPosition().Y != cy)
		//device->getCursorControl()->setPosition(cx,cy);

	return true;
}

bool CEventReceiver::IsKeyDown(int index)
{
	return keys[index];
}

