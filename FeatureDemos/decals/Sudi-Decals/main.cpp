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
#include "driverChoice.h"

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

#include "CGUIEventDispatcher.h"
class MyEvents
{
    public:
        MyEvents(SAppContext & context)
        {
            Context = context;
            env = Context.device->getGUIEnvironment();
        }

        bool onTransparentChanged(const irr::SEvent::SGUIEvent& event)
        {
            s32 pos = ((IGUIScrollBar*)event.Caller)->getPos();

            for (u32 i=0; i<EGDC_COUNT ; ++i)
            {
                SColor col = env->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
                col.setAlpha(pos);
                env->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
            }
        }

        bool onNewWindow(const irr::SEvent::SGUIEvent& event)
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
            return true;
        }

        bool onQuit(const irr::SEvent::SGUIEvent& event)
        {
            Context.device->closeDevice();
        }

        bool onFileOpenButton(const irr::SEvent::SGUIEvent& event)
        {
            Context.listbox->addItem(L"File open");
            env->addFileOpenDialog(L"Please choose a file.");
            return true;
        }
        IGUIEnvironment* env;
        SAppContext Context;
};

bool onQuit(const irr::SEvent::SGUIEvent& event)
{
    printf("Pressed close button\n");
    return false;
}


/*
Ok, now for the more interesting part. First, create the Irrlicht device. As in
some examples before, we ask the user which driver he wants to use for this
example:
*/


int main_()
{
    // ask user for driver
    video::E_DRIVER_TYPE driverType=driverChoiceConsole();
    if (driverType==video::EDT_COUNT)
        return 1;

    // create device and exit if creation failed

    IrrlichtDevice * device = createDevice(driverType, core::dimension2d<u32>(640, 480));

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
    IGUIFont* font = env->getFont("../../media/fonthaettenschweiler.bmp");
    if (font)
        skin->setFont(font);

    skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);

    /*
    We add three buttons. The first one closes the engine. The second
    creates a window and the third opens a file open dialog. The third
    parameter is the id of the button, with which we can easily identify
    the button in the event receiver.
    */

    irr::gui::IGUIElement* quitButton = env->addButton(rect<s32>(10,240,110,240 + 32), 0, -1,
                                        L"Quit", L"Exits Program");
    irr::gui::IGUIElement* newWindowButton = env->addButton(rect<s32>(10,280,110,280 + 32), 0, -1,
            L"New Window", L"Launches a new Window");
    irr::gui::IGUIElement* fileopenButton = env->addButton(rect<s32>(10,320,110,320 + 32), 0, -1,
                                            L"File Open", L"Opens a file");

    /*
    Now, we add a static text and a scrollbar, which modifies the
    transparency of all gui elements. We set the maximum value of
    the scrollbar to 255, because that's the maximal value for
    a color value.
    Then we create an other static text and a list box.
    */

    env->addStaticText(L"Transparent Control:", rect<s32>(150,20,350,40), true);
    IGUIScrollBar* scrollbar = env->addScrollBar(true,
                               rect<s32>(150, 45, 350, 60), 0, -1);
    scrollbar->setMax(255);

    // set scrollbar position to alpha value of an arbitrary element
    scrollbar->setPos(env->getSkin()->getColor(EGDC_WINDOW).getAlpha());

    env->addStaticText(L"Logging ListBox:", rect<s32>(50,110,250,130), true);
    IGUIListBox * listbox = env->addListBox(rect<s32>(50, 140, 250, 210));
    env->addEditBox(L"Editable Text", rect<s32>(350, 80, 550, 100));

    // Store the appropriate data in a context structure.
    SAppContext context;
    context.device = device;
    context.counter = 0;
    context.listbox = listbox;

    // Then create the event receiver, giving it that context structure.
    MyEvents receiver(context);
    CGUIEventDispatcher dispatcher;

    //you can also bind simple functions
    dispatcher.bind(quitButton, onQuit, irr::gui::EGET_BUTTON_CLICKED);

    //Button clicks
    dispatcher.bind(quitButton, &receiver, &MyEvents::onQuit, irr::gui::EGET_BUTTON_CLICKED);
    dispatcher.bind(newWindowButton, &receiver, &MyEvents::onNewWindow, irr::gui::EGET_BUTTON_CLICKED);
    dispatcher.bind(fileopenButton, &receiver, &MyEvents::onFileOpenButton, irr::gui::EGET_BUTTON_CLICKED);

    //Scrollbar changed
    dispatcher.bind(scrollbar, &receiver, &MyEvents::onTransparentChanged, irr::gui::EGET_SCROLL_BAR_CHANGED);


    // And tell the device to use our custom event receiver.
    device->setEventReceiver(&dispatcher);


    /*
    And at last, we create a nice Irrlicht Engine logo in the top left corner.
    */
    env->addImage(driver->getTexture("../../media/irrlichtlogo2.png"),
                  position2d<int>(10,10));


    /*
    That's all, we only have to draw everything.
    */

    while(device->run() && driver)
        if (device->isWindowActive())
        {
            driver->beginScene(true, true, SColor(0,200,200,200));

            env->drawAll();

            driver->endScene();
        }

    device->drop();

    return 0;
}

/*
**/
