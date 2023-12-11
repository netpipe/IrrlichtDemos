#include "core.h"

int main(int argc, char * argv[])
{
	Core core;
	
	core.begin("Irrlicht");
	
	while(core.run())
	{
		if(core.active()) {
			core.update();
			
			core.render();
		}
		else {
			core.yield();
		}
	}
	
	core.end();
	
	return 0;
};
