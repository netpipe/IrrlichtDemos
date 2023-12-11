// Copyright (C) 2010 randomMesh
// This file is part of the Sierpinksi demo.
// For conditions of distribution and use, see copyright notice in main.cpp

#include "Demo.h"
#include <irrlicht.h>
#include "CSierpinskiSceneNode.h"

Demo::Demo() :
	device(0), node(0), anim(0), infoText(0)
{

}

Demo::~Demo()
{
	this->node->drop();

	if (this->device)
		this->device->drop();
}

bool Demo::init(int argc, char* argv[])
{
	this->device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(800, 600));
	if (!this->device)
		return false;

	this->device->setEventReceiver(this);
	this->device->setWindowCaption(L"Sierpinski demo");
	this->device->setResizable(false);
	this->device->getCursorControl()->setVisible(false);


	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	const irr::core::dimension2du& screenSize = guienv->getVideoDriver()->getScreenSize();

	//status text
	const irr::core::dimension2du textDimension(500, 100);
	const irr::core::vector2d<irr::u32> textPos((screenSize.Width - textDimension.Width)/2, (screenSize.Height - textDimension.Height));
	const irr::core::rect<irr::s32> textRect(textPos.X, textPos.Y, textPos.X + textDimension.Width, textPos.Y + textDimension.Height);
	infoText = guienv->addStaticText(L"", textRect, true, true, 0, -1, true);




	irr::scene::ISceneManager* const smgr = device->getSceneManager();

	irr::scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition(irr::core::vector3df(0.0f, 0.0f, -1500.0f));
	//camera->setPosition(irr::core::vector3df(0.0f, 1500.0f, 0.0f));
	camera->setFOV(60.0f*irr::core::DEGTORAD);


	const irr::f32 size = 1000.0f; //extent of the tetraheders boundingbox (iteration 0)
	const irr::u32 initialIterations = 4; //be careful, don't use too large values
	const irr::video::SColor topColor(255, 255, 255, 0);
	const irr::video::SColor bottomColor(255, 255, 0, 0);

	this->node = new irr::scene::CSierpinskiSceneNode(
		size, initialIterations, topColor, bottomColor, smgr->getRootSceneNode(), smgr, -1);


	anim = smgr->createRotationAnimator(irr::core::vector3df(0.2f, 0.2f, 0.2f));
	node->addAnimator(anim);
	anim->drop();


	return true;
}

void Demo::run() const
{
	irr::video::IVideoDriver* const driver = this->device->getVideoDriver();
	irr::scene::ISceneManager* const smgr = this->device->getSceneManager();
	irr::gui::IGUIEnvironment* guienv = this->device->getGUIEnvironment();


	//coordinate system
	irr::video::SMaterial mat;
	mat.Lighting = false;

	irr::video::S3DVertex coordinateSystem[6];
	coordinateSystem[0].Pos = irr::core::vector3df(0.0f, -2000.0f, 0.0f);
	coordinateSystem[0].Color = irr::video::SColor(255, 255, 255, 0);
	coordinateSystem[1].Pos = irr::core::vector3df(0.0f, 2000.0f, 0.0f);
	coordinateSystem[1].Color = irr::video::SColor(255, 255, 255, 0);
	coordinateSystem[2].Pos = irr::core::vector3df(0.0f, 0.0f, -2000.0f);
	coordinateSystem[2].Color = irr::video::SColor(255, 0, 255, 0);
	coordinateSystem[3].Pos = irr::core::vector3df(0.0f, 0.0f, 2000.0f);
	coordinateSystem[3].Color = irr::video::SColor(255, 0, 255, 0);
	coordinateSystem[4].Pos = irr::core::vector3df(-2000.0f, 0.0f, 0.0f);
	coordinateSystem[4].Color = irr::video::SColor(255, 255, 0, 0);
	coordinateSystem[5].Pos = irr::core::vector3df(2000.0f, 0.0f, 0.0f);
	coordinateSystem[5].Color = irr::video::SColor(255, 255, 0, 0);

	const irr::u16 indices[6] = { 0, 1 ,2, 3, 4, 5 };


	while (this->device->run())
	{
		if (this->device->isWindowActive())
		{
			driver->beginScene(true, true, irr::video::SColor(255, 128, 128, 128));
			smgr->drawAll();
			guienv->drawAll();

			//draw coordinate system
			driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
			driver->setMaterial(mat);
			driver->drawVertexPrimitiveList(coordinateSystem, 6, indices, 3, irr::video::EVT_STANDARD, irr::scene::EPT_LINES);

			driver->endScene();

			//update info text
			irr::core::stringw text(L"Sierpinski Tetrahedron iteration : ");
			text.append(irr::core::stringw(node->getIteration()));
			text.append(L"\nNumber of tetrahedra : ");
			text.append(irr::core::stringw(node->getNumTetrahedra()));
			text.append(L"\nFps : ");
			text.append(irr::core::stringw(driver->getFPS()));
			text.append(L"\n\n(+ / -) or mouse buttons to change iteration\nF4 Toggle rotation\nF5 Toggle bounding boxes\nF6 Take screenshot\nESC to exit");
			this->infoText->setText(text.c_str());
		}
		else
			this->device->yield();
	}
}

bool Demo::OnEvent(const irr::SEvent& event)
{
	//check keyboard events
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		if (!event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr::KEY_ESCAPE: this->device->closeDevice(); return true;
			case irr::KEY_F6: this->takeScreenshot(); return true;
			case irr::KEY_PLUS: this->node->iterate(true); return true;
			case irr::KEY_MINUS: this->node->iterate(false); return true;
			case irr::KEY_F5: this->node->setDebugDataVisible(this->node->isDebugDataVisible() ^ irr::scene::EDS_BBOX); return true;

			case irr::KEY_F4:
			{
				if (anim)
				{
					node->removeAnimator(anim);
					anim = 0;
				}
				else
				{
					anim = device->getSceneManager()->createRotationAnimator(irr::core::vector3df(0.2f, 0.2f, 0.2f));
					node->addAnimator(anim);
					anim->drop();
				}
			}
			return true;


			default:
				return false;
			}
		}
	}
	else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	{
		if (irr::EMIE_LMOUSE_LEFT_UP == event.MouseInput.Event)
		{
			this->node->iterate(true);
			return true;
		}
		else if (irr::EMIE_RMOUSE_LEFT_UP == event.MouseInput.Event)
		{
			this->node->iterate(false);
			return true;
		}
	}

	return false;
}

void Demo::takeScreenshot() const
{
	irr::video::IVideoDriver* const driver = this->device->getVideoDriver();

	//get image from the last rendered frame
	irr::video::IImage* const image = driver->createScreenShot();
	if (image) //should always be true, but you never know. ;)
	{
		//construct a filename, consisting of local time and file extension
		irr::c8 filename[64];
		snprintf(filename, 64, "screenshot_%u.png", this->device->getTimer()->getRealTime());

		//write screenshot to file
		if (!driver->writeImageToFile(image, filename))
			this->device->getLogger()->log(L"Failed to take screenshot.", irr::ELL_WARNING);

		//Don't forget to drop image since we don't need it anymore.
		image->drop();
	}
}
