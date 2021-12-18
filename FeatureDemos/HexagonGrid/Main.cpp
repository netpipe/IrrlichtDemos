#include "Main.h"
#include "Hexagon.h"
#include "Grid.h"

#pragma comment(lib, "irrlicht.lib")

int main()
{
	IrrlichtDevice* dvc = createDevice(EDT_OPENGL, dimension2d<u32>(800, 600), 32, false, true, true, 0);
	IVideoDriver* drv = dvc->getVideoDriver();

	dvc->setWindowCaption(L"Hexagon Grid");

	Grid grid;
	grid.create(10, 10, 35); // 7x7 grid - side of hexagon is 35 units. 7 is max, see grid.h for why

	while(dvc->run())
	{
		drv->beginScene(true, true, SColor(255, 0, 0, 0));
		grid.draw(drv);
		drv->endScene();

        dvc->sleep(110,0);
	}

	dvc->drop();
	return 0;
}
