#include <iostream>

#include <irrlicht.h>
using namespace irr;

#include "CLayout.h"
#include "MyEventReceiver.h"


int main()
{
	// create device
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2du(800, 600), 32, false, false, true, 0);
    if (!device) return 1;

    // instances
	video::IVideoDriver* driver = device->getVideoDriver();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

    // load gui
	device->setWindowCaption(L"Layout test");

	env->loadGUI("skin.gui", NULL);

	gui::IGUISkin* skin = env->getSkin();

    gui::IGUIFont* font8 = env->getFont("verdana8.fnt");
	if (!font8) font8 = env->getBuiltInFont();

    skin->setFont(font8);


    /*
        Adding the desktop gui elements:
        - We define an empty rectangle for the gui elements. Sizeing and positioning will be done later by the layout.
        - As you can see, unique IDs are specified for all those elements that are binded to layout elements.
        - Binding is simply done by giving the same ID to the gui element and to the layout element (see later).
    */
    core::rect<s32> emptyRect;
    env->addButton(emptyRect, 0, EGI_AlignBorderEnableWindowButton, L"Align & Border & Enable Example");
    env->addButton(emptyRect, 0, EGI_StretchAddRemoveWindowButton, L"Stretch & Add & Remove Example");
    env->addButton(emptyRect, 0, EGI_SaveLoadWindowButton, L"Save & Load Example");
    env->addButton(emptyRect, 0, EGI_QuitButton, L"Quit");

    env->addStaticText(L"Transparent Control:", emptyRect, true, false, 0, EGI_TransparencyScrollbarLabel);

    gui::IGUIScrollBar* scrollbar = env->addScrollBar(true, emptyRect, NULL, EGI_TransparencyScrollbar);
    scrollbar->setMax(255);
    scrollbar->setPos(env->getSkin()->getColor(gui::EGDC_WINDOW).getAlpha());

    env->addStaticText(L"Logging ListBox:", emptyRect, true, false, 0, EGI_LoggingListboxLabel);

    gui::IGUIListBox* listbox = env->addListBox(emptyRect, 0, EGI_LoggingListbox, true);

    gui::IGUIEditBox* editbox = env->addEditBox(L"Editable text", emptyRect, true, 0, EGI_TextEditbox);
    editbox->setAutoScroll(true);
    editbox->setMultiLine(true);
    editbox->setWordWrap(true);
    editbox->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);


    /*
        Let's create the layout for the desktop:
        - Layouts are typically expanding containers you can add elements into.
        - There are two types of container layouts: Horizontal (which grows the X+ direction) and vertical (which grows the Y+ direction)
        - You can add other layouts into existing layouts, making a parent-child hierarchy.
        - There are two special type of layouts: slots and spaces. They cannot have children elements.
        - Slots are usually used for buttons, editboxes, scrollbars, and so on.
        - Spaces are only used for spacing, they cannot be binded to gui elements.
    */
    core::dimension2di  noBorder, border(10, 10), buttonSize(250, 32), labelSize(100, 20),
                        scrollSize(200, 20), logSize(200, 100), textEditSize(250, 100);

    // Create the root layout. All the rest layouts will be added some way into this large one.
    // We don't specify an id because we don't want to bind it to any of the gui elements. (Since this layout is for the desktop)
    // As you can see, it'll grow horizontally, has no border, and will stretch both horizontally and vertically.
    CLayout* rootLT = new CLayout(-1, CLayout::ELD_Horizontal, noBorder, true, true);

    // Let's create an other one, this will contain the buttons. It will expand vertically, it has a border defined (so all it's slots will have).
    // And start adding slots and binding them to the gui elements, simply by giving the same ID.
    CLayout* buttonsLT = new CLayout(-1, CLayout::ELD_Vertical, border, true, true);
    buttonsLT->addSlot(EGI_AlignBorderEnableWindowButton, buttonSize, true);
    buttonsLT->addSlot(EGI_StretchAddRemoveWindowButton, buttonSize, true);
    buttonsLT->addSlot(EGI_SaveLoadWindowButton, buttonSize, true);
    buttonsLT->addSpace(0, true); // this space has no size specified, but will grow as large as it can
    buttonsLT->addSlot(EGI_QuitButton, buttonSize, true);
    // Here we add this "buttons"-layout to the root layout:
    rootLT->addChild(buttonsLT);

    rootLT->addSpace(40); // since the root layout is horizontal, this will be a horizontal space

    // Let's create an other layout, this will contain various elements.
    // When we've finished adding elements, we'll add this layout to the root, just like the previous one.
    CLayout* controlLT = new CLayout(-1, CLayout::ELD_Vertical, border, true, true);
    controlLT->addSlot(EGI_TransparencyScrollbarLabel, labelSize, true);
    controlLT->addSlot(EGI_TransparencyScrollbar, scrollSize, true);
    controlLT->addSpace(40);
    controlLT->addSlot(EGI_LoggingListboxLabel, labelSize, true);
    controlLT->addSlot(EGI_LoggingListbox, logSize, true, true);
    controlLT->addSpace(40);
    controlLT->addSlot(EGI_TextEditbox, textEditSize, true, true);
    rootLT->addChild(controlLT);

    // Don't forget to update the layout, after any kind of changes you've made!
    // We have given the pointer to the IrrlichtDevice to this function, so that all the binded gui elements will update.
    // We also want this layout to be as large as the screen size is.
    rootLT->update(device, true, driver->getScreenSize());

    // Also, Don't forget to delete the layout if you don't need it anymore!
    // Note that as you cannot create gui elements with layouts, you also cannot delete them. Binding is only for updateing them.
    delete rootLT;



	// Store the appropriate data in a context structure.
	SAppContext context;
	context.device = device;
	context.counter = 0;
	context.listbox = listbox;
	context.AlignBorderEnableExampleRoot = NULL;
	context.StretchAddRemoveExampleRoot = NULL;

	MyEventReceiver receiver(context);
	device->setEventReceiver(&receiver);

	/*
	That's all, we only have to draw everything.
	*/

	while (device->run())
	{
        if (device->isWindowActive())
        {
            driver->beginScene(true, true, video::SColor(0, 64, 64, 64));

            env->drawAll();

            driver->endScene();
        }
        else
            device->yield();
	}

	device->drop();

    // Don't forget to delete the layouts if you don't need them anymore:
	if (context.AlignBorderEnableExampleRoot) delete context.AlignBorderEnableExampleRoot;
	if (context.StretchAddRemoveExampleRoot) delete context.StretchAddRemoveExampleRoot;

	return 0;
}


