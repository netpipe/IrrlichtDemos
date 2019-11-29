#include <irrlicht.h>
	using namespace irr;

#include "CStateSys.h"
#include "menu.h"

int main(void)
{
	// Create the device.
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
		core::dimension2d<s32>(640, 480));
	
	if(device == NULL)
		return EXIT_FAILURE; // Could not create selected driver.
	
	device->setWindowCaption(L"CStateSys Example");
	
	video::IVideoDriver* driver = device->getVideoDriver();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();
	scene::ISceneManager* scene = device->getSceneManager();
	
	CStateManager stateSys(device);
	CMenuState* menuState = new CMenuState(&stateSys);
	stateSys.setState(menuState);
	
	while(device->run() == true && driver != NULL)
	{
		stateSys.run();
		
		driver->beginScene(true, true, stateSys.getColor());
		
		if(stateSys.getRenderScene() == true)
			scene->drawAll();
		
		if(stateSys.getRenderGUI() == true)
			env->drawAll();
		
		driver->endScene();
	}
	
	device->drop();
	
	return EXIT_SUCCESS;
}
