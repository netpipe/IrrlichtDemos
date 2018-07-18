/*
  Copyright (C) 2016 Benjamín Expósito Jaramillo

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
*/

#include <irrlicht.h>
#include "SpheremapGenerator.h"
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

// Declare a structure to hold some context for the event receiver so that it
// has it available inside its OnEvent() method.
struct SAppContext
{
	IrrlichtDevice *device;
	s32				counter;
	IGUIListBox*	listbox;
};

// Define some values that we'll use to identify individual GUI controls.
enum
{
	GUI_ID_GENERATE_MAP = 101,
	GUI_ID_FILE_UP,
    GUI_ID_FILE_DOWN,
	GUI_ID_FILE_LEFT,
	GUI_ID_FILE_RIGHT,
	GUI_ID_FILE_FRONT,
	GUI_ID_FILE_BACK,
	GUI_ID_X_RES,
	GUI_ID_Y_RES
};

/*
	Set the skin transparency by changing the alpha values of all skin-colors
*/
void setSkinTransparency(s32 alpha, irr::gui::IGUISkin * skin)
{
	for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
	{
		video::SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
		col.setAlpha(alpha);
		skin->setColor((EGUI_DEFAULT_COLOR)i, col);
	}
}

/*
The Event Receiver is not only capable of getting keyboard and
mouse input events, but also events of the graphical user interface
(gui). There are events for almost everything: Button click,
Listbox selection change, events that say that a element was hovered
and so on. To be able to react to some of these events, we create
an event receiver.
We only react to gui events, and if it's such an event, we get the
id of the caller (the gui element which caused the event) and get
the pointer to the gui environment.
*/
class MyEventReceiver : public IEventReceiver
{
public:
    IGUIEditBox * xres;
    IGUIEditBox * yres;
    IGUIButton * bup;
    IGUIButton * bdown;
	IGUIButton * bleft;
	IGUIButton * bright;
	IGUIButton * bfrontv;
	IGUIButton * bbackv;
    ITexture * up;
    ITexture * down;
	ITexture * left;
	ITexture * right;
	ITexture * frontv;
	ITexture * backv;
	IGUIImage * spheremap;
	SpheremapGenerator * spheregen;
	u32 selectedbutton;
	irr::core::array< irr::video::ITexture* > cubemap;
	irr::core::dimension2du reso;
	MyEventReceiver(SAppContext & context) : Context(context)
	{
        reso = irr::core::dimension2du(512,512);
        spheregen = new SpheremapGenerator( context.device->getSceneManager(), reso);
        selectedbutton = 0;

        up = Context.device->getVideoDriver()->getTexture("./data/noimage.png");
        down = up;
        left = up;
        right = up;
        frontv = up;
        backv = up;

        xres = Context.device->getGUIEnvironment()->addEditBox(L"512", rect<s32>(510, 60, 510 + 100, 90 ), false, 0, GUI_ID_X_RES);
        yres = Context.device->getGUIEnvironment()->addEditBox(L"512", rect<s32>(635, 60, 635 + 100, 90 ), false, 0, GUI_ID_Y_RES);

        bup = Context.device->getGUIEnvironment()->addButton(rect<s32>( 40, 80, 140, 180), 0, GUI_ID_FILE_UP, L"Up", L"Opens a file");
        bup->setImage(up);
        bup->setScaleImage(true);

        bdown = Context.device->getGUIEnvironment()->addButton(rect<s32>( 40, 280, 140, 380) , 0, GUI_ID_FILE_DOWN, L"Down", L"Opens a file");
        bdown->setImage(up);
        bdown->setScaleImage(true);

        bleft = Context.device->getGUIEnvironment()->addButton(rect<s32>( 40, 180, 140, 180 + 100) , 0, GUI_ID_FILE_LEFT, L"Left", L"Opens a file");
        bleft->setImage(up);
        bleft->setScaleImage(true);

        bright = Context.device->getGUIEnvironment()->addButton(rect<s32>( 240, 180, 240 + 100, 180 + 100) , 0, GUI_ID_FILE_RIGHT, L"Right", L"Opens a file");
        bright->setImage(up);
        bright->setScaleImage(true);

        bfrontv = Context.device->getGUIEnvironment()->addButton(rect<s32>( 140, 180, 140 + 100, 180 + 100) , 0, GUI_ID_FILE_FRONT, L"Front", L"Opens a file");
        bfrontv->setImage(up);
        bfrontv->setScaleImage(true);

        bbackv = Context.device->getGUIEnvironment()->addButton(rect<s32>( 340, 180, 340 + 100, 180 + 100) , 0, GUI_ID_FILE_BACK, L"Back", L"Opens a file");
        bbackv->setImage(up);
        bbackv->setScaleImage(true);

        spheremap = Context.device->getGUIEnvironment()->addImage(rect<s32>( 475, 100, 775, 350 ));
        spheremap->setImage(up);
        spheremap->setScaleImage(true);
	}

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = Context.device->getGUIEnvironment();

			switch(event.GUIEvent.EventType)
			{

            case EGET_EDITBOX_CHANGED:
                Context.listbox->addItem(L"Resolution changed.");
                switch(id)
				{
                case GUI_ID_X_RES:
                    reso.Width = atoi(stringc(xres->getText()).c_str());
                break;
                case GUI_ID_Y_RES:
                    reso.Height = atoi(stringc(yres->getText()).c_str());
                break;
                default:
                break;
				}
            break;
            case EGET_EDITBOX_ENTER:
                Context.listbox->addItem(L"Resolution changed.");
                switch(id)
				{
                case GUI_ID_X_RES:
                    reso.Width = atoi(stringc(xres->getText()).c_str());
                break;
                case GUI_ID_Y_RES:
                    reso.Height = atoi(stringc(yres->getText()).c_str());
                break;
                default:
                break;
				}
            break;
			case EGET_BUTTON_CLICKED:
				switch(id)
				{

				case GUI_ID_FILE_UP:
				    selectedbutton = 0;
					Context.listbox->addItem(L"Selecting up view image..");
					env->addFileOpenDialog(L"Please choose a file.", true, 0);
					return true;

				case GUI_ID_FILE_DOWN:
				    selectedbutton = 1;
					Context.listbox->addItem(L"Selecting down view image..");
					env->addFileOpenDialog(L"Please choose a file.", true, 0);
					return true;

				case GUI_ID_FILE_LEFT:
				    selectedbutton = 2;
					Context.listbox->addItem(L"Selecting left view image..");
					env->addFileOpenDialog(L"Please choose a file.", true, 0);
					return true;

				case GUI_ID_FILE_RIGHT:
				    selectedbutton = 3;
					Context.listbox->addItem(L"Selecting right view image..");
					env->addFileOpenDialog(L"Please choose a file.", true, 0);
					return true;

				case GUI_ID_FILE_FRONT:
				    selectedbutton = 4;
					Context.listbox->addItem(L"Selecting front view image..");
					env->addFileOpenDialog(L"Please choose a file.", true, 0);
					return true;

				case GUI_ID_FILE_BACK:
				    selectedbutton = 5;
					Context.listbox->addItem(L"Selecting back view image..");
					env->addFileOpenDialog(L"Please choose a file.", true, 0);
					return true;

				case GUI_ID_GENERATE_MAP:
                    cubemap.push_back( left );
                    cubemap.push_back( right );
                    cubemap.push_back( down );
                    cubemap.push_back( up );
                    cubemap.push_back( frontv );
                    cubemap.push_back( backv );

				    selectedbutton = 100;
					Context.listbox->addItem(L"Generating...");
                    irr::c8 filename[64];
                    snprintf(filename, 64, "./skydome_%u.png", Context.device->getTimer()->getRealTime());

                    spheregen->setSkyDomeResolution(reso);
                    spheregen->cubemap2skydome( cubemap , filename );

				    cubemap.erase(0);
				    cubemap.erase(0);
				    cubemap.erase(0);
				    cubemap.erase(0);
				    cubemap.erase(0);
				    cubemap.erase(0);
                    spheremap->setImage( Context.device->getVideoDriver()->getTexture(filename) );
					return true;

				default:
					return false;
				}
				break;

                case EGET_FILE_SELECTED:
				{
					// show the model filename, selected in the file
					IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
                    Context.listbox->addItem( dialog->getFileName() );

					switch(selectedbutton)
					{
                        case 0:
                            up = Context.device->getVideoDriver()->getTexture( dialog->getFileName() );
                            bup->setImage( up );
                        break;
                        case 1:
                            down = Context.device->getVideoDriver()->getTexture(dialog->getFileName());
                            bdown->setImage( down );
                        break;
                        case 2:
                            left = Context.device->getVideoDriver()->getTexture(dialog->getFileName());
                            bleft->setImage( left );
                        break;
                        case 3:
                            right = Context.device->getVideoDriver()->getTexture(dialog->getFileName());
                            bright->setImage( right );
                        break;
                        case 4:
                            frontv = Context.device->getVideoDriver()->getTexture(dialog->getFileName());
                            bfrontv->setImage( frontv );
                        break;
                        case 5:
                            backv = Context.device->getVideoDriver()->getTexture(dialog->getFileName());
                            bbackv->setImage( backv );
                        break;
                        default:
                        break;
					}
				}
				break;

			default:
				break;
			}
		}

		return false;
	}

private:
	SAppContext & Context;
};


/*
Ok, now for the more interesting part. First, create the Irrlicht device. As in
some examples before, we ask the user which driver he wants to use for this
example:
*/
int main()
{
	// create device and exit if creation failed
	IrrlichtDevice * device = createDevice(EDT_OPENGL, core::dimension2d<u32>(800, 600));

	if (device == 0)
		return 1; // could not create selected driver.

	device->setWindowCaption(L"Irrlicht Engine - CubeToSphere");
	device->setResizable(true);

	video::IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* env = device->getGUIEnvironment();

	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("./data/fontcourier.bmp");
	if (font)
		skin->setFont(font);

	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);

	env->addStaticText(L"Log:", rect<s32>(20, 400, 780, 420), true);
	IGUIListBox * listbox = env->addListBox(rect<s32>(20, 421, 780, 550));

    for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
    {
        video::SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
        col.setAlpha(255);
        skin->setColor((EGUI_DEFAULT_COLOR)i, col);
    }
	// Store the appropriate data in a context structure.
	SAppContext context;
	context.device = device;
	context.counter = 0;
	context.listbox = listbox;

	// Then create the event receiver, giving it that context structure.
	MyEventReceiver receiver(context);
	device->setEventReceiver(&receiver);

    env->addButton( rect<s32>( 575, 360, 575 + 100, 360 + 32) , 0, GUI_ID_GENERATE_MAP, L"Generate", L"Generate sphere map");

	env->addImage(driver->getTexture("./data/irrlichtlogo3.png"), position2d<int>(10,10));
	env->addStaticText(L"X", rect<s32>(618, 70, 635, 90), false);

	while(device->run() && driver)
	if (device->isWindowActive())
	{
		driver->beginScene(true, true, SColor(255,65,65,70));

		env->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}

/*
**/
