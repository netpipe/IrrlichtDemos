#include "tach.h"


// setup tachometer:
gui::IGUIEnvironment* env = device->getGUIEnvironment();
gui::IGUIElement* root = env->getRootGUIElement();
	
u32 sw = driver->getScreenSize().Width;
u32 sh = driver->getScreenSize().Height;
	
core::rect<s32> rect = core::rect<s32>(50, sh-200, 200, sh-50);
//core::rect<s32> rect = Tach::getRect(device, 0.1f, 0.7f, 0.15f);
Tach* tach = new Tach(rect, env, root);


// set reading, valid values from 0.0f to 10.0f"
tach->setReading(...);
		

// clean up:
if (tach) { delete tach; tach = 0; }


	
	

