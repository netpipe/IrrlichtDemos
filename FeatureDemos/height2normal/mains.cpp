#include <irrlicht.h>
#include <iostream>

using namespace irr;

class EventReceiver : public irr::IEventReceiver
{
    public:
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
            //else GuiEventOccured= false;
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

    private:
        bool GuiEventOccured;
        gui::EGUI_EVENT_TYPE GuiEventType;
        unsigned int GuiEventCallerId;
        core::stringw GuiEventFilename;
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
            dev= createDevice(video::EDT_OPENGL, core::dimension2du(800,600));
            smgr= dev->getSceneManager();
            video= smgr->getVideoDriver();
            gui= smgr->getGUIEnvironment();
            rec= new EventReceiver;
            dev->setEventReceiver(rec);

            scene::IMesh* Mesh= smgr->getMeshManipulator()->createMeshWithTangents(smgr->getGeometryCreator()->createCubeMesh(core::vector3df(2.4)));
            Node= smgr->addMeshSceneNode(Mesh);
            Node->setPosition(core::vector3df(0.5, 0, 3.5));
            Node->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
            Node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);

            smgr->addCameraSceneNode();
            smgr->addLightSceneNode(0, core::vector3df(0, 0, -5));

            gui->getSkin()->setFont(gui->getFont("data/font.bmp"));
            //gui->getSkin()->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255,255,255,255));
            gui->loadGUI("data/gui.xml");
            ((gui::IGUISpinBox*)gui->getRootGUIElement()->getElementFromId(EGI_HEIGHT, true))->setValue(10);

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
                        //Node->setMaterialTexture(0, Normal);
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
                else if(rec->getGuiEventCallerId() == EGI_ROTATION)
                {
                    gui::IGUIScrollBar* bar= 0;
                    bar= (gui::IGUIScrollBar*)gui->getRootGUIElement()->getElementFromId(EGI_ROTATION, true);
                    Node->setRotation(core::vector3df(0, bar->getPos(), 0));
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
                    if(AboutToOpenDiffuseMap) Node->setMaterialTexture(0, video->getTexture(rec->getGuiEventFileOpenFilename().c_str()));
                    else loadHeightmap(rec->getGuiEventFileOpenFilename().c_str());
                }
            }

            video->beginScene();

            core::stringw caption= "Height2Normal FPS: ";
            caption+= video->getFPS();
            dev->setWindowCaption(caption.c_str());
            smgr->drawAll();
            gui->drawAll();
dev->sleep(10,0);
            video->endScene();

            return dev->run();
        }

    private:
        void loadHeightmap(io::path path)
        {
            Height= video->getTexture(path.c_str());

            video::IImage* image= video->createImage(Height, core::position2d<s32>(0,0), Height->getSize());
            Normal= video->addTexture("normal", image);
            video->makeNormalMapTexture(Normal, ((gui::IGUISpinBox*)gui->getRootGUIElement()->getElementFromId(EGI_HEIGHT, true))->getValue());
            image->drop();

            gui::IGUIEditBox* edit= (gui::IGUIEditBox*)gui->getRootGUIElement()->getElementFromId(EGI_FILENAME, true);
            core::stringw name= path;
            core::array<core::stringw> splited;
            //name.split(splited, ".");

            edit->setText(name.c_str());

            Node->setMaterialTexture(0, Height);
            Node->setMaterialTexture(1, Normal);
        }

        void saveNormalmap()
        {
            video::IImage* image= video->createImage(Normal, core::position2d<s32>(0,0), Normal->getSize());
            gui::IGUIEditBox* edit= (gui::IGUIEditBox*)gui->getRootGUIElement()->getElementFromId(EGI_FILENAME, true);
            video->writeImageToFile(image, edit->getText());
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


    //What is this?
    //This is a comment. You use it to make it clear, what did you do here.
    //Ah!

    //We exit the program.
    return 0;
}
