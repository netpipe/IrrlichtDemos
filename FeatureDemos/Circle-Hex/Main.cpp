#include "Main.h"
#include "Hexagon.h"
#include "Grid.h"
#include "circle.h"
#include "circleclass.h"

using namespace irr;
using namespace scene;

int main()
{

	IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2d<u32>(800, 600), 32, false, true, true, 0);
	IVideoDriver* driver = device->getVideoDriver();
    irr::scene::ISceneManager* smgr = device->getSceneManager();

	device->setWindowCaption(L"Hexagon Grid");

	int t = 3.14;

    smgr->addCameraSceneNodeFPS();

	Grid grid;
	grid.create(7, 7, 35); // 7x7 grid - side of hexagon is 35 units. 7 is max, see grid.h for why


    cCircle circle = createCircle(core::vector3df(0,0,0),10,20,core::vector3df(0,1,0));
    cCircle circle1 = createCircle(core::vector3df(0,0,0),20,20,core::vector3df(1,0,0));
    cCircle circle2 = createCircle(core::vector3df(0,0,0),10,20,core::vector3df(0,0,1));
    cCircle circle3 = createCircle(core::vector3df(0,0,0),20,20,core::vector3df(0,1,0));


circleclass* c = new circleclass(core::vector3df(-50 + (2 * t), 0, 100), 20, 40,
                                core::vector3df(0,1,0), smgr->getRootSceneNode(), smgr);

/*  If you create object on the heap, you should delete it after use, or memory leaks will occur.   --randomMesh
circleclass* circleNode = new circleclass(core::vector3df(50, 0, 100), 20, 40, core::vector3df(0,1,0), smgr->getRootSceneNode(), smgr);
circleNode->drop();
*/

SMaterial NM;
NM.EmissiveColor = SColor(255, 255,0,0);
c->setMaterialFlag(EMF_LIGHTING, false);
c->setMaterial(NM);

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255, 110, 50, 0));
		smgr->drawAll();

				drawCircle(circle, driver);
				drawCircle(circle2, driver);
				drawCircle(circle3, driver);
				drawCircle(circle1, driver);

		grid.draw(driver);

		driver->endScene();
        device->sleep(10,0);
	}
	   device->drop();
	return 0;
}

