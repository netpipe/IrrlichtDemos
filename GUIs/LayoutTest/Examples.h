#ifndef EXAMPLES_H_INCLUDED
#define EXAMPLES_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "CLayout.h"


struct SAppContext
{
	IrrlichtDevice* device;
	s32 counter;
	gui::IGUIListBox* listbox;

	CLayout* AlignBorderEnableExampleRoot;
	CLayout* StretchAddRemoveExampleRoot;
};

// define unique IDs for the gui elements/layouts
enum EGUIID
{
	EGI_QuitButton = 101,
	EGI_AlignBorderEnableWindowButton,
	EGI_StretchAddRemoveWindowButton,
	EGI_SaveLoadWindowButton,

	EGI_TransparencyScrollbarLabel,
	EGI_TransparencyScrollbar,
	EGI_LoggingListboxLabel,
	EGI_LoggingListbox,
	EGI_TextEditbox,

	EGI_AlignBorderEnableWindow = 200,
	EGI_ABEW_SmallBorderButton,
	EGI_ABEW_MediumBorderButton,
	EGI_ABEW_LargeBorderButton,
    EGI_ABEW_EnableButton,
    EGI_ABEW_DisableButton,
    EGI_ABEW_AlignsLayout,
	EGI_ABEW_WestLabel,
	EGI_ABEW_EastLabel,
	EGI_ABEW_NorthLabel,
	EGI_ABEW_SouthLabel,
	EGI_ABEW_HAlignLayout,
	EGI_ABEW_VAlignLayout,
	EGI_ABEW_ToggleHAlignButton,
	EGI_ABEW_ToggleVAlignButton,

    EGI_StretchAddRemoveWindow = 300,
    EGI_SARW_StretchHButton,
    EGI_SARW_StretchVButton,
    EGI_SARW_StretchElementLabel,
    EGI_SARW_AddButton,
    EGI_SARW_RemoveButton,
    EGI_SARW_AddRemoveElementsLayout,
    EGI_SARW_AddRemoveElementsStart = 320,
    EGI_SARW_AddRemoveElementsEnd = 340,

	EGI_SaveLoadWindow = 400,
	EGI_SLW_LayoutA,
	EGI_SLW_LayoutB,
	EGI_SLW_LayoutC,
	EGI_SLW_LayoutD,
	EGI_SLW_LayoutE,
	EGI_SLW_LayoutBCD,
	EGI_SLW_Button0,
	EGI_SLW_Button1,
	EGI_SLW_Button2,
	EGI_SLW_Button3,
	EGI_SLW_Button4,
	EGI_SLW_Button5,
	EGI_SLW_Button6,
	EGI_SLW_Button7,
	EGI_SLW_Button8,
	EGI_SLW_Button9,
	EGI_SLW_Button10,
	EGI_SLW_Button11,
	EGI_SLW_Button12,
	EGI_SLW_AfterUpdate
};


void addAlignBorderEnableExampleWindow(SAppContext &Context)
{
    gui::IGUIEnvironment* env = Context.device->getGUIEnvironment();
    gui::IGUIElement* rootGUI = env->getRootGUIElement();
    gui::IGUIElement* window = rootGUI->getElementFromId(EGI_AlignBorderEnableWindow, true);
    CLayout* &rootLT = Context.AlignBorderEnableExampleRoot;

    // if the layout doesn't exist, create it
    if (!rootLT)
    {
        // we define some default sizes:
        core::dimension2di noBorder, border(10, 10), buttonSize(100, 25);

        // create a horizontal layout for our "HAlign toggle" button:
        CLayout* hAlignLT = new CLayout(EGI_ABEW_HAlignLayout, CLayout::ELD_Vertical, border, true, true);
        hAlignLT->addSlot(EGI_ABEW_ToggleHAlignButton, buttonSize, false, true, CLayout::ELA_UpperLeft);

        // create a vertical layout for our "VAlign toggle" button:
        CLayout* vAlignLT = new CLayout(EGI_ABEW_VAlignLayout, CLayout::ELD_Horizontal, border, true, true);
        vAlignLT->addSlot(EGI_ABEW_ToggleVAlignButton, buttonSize, true, false, CLayout::ELA_UpperLeft);

        // create another layout that will contain the previous two, plus 3 more buttons:
        CLayout* alignsLT = new CLayout(EGI_ABEW_AlignsLayout, CLayout::ELD_Vertical, noBorder, true, true);
        alignsLT->addSlot(EGI_ABEW_SmallBorderButton, buttonSize, true);
        alignsLT->addSlot(EGI_ABEW_MediumBorderButton, buttonSize, true);
        alignsLT->addSlot(EGI_ABEW_LargeBorderButton, buttonSize, true);
        alignsLT->addChild(hAlignLT);
        alignsLT->addChild(vAlignLT);

        // yet another one, add the previous container, and add other slots:
        CLayout* collectLT = new CLayout(-1, CLayout::ELD_Horizontal, noBorder, true, true);
        collectLT->addSlot(EGI_ABEW_WestLabel, core::dimension2di(20, 300), false, true);
        collectLT->addSlot(EGI_ABEW_EnableButton, core::dimension2di(), true, true);
        collectLT->addChild(alignsLT);
        collectLT->addSlot(EGI_ABEW_DisableButton, core::dimension2di(), true, true);
        collectLT->addSlot(EGI_ABEW_EastLabel, core::dimension2di(20, 300), false, true);

        // here's the root layout that will contain all the layouts above:
        rootLT = new CLayout(EGI_AlignBorderEnableWindow, CLayout::ELD_Vertical, noBorder, true, true);
        rootLT->addSpace(20); // note that I've added 20 pixel spacing because of the window title!
        rootLT->addSlot(EGI_ABEW_NorthLabel, core::dimension2di(400, 20), true);
        rootLT->addChild(collectLT);
        rootLT->addSlot(EGI_ABEW_SouthLabel, core::dimension2di(400, 20), true);
    }

    // if the window don't exist, create it
    if (!window)
    {
        // define an empty rectangle, since the positioning and spacing will done later by the layout:
        core::rect<s32> emptyRect;

        // create the window itself. This will be the parent of the other gui elements.
        window = env->addWindow(emptyRect, false, L"Align & Border & Enable Example", rootGUI, EGI_AlignBorderEnableWindow);

        // start adding gui elements with the same IDs we've used when creating the layouts (binding!) :
        env->addButton(emptyRect, window, EGI_ABEW_EnableButton, L"Enable");
        env->addButton(emptyRect, window, EGI_ABEW_DisableButton, L"Disable");

        env->addStaticText(L"W e s t ", emptyRect, true, true, window, EGI_ABEW_WestLabel);
        env->addStaticText(L"E a s t ", emptyRect, true, true, window, EGI_ABEW_EastLabel);
        env->addStaticText(L"North", emptyRect, true, true, window, EGI_ABEW_NorthLabel);
        env->addStaticText(L"South", emptyRect, true, true, window, EGI_ABEW_SouthLabel);

        env->addStaticText(L"", emptyRect, true, true, window, EGI_ABEW_HAlignLayout);
        env->addStaticText(L"", emptyRect, true, true, window, EGI_ABEW_VAlignLayout);

        env->addButton(emptyRect, window, EGI_ABEW_ToggleHAlignButton, L"HAlign toggle");
        env->addButton(emptyRect, window, EGI_ABEW_ToggleVAlignButton, L"VAlign toggle");
        env->addButton(emptyRect, window, EGI_ABEW_SmallBorderButton, L"Small border");
        env->addButton(emptyRect, window, EGI_ABEW_MediumBorderButton, L"Medium border");
        env->addButton(emptyRect, window, EGI_ABEW_LargeBorderButton, L"Large border");

        // don't forget to update the layout (and because we have given a pointer to the IrrlichtDevice, all the gui elements will update)
        rootLT->update(Context.device, true);

        // after updating, we know the size of the window, so we can put it to the center of the screen:
        core::dimension2du screenSize = Context.device->getVideoDriver()->getScreenSize();
        core::rect<s32> windowRect = rootLT->calcAbsRect();
        window->setRelativePosition(core::position2di((screenSize.Width / 2) - (windowRect.getWidth() / 2),
                                                      (screenSize.Height / 2) - (windowRect.getHeight() / 2)));

        Context.listbox->addItem(L"Align & Border & Enable Example Window created");
        Context.counter += 30;
        if (Context.counter > 200) Context.counter = 0;
    }

    // since we'll need the pointer to the layout, we'll delete it later (see the end of main.cpp)!
}


void addStretchAddRemoveExampleWindow(SAppContext &Context)
{
    gui::IGUIEnvironment* env = Context.device->getGUIEnvironment();
    gui::IGUIElement* rootGUI = env->getRootGUIElement();
    gui::IGUIElement* window = rootGUI->getElementFromId(EGI_StretchAddRemoveWindow, true);
    CLayout* &rootLT = Context.StretchAddRemoveExampleRoot;

    // if the layout doesn't exist, create it
    if (!rootLT)
    {
        // define default sizes:
        core::dimension2di noBorder, border(5, 5), buttonSize(100, 30), buttonSize2(150, 50), stretchElementSize(150, 20);

        // here we create all the layouts:
        rootLT = new CLayout(EGI_StretchAddRemoveWindow, CLayout::ELD_Vertical, noBorder, true, true);
        CLayout* collectLT = new CLayout(-1, CLayout::ELD_Horizontal, noBorder, true, true);
        CLayout* stretchLT = new CLayout(-1, CLayout::ELD_Vertical, noBorder, true, true);
        CLayout* stretchButtonsLT = new CLayout(-1, CLayout::ELD_Horizontal, border, true);
        CLayout* stretchElementLT = new CLayout(-1, CLayout::ELD_Vertical, border, true, true);
        CLayout* addRemoveLT = new CLayout(-1, CLayout::ELD_Vertical, noBorder, true, true);
        CLayout* addRemoveButtonsLT = new CLayout(-1, CLayout::ELD_Vertical, border, true);
        CLayout* addRemoveElementsLT = new CLayout(EGI_SARW_AddRemoveElementsLayout, CLayout::ELD_Vertical, border, true, true);

        // start adding elements into the layouts:
        stretchButtonsLT->addSlot(EGI_SARW_StretchHButton, buttonSize, true);
        stretchButtonsLT->addSlot(EGI_SARW_StretchVButton, buttonSize, true);
        stretchElementLT->addSlot(EGI_SARW_StretchElementLabel, stretchElementSize, false, false, CLayout::ELA_Center);
        stretchLT->addChild(stretchButtonsLT);
        stretchLT->addChild(stretchElementLT);

        addRemoveButtonsLT->addSlot(EGI_SARW_AddButton, buttonSize2, true);
        addRemoveButtonsLT->addSlot(EGI_SARW_RemoveButton, buttonSize2, true);
        addRemoveLT->addChild(addRemoveButtonsLT);
        addRemoveLT->addChild(addRemoveElementsLT);

        collectLT->addChild(stretchLT);
        collectLT->addSpace(20);
        collectLT->addChild(addRemoveLT);

        rootLT->addSpace(20); // note that I've added 20 pixel spacing because of the window title!
        rootLT->addChild(collectLT);
    }

    // if the window don't exist, create it
    if (!window)
    {
        core::rect<s32> emptyRect; // we don't know the exact position and size yet

        // create the window, parent of all the other gui elements:
        window = env->addWindow(emptyRect, false, L"Stretch & Add & Remove Example", rootGUI, EGI_StretchAddRemoveWindow);

        // adding gui elements to the window:
        env->addButton(emptyRect, window, EGI_SARW_StretchHButton, L"StretchH");
        env->addButton(emptyRect, window, EGI_SARW_StretchVButton, L"StretchV");
        gui::IGUIStaticText* stretchElement = env->addStaticText(L"Stretching Element", emptyRect, true, true, window, EGI_SARW_StretchElementLabel, true);
        stretchElement->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);
        env->addButton(emptyRect, window, EGI_SARW_AddButton, L"Add");
        env->addButton(emptyRect, window, EGI_SARW_RemoveButton, L"Remove");

        // don't forget to update the layout:
        rootLT->update(Context.device, true);

        // put the window to the center of the screen:
        core::dimension2du screenSize = Context.device->getVideoDriver()->getScreenSize();
        core::rect<s32> windowRect = rootLT->calcAbsRect();
        window->setRelativePosition(core::position2di((screenSize.Width / 2) - (windowRect.getWidth() / 2),
                                                      (screenSize.Height / 2) - (windowRect.getHeight() / 2)));

        Context.listbox->addItem(L"Stretch & Add & Remove Example Window created");
        Context.counter += 30;
        if (Context.counter > 200) Context.counter = 0;
    }

    // since we'll need the pointer to the layout, we'll delete it later (see the end of main.cpp)!
}


void addSaveLoadExampleWindow(SAppContext &Context)
{
    gui::IGUIEnvironment* env = Context.device->getGUIEnvironment();
    gui::IGUIElement* rootGUI = env->getRootGUIElement();
    gui::IGUIElement* window = rootGUI->getElementFromId(EGI_SaveLoadWindow, true);

    // this is actually my own test. It might be a little bit confusing... the most important things are commented, see below:
    // let's create the layout:
    core::dimension2di border(5, 5), border2(10, 10);

    CLayout *rootLT = new CLayout(EGI_SaveLoadWindow, CLayout::ELD_Vertical);
    CLayout *layoutA = new CLayout(EGI_SLW_LayoutA, CLayout::ELD_Horizontal, border, true, true);
    CLayout *layoutB = new CLayout(EGI_SLW_LayoutB, CLayout::ELD_Vertical, border, true, true);
    CLayout *layoutC = new CLayout(EGI_SLW_LayoutC, CLayout::ELD_Vertical, border2, true, true);
    CLayout *layoutD = new CLayout(EGI_SLW_LayoutD, CLayout::ELD_Vertical, core::dimension2di(), true, true);
    CLayout *layoutBCD = new CLayout(EGI_SLW_LayoutBCD, CLayout::ELD_Horizontal);
    CLayout *layoutE = new CLayout(EGI_SLW_LayoutE, CLayout::ELD_Horizontal, core::dimension2di(), true, true);
    CLayout *layoutABCDE = new CLayout(-1, CLayout::ELD_Vertical);

    layoutA->addSpace(20);
    layoutA->addSlot(EGI_SLW_Button0, core::dimension2di(120, 20), true, true);
    layoutA->changeBorder(border);

    layoutB->addSpace(20);
    layoutB->addSlot(EGI_SLW_Button3, core::dimension2di(150, 30), true);
    layoutB->addSlot(EGI_SLW_Button4, core::dimension2di(170, 40), true);

    layoutC->addSpace(20);
    layoutC->addSlot(EGI_SLW_Button5, core::dimension2di(150, 50));
    layoutC->addSlot(EGI_SLW_Button6, core::dimension2di(130, 50));

    layoutD->addSpace(20);
    layoutD->addSlot(EGI_SLW_Button7, core::dimension2di(160, 20), false, true, CLayout::ELA_LowerRight);

    layoutE->addSpace(20);
    layoutE->addSlot(EGI_SLW_Button10, core::dimension2di(130, 70));
    layoutE->addSlot(EGI_SLW_Button11, core::dimension2di(160, 20));
    layoutE->addSlot(EGI_SLW_Button12, core::dimension2di(150, 50));

    layoutBCD->addSpace(40);
    layoutBCD->addChild(layoutB);
    layoutBCD->addSpace(40);
    layoutBCD->addChild(layoutC);
    layoutBCD->addChild(layoutD);

    rootLT->addSpace(20);
    layoutABCDE->addChild(layoutA);
    layoutABCDE->addChild(layoutBCD);
    layoutABCDE->addChild(layoutE);
    rootLT->addChild(layoutABCDE);

    layoutA->addSlot(EGI_SLW_Button1, core::dimension2di(120, 40), true, true);
    layoutA->addSlot(EGI_SLW_Button2, core::dimension2di(120, 10), true, true);

    layoutD->addSlot(EGI_SLW_Button8, core::dimension2di(140, 50), false, true, CLayout::ELA_LowerRight);
    layoutD->addSlot(EGI_SLW_Button9, core::dimension2di(130, 50), false, false, CLayout::ELA_LowerRight);
    layoutD->changeBorder(border);

    rootLT->update(NULL, true);

    layoutC->addSlot(EGI_SLW_AfterUpdate, core::dimension2di(140, 150));
    layoutA->changeEnabled(false);
    layoutC->changeEnabled(false);
    layoutA->changeEnabled(true);
    layoutB->changeBorder(border2);
    layoutB->changeBorder(border);

    rootLT->update(NULL, true);

    // after you've created a layout, you can save it (IrrlichtDevice is needed because of the XML funtions!):
    rootLT->save(Context.device, "test.lt");

    // here we delete the whole layout, for testing the load function, and also for showing how to create a root layout,
    // while you don't know, what's in the file:
    delete rootLT;
    rootLT = new CLayout;

    rootLT->load(Context.device, "test.lt");

    // if the window doesn't exist, create it:
    if (!window)
    {
        core::rect<s32> emptyRect;
        window = env->addWindow(emptyRect, false, L"Save & Load Example", rootGUI, EGI_SaveLoadWindow);

        env->addStaticText(L"A", emptyRect, true, true, window, EGI_SLW_LayoutA);
        env->addStaticText(L"B", emptyRect, true, true, window, EGI_SLW_LayoutB);
        env->addStaticText(L"C", emptyRect, true, true, window, EGI_SLW_LayoutC);
        env->addStaticText(L"D", emptyRect, true, true, window, EGI_SLW_LayoutD);
        env->addStaticText(L"BCD", emptyRect, true, true, window, EGI_SLW_LayoutBCD);
        env->addStaticText(L"E", emptyRect, true, true, window, EGI_SLW_LayoutE);

        env->addButton(emptyRect, window, EGI_SLW_Button0, L"button0 A");
        env->addButton(emptyRect, window, EGI_SLW_Button1, L"button1 A");
        env->addButton(emptyRect, window, EGI_SLW_Button2, L"button2 A");
        env->addButton(emptyRect, window, EGI_SLW_Button3, L"button3 B");
        env->addButton(emptyRect, window, EGI_SLW_Button4, L"button4 B");
        env->addButton(emptyRect, window, EGI_SLW_Button5, L"button5 C");
        env->addButton(emptyRect, window, EGI_SLW_Button6, L"button6 C");
        env->addButton(emptyRect, window, EGI_SLW_Button7, L"button7 D");
        env->addButton(emptyRect, window, EGI_SLW_Button8, L"button8 D");
        env->addButton(emptyRect, window, EGI_SLW_Button9, L"button9 D");
        env->addButton(emptyRect, window, EGI_SLW_Button10, L"button10 E");
        env->addButton(emptyRect, window, EGI_SLW_Button11, L"button11 E");
        env->addButton(emptyRect, window, EGI_SLW_Button12, L"button12 E");
        env->addStaticText(L"afterUpdate", emptyRect, true, true, window, EGI_SLW_AfterUpdate);

        // we don't know if the loading was successfull or not
        if (rootLT)
        {
            // don't forget to update the layout:
            rootLT->update(Context.device, true);

            core::dimension2du screenSize = Context.device->getVideoDriver()->getScreenSize();
            core::rect<s32> windowRect = rootLT->calcAbsRect();
            window->setRelativePosition(core::position2di((screenSize.Width / 2) - (windowRect.getWidth() / 2),
                                                          (screenSize.Height / 2) - (windowRect.getHeight() / 2)));
        }

        Context.listbox->addItem(L"Save & Load Example Window created");
        Context.counter += 30;
        if (Context.counter > 200) Context.counter = 0;
    }

    // don't forget to delete the layout, if you don't need it anymore:
    if (rootLT) delete rootLT;
}


#endif // EXAMPLES_H_INCLUDED
