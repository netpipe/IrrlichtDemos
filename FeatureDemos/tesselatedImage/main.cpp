// Authors : Airo, freetimecoder


#include <irrlicht.h>
#include "tesselatedImage.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



int main()
{
	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
			false, false, false, 0);

	if (!device)
		return 1;


	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();


	ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS();

camera->setFarValue(4203300.0f);
    //! Constructor.
    /*! @param img       - The path to the image.
        @param startFr   - Vector defining the left(x1)/bottom(y1)/front(z1) position of the cube within the parts are being randomly positioned.
        @param startTo   - Vector defining the right(x2)/top(y2)/behind(z2) position of the cube with x2 > x1, y2 > y1, z2 > z1.
        @param targetPos - The bottom left position of the final compound image.
        @param speed     - Amount of units per seconds a part is moving to its target position.
        @param maxTes    - The maximum number of parts that should be used to split the image. If resolutionX * resolutionY < maxTes then a part represents more than one pixel.
        @param colChange - How quickly a part can correct its color to reach the final color, defined in 'one unit per RGB' per second.
        @param type      - What the parts should be made of, default is using a Billboard.
        @param dim       - The quadratic dimension of the billboard or the radius of the sphere. See parameter type.
        @param gap       - The gap between parts.
    */

TesselatedImage *tesImage = new TesselatedImage(device, "media/test.bmp", vector3df(-600,0,500), vector3df(500, 550, 1000), vector3df(-130,50,100), 45, 1500, 10);
tesImage->startAnimation();

    // In order to do framerate independent movement, we have to know
    // how long it was since the last frame
    u32 then = device->getTimer()->getTime();


   while(device->run())
   {
        // Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
        then = now;

        tesImage->move( frameDeltaTime );

        if (tesImage->hasAnimationFinished())
        {
            tesImage->reset();
            tesImage->startAnimation();
        }

      driver->beginScene(true, true, SColor(255,100,101,140));

      smgr->drawAll();
      guienv->drawAll();

      driver->endScene();
   }


   device->drop();

   return 0;
}
/*
That's it. Compile and run.
**/
