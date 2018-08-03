#include <irrlicht.h>
#include "Compass.h"

int main()
{

	irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(1024, 768));
	if (device == 0)
		return 1;

	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	irr::video::IVideoDriver* driver = device->getVideoDriver();
	irr::scene::ISceneManager* smgr = device->getSceneManager();

	guienv->addStaticText(L"Press ALT + F4 to exit", irr::core::rect<irr::s32>(20, 20, 200, 40));

	irr::scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();


	//an empty scene is boring
	for (irr::u32 i = 0; i < 10; ++i)
		smgr->addCubeSceneNode(10.0f, 0, -1, irr::core::vector3df(10.0f*i + 10, 0.0f, 50.0));


	// add a compass
	const irr::core::dimension2di compassSize(128, 128);

	irr::core::rect<irr::s32> compassRect;
	compassRect.UpperLeftCorner.X = 880;
	compassRect.UpperLeftCorner.Y = 10;
	compassRect.LowerRightCorner.X = 880 + compassSize.Width;
	compassRect.LowerRightCorner.Y = 10 + compassSize.Height;

	Compass* compass = new Compass(compassRect, guienv, guienv->getRootGUIElement());
	compass->SetCompassBodyTexture(driver->getTexture("compass_body.png"));
	compass->SetCompassNeedleTexture(driver->getTexture("compass_needle.png"));


	while (device->run())
	{
		//update compass
		irr::core::vector3df vec(0.0f, 0.0f, 1.0f);
		camera->getAbsoluteTransformation().rotateVect(vec);
		compass->SetCompassHeading(vec.getHorizontalAngle().Y);

		driver->beginScene(true, true, irr::video::SColor(255, 128, 128, 128));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
	}

	device->drop();
	compass->drop();

	return 0;
}
