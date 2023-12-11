/** Example 005 User Interface

This tutorial shows how to use the built in User Interface of
the Irrlicht Engine. It will give a brief overview and show
how to create and use windows, buttons, scroll bars, static
texts, and list boxes.

As always, we include the header files, and use the irrlicht
namespaces. We also store a pointer to the Irrlicht device,
a counter variable for changing the creation position of a window,
and a pointer to a listbox.
*/
#include <irrlicht.h>
//#include "driverChoice.h"
#include "CGUIDynamicGrid.h"

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
	IGUIDynamicGrid * grid;             ///< ADDED
};

// Define some values that we'll use to identify individual GUI controls.
enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NEW_SMALL_WINDOW_BUTTON,     ///< ADDED
	GUI_ID_NEW_LARGE_WINDOW_BUTTON,     ///< ADDED
	GUI_ID_FILE_OPEN_BUTTON,
	GUI_ID_TRANSPARENCY_SCROLL_BAR,
	GUI_ID_SHOW_JOINTS_BUTTON,          ///< ADDED
	GUI_ID_HIDE_JOINTS_BUTTON,          ///< ADDED
	GUI_ID_GRID_RESET,                  ///< ADDED
	GUI_ID_GRID_ATTACH_RUN_ONCE,        ///< ADDED
	GUI_ID_GRID_ATTACH_FOCUSED,         ///< ADDED
	GUI_ID_GRID_RUN,                    ///< ADDED
	GUI_ID_GRID_DETACH_FOCUSED,         ///< ADDED
	GUI_ID_GRID_DETACH_ALL              ///< ADDED
};

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
			'our' scrollbar (the one with id GUI_ID_TRANSPARENCY_SCROLL_BAR), then we change
			the transparency of all gui elements. This is a very
			easy task: There is a skin object, in which all color
			settings are stored. We simply go through all colors
			stored in the skin and change their alpha value.
			*/
			case EGET_SCROLL_BAR_CHANGED:
				if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR)
				{
					s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();

					for (u32 i=0; i<EGDC_COUNT ; ++i)
					{
						SColor col = env->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
						col.setAlpha(pos);
						env->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
					}

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

				case GUI_ID_NEW_SMALL_WINDOW_BUTTON:
					{
                        Context.listbox->addItem(L"Small Window created");
                        Context.counter += 60;
                        if (Context.counter > 500)
                            Context.counter = 0;

                        IGUIWindow* window = env->addWindow(
                            rect<s32>(150 + Context.counter, 100 + Context.counter, 350 + Context.counter, 200 + Context.counter),
                            false, // modal?
                            L"Test window",
                            Context.grid);

                        env->addStaticText(L"Please close me",
                            rect<s32>(35,35,140,50),
                            true, // border?
                            false, // wordwrap?
                            window);
                        EGUI_DYNAMIC_GRID_BEHAVIOR previous_behavior = Context.grid->getBehavior();
                        Context.grid->setBehavior(EGDGB_RESET); ///< Need to stop dynamic grid as it is not possible to detect new child GUI element (Irr 1.8)
                        Context.grid->setBehavior(EGDGB_ATTACH_RUN_ONCE); ///< Try to reattach all
                        Context.grid->setBehavior(previous_behavior); ///< Restore behavior
					}
					return true;

				case GUI_ID_NEW_LARGE_WINDOW_BUTTON:
					{
                        Context.listbox->addItem(L"Large Window created");
                        Context.counter += 60;
                        if (Context.counter > 500)
                            Context.counter = 0;

                        IGUIWindow* window = env->addWindow(
                            rect<s32>(150 + Context.counter, 100 + Context.counter, 550 + Context.counter, 300 + Context.counter),
                            false, // modal?
                            L"Test window",
                            Context.grid);

                        env->addStaticText(L"Please close me",
                            rect<s32>(35,35,140,50),
                            true, // border?
                            false, // wordwrap?
                            window);
                        EGUI_DYNAMIC_GRID_BEHAVIOR previous_behavior = Context.grid->getBehavior();
                        Context.grid->setBehavior(EGDGB_RESET); ///< Need to stop dynamic grid as it is not possible to detect new child GUI element (Irr 1.8)
                        Context.grid->setBehavior(EGDGB_ATTACH_RUN_ONCE); ///< Try to reattach all
                        Context.grid->setBehavior(previous_behavior); ///< Restore behavior
					}
					return true;

				case GUI_ID_FILE_OPEN_BUTTON:
				    {
                        Context.listbox->addItem(L"File open");
                        // There are some options for the file open dialog
                        // We set the title, make it a modal window, and make sure
                        // that the working directory is restored after the dialog
                        // is finished.
                        env->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
                        EGUI_DYNAMIC_GRID_BEHAVIOR previous_behavior = Context.grid->getBehavior();
                        Context.grid->setBehavior(EGDGB_RESET); ///< Need to stop dynamic grid as it is not possible to detect new child GUI element (Irr 1.8)
                        Context.grid->setBehavior(EGDGB_ATTACH_RUN_ONCE); ///< Try to reattach all
                        Context.grid->setBehavior(previous_behavior); ///< Restore behavior
				    }
                    return true;

				case GUI_ID_SHOW_JOINTS_BUTTON:
					Context.grid->setJointVisible(true);
					return true;

				case GUI_ID_HIDE_JOINTS_BUTTON:
					Context.grid->setJointVisible(false);
					return true;

				case GUI_ID_GRID_RESET:
				case GUI_ID_GRID_ATTACH_RUN_ONCE:
				case GUI_ID_GRID_ATTACH_FOCUSED:
				case GUI_ID_GRID_RUN:
				case GUI_ID_GRID_DETACH_FOCUSED:
				case GUI_ID_GRID_DETACH_ALL:
					Context.grid->setBehavior((EGUI_DYNAMIC_GRID_BEHAVIOR)(id-GUI_ID_GRID_RESET));
					return true;


				default:
					return false;
				}
				break;

			case EGET_FILE_SELECTED:
				{
					// show the model filename, selected in the file dialog
					IGUIFileOpenDialog* dialog =
						(IGUIFileOpenDialog*)event.GUIEvent.Caller;
					Context.listbox->addItem(dialog->getFileName());
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
//	// ask user for driver
//	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
//	if (driverType==video::EDT_COUNT)
//		return 1;

	// create device and exit if creation failed

	IrrlichtDevice * device = createDevice(video::EDT_OPENGL , core::dimension2d<u32>(800, 600));

	if (device == 0)
		return 1; // could not create selected driver.

	/* The creation was successful, now we set the event receiver and
		store pointers to the driver and to the gui environment. */

	device->setWindowCaption(L"Irrlicht Engine - User Interface Demo");
	device->setResizable(true);

	video::IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* env = device->getGUIEnvironment();

	/*
	To make the font a little bit nicer, we load an external font
	and set it as the new default font in the skin.
	To keep the standard font for tool tip text, we set it to
	the built-in font.
	*/

	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("./media/fonthaettenschweiler.bmp");
	if (font)
		skin->setFont(font);

	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);

    /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Add a dynamic grid (first GUI root child  = background GUI = last called when event)
    IGUIDynamicGrid * grid =  new CGUIDynamicGrid( env, env->getRootGUIElement() );
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	We add three buttons. The first one closes the engine. The second
	creates a window and the third opens a file open dialog. The third
	parameter is the id of the button, with which we can easily identify
	the button in the event receiver.
	*/

	env->addButton(rect<s32>(10,240,110,240 + 32), 0, GUI_ID_QUIT_BUTTON,
			L"Quit", L"Exits Program");
	env->addButton(rect<s32>(10,280,110,280 + 32), 0, GUI_ID_NEW_SMALL_WINDOW_BUTTON,
			L"New Small Window", L"Launches a new Window");
	env->addButton(rect<s32>(10,320,110,320 + 32), 0, GUI_ID_NEW_LARGE_WINDOW_BUTTON,
			L"New Large Window", L"Launches a new Window");
	env->addButton(rect<s32>(10,360,110,360 + 32), 0, GUI_ID_FILE_OPEN_BUTTON,
			L"File Open", L"Opens a file");
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Add a few controls to control the dynamic grad behavior and appearance
	env->addButton(rect<s32>(10,440,120,460), 0, GUI_ID_SHOW_JOINTS_BUTTON,
			L"Show Joints", L"DYNAMIC GRID DEBUG : Shows the joints");
	env->addButton(rect<s32>(130,440,240,460), 0, GUI_ID_HIDE_JOINTS_BUTTON,
			L"Hide Joints", L"DYNAMIC GRID DEBUG : Hides the joints");
	IGUIStaticText * behavior = env->addStaticText(0, rect<s32>(10,470,240,490), true );
	env->addButton(rect<s32>(10,500,120,520), 0, GUI_ID_GRID_RESET,
			stringw(GUIDynamicGridBehaviorNames[0]).c_str(), L"DYNAMIC GRID BEHAVIOR : Reset");
	env->addButton(rect<s32>(130,500,240,520), 0, GUI_ID_GRID_ATTACH_RUN_ONCE,
			stringw(GUIDynamicGridBehaviorNames[1]).c_str(), L"DYNAMIC GRID BEHAVIOR : Reset");
	env->addButton(rect<s32>(10,530,120,550), 0, GUI_ID_GRID_ATTACH_FOCUSED,
			stringw(GUIDynamicGridBehaviorNames[2]).c_str(), L"DYNAMIC GRID BEHAVIOR : Reset");
	env->addButton(rect<s32>(130,530,240,550), 0, GUI_ID_GRID_RUN,
			stringw(GUIDynamicGridBehaviorNames[3]).c_str(), L"DYNAMIC GRID BEHAVIOR : Reset");
	env->addButton(rect<s32>(10,560,120,580), 0, GUI_ID_GRID_DETACH_FOCUSED,
			stringw(GUIDynamicGridBehaviorNames[4]).c_str(), L"DYNAMIC GRID BEHAVIOR : Reset");
	env->addButton(rect<s32>(130,560,240,580), 0, GUI_ID_GRID_DETACH_ALL,
			stringw(GUIDynamicGridBehaviorNames[5]).c_str(), L"DYNAMIC GRID BEHAVIOR : Reset");
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	Now, we add a static text and a scrollbar, which modifies the
	transparency of all gui elements. We set the maximum value of
	the scrollbar to 255, because that's the maximal value for
	a color value.
	Then we create an other static text and a list box.
	*/

	env->addStaticText(L"Transparent Control:", rect<s32>(150,20,350,40), true);
	IGUIScrollBar* scrollbar = env->addScrollBar(true,
			rect<s32>(150, 45, 350, 60), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
	scrollbar->setMax(255);

	// set scrollbar position to alpha value of an arbitrary element
	scrollbar->setPos(env->getSkin()->getColor(EGDC_WINDOW).getAlpha());

	env->addStaticText(L"Logging ListBox:", rect<s32>(50,110,250,130), true);
	IGUIListBox * listbox = env->addListBox(rect<s32>(50, 140, 250, 210));

	// Store the appropriate data in a context structure.
	SAppContext context;
	context.device = device;
	context.counter = 0;
	context.listbox = listbox;
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Add dynamic grid to context
	context.grid = grid;
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Then create the event receiver, giving it that context structure.
	MyEventReceiver receiver(context);

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(&receiver);

	/*
	And at last, we create a nice Irrlicht Engine logo in the top left corner.
	*/
	env->addImage(driver->getTexture("./media/irrlichtlogo2.png"),
			position2d<int>(10,10));

	/*
	That's all, we only have to draw everything.
	*/

	while(device->run() && driver)
	if (device->isWindowActive())
	{
	    behavior->setText( stringw(GUIDynamicGridBehaviorNames[grid->getBehavior()]).c_str() );
		driver->beginScene(true, true, SColor(0,200,200,200));

		env->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}

/*
**/
