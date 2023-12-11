/* main.cpp -- Height2Normal

  Copyright (C) 2011-2013 Adam 'entity' Krupicka

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
#include <iostream>

#define VERSION "2.0"

using namespace irr;

class EventReceiver : public irr::IEventReceiver
{
    public:
        EventReceiver()
        {
            GuiEventOccured= false;
            MouseDragStarted= false;
            MouseDragInProgress= false;
        }

        virtual bool OnEvent(const irr::SEvent& event)
        {
            if(event.EventType == irr::EET_GUI_EVENT)
            {
                GuiEventOccured= true;
                GuiEventType= event.GUIEvent.EventType;
                GuiEventCallerId= event.GUIEvent.Caller->getID();
                if(GuiEventType == gui::EGET_FILE_SELECTED)
                {
                    gui::IGUIFileOpenDialog* dia= (gui::IGUIFileOpenDialog*)event.GUIEvent.Caller;
                    GuiEventFilename= dia->getFileName();
                }
            }
            else if(event.EventType == irr::EET_MOUSE_INPUT_EVENT)
            {
                if(event.MouseInput.isLeftPressed())
                {
                    if(!MouseDragStarted)
                    {
                        MouseDragStarted= true;
                        MouseLast= core::vector2di(event.MouseInput.X, event.MouseInput.Y);
                    }
                    else
                    {
                        MouseDragInProgress= true;
                        core::vector2di MouseNow= core::vector2di(event.MouseInput.X, event.MouseInput.Y);
                        MouseDelta= MouseNow - MouseLast;
                        MouseLast= MouseNow;
                    }
                }
                else
                {
                    MouseDragInProgress= false;
                    MouseDragStarted= false;
                }
            }
            return false;
        }

        bool guiEvenOccured()
        {
            if(GuiEventOccured)
            {
                GuiEventOccured= false;
                return true;
            }
            return false;
        }

        gui::EGUI_EVENT_TYPE getGuiEventType()
        {
            return GuiEventType;
        }

        unsigned int getGuiEventCallerId()
        {
            return GuiEventCallerId;
        }

        core::stringw getGuiEventFileOpenFilename()
        {
            return GuiEventFilename;
        }

        bool mouseDragOccured()
        {
            return MouseDragInProgress;
        }

        core::vector2di getMouseDelta()
        {
            core::vector2di retVal= MouseDelta;
            MouseDelta= core::vector2di(0, 0);
            return retVal;
        }

    private:
        bool GuiEventOccured;
        gui::EGUI_EVENT_TYPE GuiEventType;
        unsigned int GuiEventCallerId;
        core::stringw GuiEventFilename;

        bool MouseDragStarted, MouseDragInProgress;
        core::vector2di MouseLast, MouseDelta;
};


enum EGUI_ID
{
    EGI_LOAD= 1,
    EGI_HEIGHT,
    EGI_FILENAME,
    EGI_SAVE,
    EGI_DIFFUSE,
    EGI_ROTATION
};



class Framework
{
    public:
        Framework()
        {
            dev= createDevice(video::EDT_OPENGL, core::dimension2du(800,600), 16, false, false, true);
            smgr= dev->getSceneManager();
            video= smgr->getVideoDriver();
            gui= smgr->getGUIEnvironment();
            rec= new EventReceiver;
            dev->setEventReceiver(rec);
            video->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

            scene::IMesh* mesh= smgr->getMeshManipulator()->createMeshWithTangents(smgr->getMesh("data/cube.obj"));
            Node= smgr->addMeshSceneNode(mesh);
            Node->setPosition(core::vector3df(0.5, 0, 3.5));
            Node->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
            Node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);

            smgr->addCameraSceneNode();
            smgr->addLightSceneNode(0, core::vector3df(0, 0, -5));

            gui->getSkin()->setFont(gui->getFont("data/font.bmp"));
            gui->loadGUI("data/gui.xml");
            gui::IGUISpinBox* spinBox= (gui::IGUISpinBox*)gui->getRootGUIElement()->getElementFromId(EGI_HEIGHT, true);
            spinBox->setValue(20);
            spinBox->getEditBox()->setOverrideColor(video::SColor(0xfaffffff));

            loadHeightmap("data/example.png");
            dev->run();
        }

        ~Framework()
        {
            dev->closeDevice();
        }

        bool run()
        {
            if(rec->guiEvenOccured())
            {
                if(rec->getGuiEventCallerId() == EGI_LOAD)
                {
                    if(rec->getGuiEventType() == gui::EGET_BUTTON_CLICKED) gui->addFileOpenDialog(L"Please select a height map");
                    AboutToOpenDiffuseMap= false;
                }
                else if(rec->getGuiEventCallerId() == EGI_HEIGHT)
                {
                    gui::IGUISpinBox* spin= 0;
                    spin= (gui::IGUISpinBox*)gui->getRootGUIElement()->getElementFromId(EGI_HEIGHT, true);

                    if(spin && Normal)
                    {
                        video->removeTexture(Normal);
                        Normal= 0;
                        video::IImage* image= video->createImage(Height, core::position2d<s32>(0,0), Height->getSize());
                        Normal= video->addTexture("normal", image);
                        video->makeNormalMapTexture(Normal, spin->getValue());
                        image->drop();
                        Node->setMaterialTexture(1, Normal);
                    }
                    else
                    {
                        Node->setMaterialTexture(1, 0);
                        Node->setMaterialType(video::EMT_SOLID);
                    }
                }
                else if(rec->getGuiEventCallerId() == EGI_DIFFUSE)
                {
                    if(rec->getGuiEventType() == gui::EGET_BUTTON_CLICKED)
                    {
                        gui->addFileOpenDialog(L"Please select a diffuse map");
                        AboutToOpenDiffuseMap= true;
                    }
                }
                else if(rec->getGuiEventCallerId() == EGI_SAVE)
                {
                    if(rec->getGuiEventType() == gui::EGET_BUTTON_CLICKED)
                    {
                        saveNormalmap();
                    }
                }

                if(rec->getGuiEventType() == gui::EGET_FILE_SELECTED)
                {
                    if(AboutToOpenDiffuseMap)
                    {
                        Node->setMaterialTexture(0, video->getTexture(rec->getGuiEventFileOpenFilename().c_str()));
                    }
                    else loadHeightmap(rec->getGuiEventFileOpenFilename().c_str());
                }
            }

            if(rec->mouseDragOccured())
            {
                #ifdef _IRR_LINUX_PLATFORM_
                #define STRENGTH 4
                #else
                #define STRENGTH 0.25
                #endif

                core::vector3df rotation= Node->getRotation();
                core::vector2di mouse= rec->getMouseDelta();
                rotation.Y-= mouse.X * STRENGTH;
                rotation.X-= mouse.Y * STRENGTH;
                Node->setRotation(rotation);
            }

            video->beginScene();

            core::stringw caption= "Height2Normal v. "VERSION"  FPS: ";
            caption+= video->getFPS();
            dev->setWindowCaption(caption.c_str());
            smgr->drawAll();
            gui->drawAll();

            video->endScene();

            return dev->run();
        }

    private:
        void loadHeightmap(io::path path)
        {
            video->removeAllTextures();

            Height= video->getTexture(path.c_str());

            video::IImage* image= video->createImage(Height, core::position2d<s32>(0,0), Height->getSize());
            Normal= video->addTexture("normal", image);
            video->makeNormalMapTexture(Normal, ((gui::IGUISpinBox*)gui->getRootGUIElement()->getElementFromId(EGI_HEIGHT, true))->getValue());
            image->drop();

            gui::IGUIEditBox* edit= (gui::IGUIEditBox*)gui->getRootGUIElement()->getElementFromId(EGI_FILENAME, true);
            core::stringw name= path;

            edit->setText(name.c_str());

            Node->setMaterialTexture(0, Height);
            Node->setMaterialTexture(1, Normal);
        }

        void saveNormalmap()
        {
            video::IImage* image= video->createImage(Normal, core::position2d<s32>(0,0), Normal->getSize());
            gui::IGUIEditBox* edit= (gui::IGUIEditBox*)gui->getRootGUIElement()->getElementFromId(EGI_FILENAME, true);
            core::stringw filename= edit->getText();
            bool success= video->writeImageToFile(image, filename.c_str());
            if(success)
            {
                s32 startPos= filename.findLast('/') + 1;
                s32 endPos= filename.size() - startPos;
                core::stringw message= L"Normal map saved as ";
                message+= filename.subString(startPos, endPos);
                gui->addMessageBox(L"Success!", message.c_str(), true, gui::EMBF_OK);
            }
            else
            {
                gui->addMessageBox(L"Error!", L"Could not write file.", true, gui::EMBF_OK);
            }
        }

        scene::IMeshSceneNode* Node;
        video::ITexture* Height, *Normal;
        IrrlichtDevice* dev;
        scene::ISceneManager* smgr;
        video::IVideoDriver* video;
        gui::IGUIEnvironment* gui;
        EventReceiver* rec;
        bool AboutToOpenDiffuseMap;
};



int main()
{
    Framework* framework= new Framework;

    while(framework->run())
    {

    }

    delete framework;


    //- What is this?
    //- This is a comment. You use it to make it clear what you did here.
    //- Ah! Now I finally get it! Here, watch me:

    //We exit the program.
    return 0;
}
