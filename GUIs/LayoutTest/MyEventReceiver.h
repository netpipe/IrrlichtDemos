#ifndef MYEVENTRECEIVER_H_INCLUDED
#define MYEVENTRECEIVER_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "Examples.h"


class MyEventReceiver : public IEventReceiver
{
    public:
        MyEventReceiver(SAppContext & context) : Context(context) { }

        virtual bool OnEvent(const SEvent& event)
        {
            if (event.EventType == EET_GUI_EVENT)
            {
                s32 id = event.GUIEvent.Caller->getID();
                gui::IGUIEnvironment* env = Context.device->getGUIEnvironment();

                switch(event.GUIEvent.EventType)
                {
                    case gui::EGET_SCROLL_BAR_CHANGED:
                    {
                        if (id == EGI_TransparencyScrollbar)
                        {
                            s32 pos = ((gui::IGUIScrollBar*)event.GUIEvent.Caller)->getPos();

                            for (u32 i=0; i<gui::EGDC_COUNT ; ++i)
                            {
                                video::SColor col = env->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)i);
                                col.setAlpha(pos);
                                env->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
                            }

                        }
                    }
                    break;

                    case gui::EGET_BUTTON_CLICKED:
                    {
                        switch (id)
                        {
                            case EGI_QuitButton: // exits from the application
                            {
                                Context.device->closeDevice();
                                return true;
                            }

                            case EGI_AlignBorderEnableWindowButton: // create an example window:
                            {
                                addAlignBorderEnableExampleWindow(Context);
                                return true;
                            }
                            case EGI_ABEW_EnableButton: // enable a part of the layout:
                            {
                                CLayout* rootLT = Context.AlignBorderEnableExampleRoot, *child = NULL;

                                if (rootLT)
                                {
                                    // see how to find and change an element of the layout:
                                    if (child = rootLT->findChild(EGI_ABEW_AlignsLayout)) child->changeEnabled(true);

                                    // don't forget to update it after the changes:
                                    // we want the layout to keep it's current size, if possible:
                                    rootLT->update(Context.device, false);
                                }

                                return true;
                            }
                            case EGI_ABEW_DisableButton: // disable a part of the layout:
                            {
                                CLayout* rootLT = Context.AlignBorderEnableExampleRoot, *child = NULL;

                                if (rootLT)
                                {
                                    // see how to find and change an element of the layout:
                                    if (child = rootLT->findChild(EGI_ABEW_AlignsLayout)) child->changeEnabled(false);

                                    // don't forget to update it after the changes:
                                    // we want the layout to keep it's current size, if possible:
                                    rootLT->update(Context.device, false);
                                }

                                return true;
                            }
                            case EGI_ABEW_SmallBorderButton: // set a new border to 2 layouts:
                            {
                                CLayout* rootLT = Context.AlignBorderEnableExampleRoot, *child = NULL;
                                core::dimension2di border(5, 5);

                                if (rootLT)
                                {
                                    // see how to find and change elements of the layout:
                                    if (child = rootLT->findChild(EGI_ABEW_HAlignLayout)) child->changeBorder(border);
                                    if (child = rootLT->findChild(EGI_ABEW_VAlignLayout)) child->changeBorder(border);

                                    rootLT->update(Context.device, false);
                                }

                                return true;
                            }
                            case EGI_ABEW_MediumBorderButton:
                            {
                                CLayout* rootLT = Context.AlignBorderEnableExampleRoot, *child = NULL;
                                core::dimension2di border(10, 10);

                                if (rootLT)
                                {
                                    // see how to find and change elements of the layout:
                                    if (child = rootLT->findChild(EGI_ABEW_HAlignLayout)) child->changeBorder(border);
                                    if (child = rootLT->findChild(EGI_ABEW_VAlignLayout)) child->changeBorder(border);

                                    rootLT->update(Context.device, false);
                                }

                                return true;
                            }
                            case EGI_ABEW_LargeBorderButton:
                            {
                                CLayout* rootLT = Context.AlignBorderEnableExampleRoot, *child = NULL;
                                core::dimension2di border(20, 20);

                                if (rootLT)
                                {
                                    if (child = rootLT->findChild(EGI_ABEW_HAlignLayout)) child->changeBorder(border);
                                    if (child = rootLT->findChild(EGI_ABEW_VAlignLayout)) child->changeBorder(border);

                                    rootLT->update(Context.device, false);
                                }

                                return true;
                            }
                            case EGI_ABEW_ToggleHAlignButton: // toggle the alignment of a slot:
                            {
                                CLayout* rootLT = Context.AlignBorderEnableExampleRoot, *child = NULL;

                                if (rootLT)
                                {
                                    if (child = rootLT->findChild(EGI_ABEW_ToggleHAlignButton))
                                    {
                                        int a = (int)child->getAlign();
                                        child->changeAlign(CLayout::ELayoutAlignment(++a > 3 ? 1 : a));
                                    }

                                    rootLT->update(Context.device, false);
                                }

                                return true;
                            }
                            case EGI_ABEW_ToggleVAlignButton:
                            {
                                CLayout* rootLT = Context.AlignBorderEnableExampleRoot, *child = NULL;

                                if (rootLT)
                                {
                                    if (child = rootLT->findChild(EGI_ABEW_ToggleVAlignButton))
                                    {
                                        int a = (int)child->getAlign();
                                        child->changeAlign(CLayout::ELayoutAlignment(++a > 3 ? 1 : a));
                                    }

                                    rootLT->update(Context.device, false);
                                }

                                return true;
                            }

                            case EGI_StretchAddRemoveWindowButton: // create an example window
                            {
                                addStretchAddRemoveExampleWindow(Context);
                                return true;
                            }
                            case EGI_SARW_StretchHButton: // stretch a slot inside the layout:
                            {
                                CLayout* rootLT = Context.StretchAddRemoveExampleRoot, *child = NULL;

                                if (rootLT)
                                {
                                    if (child = rootLT->findChild(EGI_SARW_StretchElementLabel))
                                        child->changeStretch(child->getStretchH() ? false : true, child->getStretchV());

                                    rootLT->update(Context.device, false);
                                }

                                return true;
                            }
                            case EGI_SARW_StretchVButton:
                            {
                                CLayout* rootLT = Context.StretchAddRemoveExampleRoot, *child = NULL;

                                if (rootLT)
                                {
                                    if (child = rootLT->findChild(EGI_SARW_StretchElementLabel))
                                        child->changeStretch(child->getStretchH(), child->getStretchV() ? false : true);

                                    rootLT->update(Context.device, false);
                                }

                                return true;
                            }
                            case EGI_SARW_AddButton: // add new elements to the window and to the layout:
                            {
                                CLayout* rootLT = Context.StretchAddRemoveExampleRoot, *child = NULL;
                                core::dimension2di buttonSize(150, 30);
                                core::rect<s32> emptyRect;

                                if (rootLT)
                                {
                                    // this is the layout we want to put new slots into:
                                    if (child = rootLT->findChild(EGI_SARW_AddRemoveElementsLayout))
                                    {
                                        // this is the maximum amounts of elements that can be added:
                                        int maxCount = EGI_SARW_AddRemoveElementsEnd - EGI_SARW_AddRemoveElementsStart;

                                        if (child->getCount() < maxCount)
                                        {
                                            // we'll need the example window to set as the parent of the new gui elements
                                            gui::IGUIElement* window = env->getRootGUIElement()->getElementFromId(EGI_StretchAddRemoveWindow);

                                            // we'll need unique ID for the new element
                                            int nID = EGI_SARW_AddRemoveElementsStart + child->getCount();
                                            core::stringw text = core::stringw(L"New Element ") + core::stringw(child->getCount());

                                            // create the new element, and add it to the layout:
                                            env->addStaticText(text.c_str(), emptyRect, true, true, window, nID);
                                            child->addSlot(nID, buttonSize, true);
                                        }
                                    }

                                    // this time we want the layout to calculte a new size:
                                    rootLT->update(Context.device, true);
                                }

                                return true;
                            }
                            case EGI_SARW_RemoveButton: // remove elements from the layout, and from the gui root:
                            {
                                CLayout* rootLT = Context.StretchAddRemoveExampleRoot, *child = NULL;
                                gui::IGUIElement *rootGUI = env->getRootGUIElement();

                                if (rootLT)
                                {
                                    if (child = rootLT->findChild(EGI_SARW_AddRemoveElementsLayout))
                                    {
                                        // if there are slots inside this layout
                                        if (child->getCount() > 0)
                                        {
                                            gui::IGUIElement* e = NULL;

                                            // this is where their IDs start from:
                                            int sID = EGI_SARW_AddRemoveElementsStart;

                                            // remove all the gui elements:
                                            for (int i = sID; i < sID + child->getCount(); ++i)
                                            {
                                                e = rootGUI->getElementFromId(i, true);
                                                if (e) e->remove();
                                            }

                                            // remove them from the layout:
                                            child->removeChildren();
                                        }
                                    }

                                    // this time we want the layout to calculte a new size:
                                    rootLT->update(Context.device, true);
                                }

                                return true;
                            }

                            case EGI_SaveLoadWindowButton: // creates an example window
                            {
                                addSaveLoadExampleWindow(Context);
                                return true;
                            }

                            default: return false;
                        }
                    }
                    break;

                    case gui::EGET_ELEMENT_CLOSED:
                    {
                        if (id == EGI_StretchAddRemoveWindow)
                        {
                            // remove elements from the layout, and from the gui root:
                            CLayout* rootLT = Context.StretchAddRemoveExampleRoot, *child = NULL;
                            gui::IGUIElement *rootGUI = env->getRootGUIElement();

                            if (rootLT)
                            {
                                if (child = rootLT->findChild(EGI_SARW_AddRemoveElementsLayout))
                                {
                                    // if there are slots inside this layout
                                    if (child->getCount() > 0)
                                    {
                                        gui::IGUIElement* e = NULL;

                                        // this is where their IDs start from:
                                        int sID = EGI_SARW_AddRemoveElementsStart;

                                        // remove all the gui elements:
                                        for (int i = sID; i < sID + child->getCount(); ++i)
                                        {
                                            e = rootGUI->getElementFromId(i, true);
                                            if (e) e->remove();
                                        }

                                        // remove them from the layout:
                                        child->removeChildren();
                                    }
                                }

                                // this time we want the layout to calculte a new size:
                                rootLT->update(NULL, true);
                            }

                            // closing the window normally:
                            return false;
                        }
                    }
                    break;

                    default: break;
                }
            }

            return false;
        }

    private:
        SAppContext & Context;
};


#endif // MYEVENTRECEIVER_H_INCLUDED
