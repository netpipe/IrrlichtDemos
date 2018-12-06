/*
  Copyright (C) 2011 Daniel Sudmann

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Daniel Sudmann suddani@googlemail.com
*/
#include "CIrrRocketGUI.h"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <irrlicht.h>

#include "IrrRocketInterface.h"
#include "IrrRocketFileInterface.h"
#include "IrrRocketRenderer.h"

#include "CIrrRocketEventReceiver.h"

unsigned int CIrrRocketGUI::RocketContextCount = 0;

CIrrRocketGUI::CIrrRocketGUI(irr::IrrlichtDevice* device) : Device(device)
{
    //ctor
    irr::video::IVideoDriver* driver = device->getVideoDriver();

    Rocket::Core::SetFileInterface(new IrrRocketFileInterface(Device->getFileSystem()));
    Rocket::Core::SetRenderInterface(new IrrRocketRenderer(Device->getVideoDriver()));
    Rocket::Core::SetSystemInterface(new IrrRocketInterface(Device->getTimer()));

    RocketContextCount++;
    if (RocketContextCount == 1)
    {
        Rocket::Core::Initialise();

        /// TODO (sudi#1#): Have to check if the Rocket::Controls are needed
        //Rocket::Controls::Initialise();
    }

    printf("Loading fonts\n");
	// Load the fonts from the path to the sample directory.
	Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Roman.otf");
	Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Bold.otf");
	Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-Italic.otf");
	Rocket::Core::FontDatabase::LoadFontFace("assets/Delicious-BoldItalic.otf");

	RocketContext = Rocket::Core::CreateContext("default", Rocket::Core::Vector2i(Device->getVideoDriver()->getScreenSize().Width, Device->getVideoDriver()->getScreenSize().Height));

	InputEvents = new CIrrRocketEventReceiver(RocketContext);

    GUI_Camera = Device->getSceneManager()->addCameraSceneNode(0, irr::core::vector3df(driver->getScreenSize().Width/2.f, driver->getScreenSize().Height/2.f, 5), irr::core::vector3df(driver->getScreenSize().Width/2.f, driver->getScreenSize().Height/2.f, 0), -1, false);
    GUI_Camera->setUpVector(irr::core::vector3df(0, -1, 0));
    irr::core::matrix4 mat;
	mat.buildProjectionMatrixOrthoLH(driver->getScreenSize().Width, driver->getScreenSize().Height, 0.1,10);
	GUI_Camera->setProjectionMatrix(mat);


	///DEMO
	Device->setEventReceiver(InputEvents);

	Rocket::Core::ElementDocument* cursor = RocketContext->LoadMouseCursor("assets/cursor.rml");
	if (cursor)
		cursor->RemoveReference();

	Rocket::Core::ElementDocument* document = RocketContext->LoadDocument("assets/demo.rml");
	if (document)
	{
		document->Show();
		document->RemoveReference();
	}
}

CIrrRocketGUI::~CIrrRocketGUI()
{
    //dtor
    RocketContextCount--;
    delete InputEvents;
    InputEvents = 0;
}

irr::IEventReceiver* CIrrRocketGUI::getGUIEventReceiver(void)
{
    return InputEvents;
}

void CIrrRocketGUI::run(void)
{
    RocketContext->Update();
    GUI_Camera->render();
    RocketContext->Render();
}
