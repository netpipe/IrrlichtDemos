#include <irrlicht.h>
////http://irrlicht.sourceforge.net/forum//viewtopic.php?f=1&t=30750&p=174159&hilit=sdl#p174159

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define SDL
#define SDLVIDEO

#include <SDL/SDL.h>

SDL_Joystick *joystick1 = NULL;

IGUIEnvironment* env;

struct SAppContext
{
	IrrlichtDevice *device;
	s32				counter;
	IGUIListBox*	listbox;
};

// Define some values that we'll use to identify individual GUI controls.
enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NEW_WINDOW_BUTTON,
	GUI_ID_FILE_OPEN_BUTTON,
	GUI_ID_TRANSPARENCY_SCROLL_BAR
};


void setSkinTransparency(s32 alpha, irr::gui::IGUISkin * skin)
{
	for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
	{
		video::SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
		col.setAlpha(alpha);
		skin->setColor((EGUI_DEFAULT_COLOR)i, col);
	}
}


class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver(SAppContext & context) : Context(context) { }

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = Context.device->getGUIEnvironment();

			switch(event.GUIEvent.EventType)
			{

			/*
			If a scrollbar changed its scroll position, and it is
			'our' scrollbar (the one with id GUI_ID_TRANSPARENCY_SCROLL_BAR),
			then we change the transparency of all gui elements. This is an
			easy task: There is a skin object, in which all color
			settings are stored. We simply go through all colors
			stored in the skin and change their alpha value.
			*/
			case EGET_SCROLL_BAR_CHANGED:
				if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR)
				{
					s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					setSkinTransparency(pos, env->getSkin());
				}
				break;

			/*
			If a button was clicked, it could be one of 'our'
			three buttons. If it is the first, we shut down the engine.
			If it is the second, we create a little window with some
			text on it. We also add a string to the list box to log
			what happened. And if it is the third button, we create
			a file open dialog, and add also this as string to the list box.
			That's all for the event receiver.
			*/
			case EGET_BUTTON_CLICKED:
				switch(id)
				{
				case GUI_ID_QUIT_BUTTON:
					Context.device->closeDevice();
					return true;

				case GUI_ID_NEW_WINDOW_BUTTON:
					{
					Context.listbox->addItem(L"Window created");
					Context.counter += 30;
					if (Context.counter > 200)
						Context.counter = 0;

					IGUIWindow* window = env->addWindow(
						rect<s32>(100 + Context.counter, 100 + Context.counter, 300 + Context.counter, 200 + Context.counter),
						false, // modal?
						L"Test window");

					env->addStaticText(L"Please close me",
						rect<s32>(35,35,140,50),
						true, // border?
						false, // wordwrap?
						window);
					}
					return true;

				case GUI_ID_FILE_OPEN_BUTTON:
					Context.listbox->addItem(L"File open");
					// There are some options for the file open dialog
					// We set the title, make it a modal window, and make sure
					// that the working directory is restored after the dialog
					// is finished.
					env->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
					return true;

				default:
					return false;
				}
				break;

			case EGET_FILE_SELECTED:
				{
					// show the event and the selected model filename from the file dialog
					IGUIFileOpenDialog* dialog =
						(IGUIFileOpenDialog*)event.GUIEvent.Caller;
					Context.listbox->addItem(L"EGET_FILE_SELECTED");
					Context.listbox->addItem(dialog->getFileName());
				}
				break;

			case EGET_DIRECTORY_SELECTED:
				{
					// show the event and the selected directory name from the file dialog
					IGUIFileOpenDialog* dialog =
						(IGUIFileOpenDialog*)event.GUIEvent.Caller;
					Context.listbox->addItem(L"EGET_DIRECTORY_SELECTED");
					Context.listbox->addItem(dialog->getDirectoryNameW());
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


int main()
{
SDL_Event event;

	// ask user for driver
//	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
//	if (driverType==video::EDT_COUNT)
//		return 1;

	// create device and exit if creation failed
	IrrlichtDevice * device = createDevice(EDT_OPENGL, core::dimension2d<u32>(640, 480));

	if (device == 0)
		return 1; // could not create selected driver.

	/* The creation was successful, now we set the event receiver and
		store pointers to the driver and to the gui environment. */

	device->setWindowCaption(L"Irrlicht Engine - User Interface Demo");
	device->setResizable(true);
#ifdef SDL
	if(SDL_Init(SDL_INIT_JOYSTICK|SDL_INIT_TIMER) < 0)  return 1;
#ifdef SDLVIDEO
  SDL_Init(SDL_INIT_VIDEO);
#endif
	video::IVideoDriver* driver = device->getVideoDriver();
	env = device->getGUIEnvironment();
     //The images
    SDL_Surface* hello = NULL;
    SDL_Surface* screen = NULL;

    //Start SDL
  //  SDL_Init( SDL_INIT_EVERYTHING );

    //Set up screen
  //  screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );

    //Load image
    hello = SDL_LoadBMP( "sdltest.bmp" );

    //Apply image to screen
   // SDL_BlitSurface( hello, NULL, screen, NULL );

    //Update Screen
   // SDL_Flip( screen );

	SDL_SaveBMP( hello, "your_snapshot_name.bmp" );

       SDL_FreeSurface( hello );


	env->addImage(driver->getTexture("your_snapshot_name.bmp"),
			position2d<int>(10,10));


//{error text
IGUIStaticText* joy_error = env->addStaticText(L"Please insert a joystick or gamepad...",rect<s32>(15,1,260,22), true);
joy_error->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
joy_error->move(position2d<int>(350,350) );
//}

//}

//{open joysticks

//Open joystick1
SDL_JoystickEventState(SDL_ENABLE);
joystick1 = SDL_JoystickOpen(0);


#endif



	const io::path mediaPath = "./media/";//getExampleMediaPath();


	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont(mediaPath + "fonthaettenschweiler.bmp");
	if (font)
		skin->setFont(font);

	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);


	env->addButton(rect<s32>(10,240,110,240 + 32), 0, GUI_ID_QUIT_BUTTON,
			L"Quit", L"Exits Program");
	env->addButton(rect<s32>(10,280,110,280 + 32), 0, GUI_ID_NEW_WINDOW_BUTTON,
			L"New Window", L"Launches a new Window");
	env->addButton(rect<s32>(10,320,110,320 + 32), 0, GUI_ID_FILE_OPEN_BUTTON,
			L"File Open", L"Opens a file");


	env->addStaticText(L"Transparent Control:", rect<s32>(150,20,350,40), true);
	IGUIScrollBar* scrollbar = env->addScrollBar(true,
			rect<s32>(150, 45, 350, 60), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
	scrollbar->setMax(255);
	scrollbar->setPos(255);
	setSkinTransparency( scrollbar->getPos(), env->getSkin());

	// set scrollbar position to alpha value of an arbitrary element
	scrollbar->setPos(env->getSkin()->getColor(EGDC_WINDOW).getAlpha());

	env->addStaticText(L"Logging ListBox:", rect<s32>(10,110,350,130), true);
	IGUIListBox * listbox = env->addListBox(rect<s32>(10, 140, 350, 210));
	env->addEditBox(L"Editable Text", rect<s32>(350, 80, 550, 100));

	// Store the appropriate data in a context structure.
	SAppContext context;
	context.device = device;
	context.counter = 0;
	context.listbox = listbox;

	// Then create the event receiver, giving it that context structure.
	MyEventReceiver receiver(context);

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(&receiver);

	env->addImage(driver->getTexture(mediaPath + "irrlichtlogo2.png"),
			position2d<int>(10,10));



	while(device->run() && driver){

		//{check joysticks
		if ( SDL_NumJoysticks() < 1 ) joy_error->setVisible(true); else joy_error->setVisible(false);
		//}

				if (device->isWindowActive())
			{
				driver->beginScene(video::ECBF_COLOR | video::ECBF_DEPTH, SColor(0,200,200,200));

				env->drawAll();

				driver->endScene();
			}
		//{SDL event receiver
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
					case SDL_KEYDOWN:
					/* handle keyboard stuff here */
					break;

					case SDL_QUIT:
					/* Set whatever flags are necessary to */
					/* end the main game loop here */
					break;

					case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */

					if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) )
					{
//						 if( event.jaxis.axis == 0)
//						 {
//						 menu_item++;
//						 }
//
//						 if( event.jaxis.axis == 1)
//						 {
//						 menu_item--;
//						 }
				   }
				   break;


			}
			}



	}

SDL_JoystickClose(joystick1);
	device->drop();

	return 0;
}

/*
**/
