#include "GUI.h"

CGUI::CGUI(IGUIEnvironment* gui, IVideoDriver* driver, ISceneManager* smgr, IrrlichtDevice* device, CEventReceiver* receiver, CSound* Sound)
{
    CGUI::receiver = receiver;
    CGUI::device = device;
    CGUI::smgr = smgr;
    CGUI::driver = driver;
    CGUI::gui = gui;
    CGUI::ReloadPreviousMap = false;
    CGUI::PreviousMapFile = L"FAILED";
    CGUI::Sound = Sound;
}

core::stringw CGUI::MainMenu()
{
    if(DEBUG_CONSOLE)
        cout<<"Entering Main Menu"<<endl;

    if(CGUI::ReloadPreviousMap)
        return CGUI::PreviousMapFile;

    core::stringw mapName = L"_";

    core::stringw FAILED = L"FAILED";
    ITexture* Logo = CGUI::driver->getTexture("./Media/Images/SIGMUS.png");

    CGUI::SIGMUSImage = CGUI::gui->addImage(Logo, position2d<s32>(10, 50));
    CGUI::LoadMapButton = CGUI::gui->addButton(rect<s32>(50, 270, 150, 300), 0, -1, L"Load Map");
    CGUI::QuitButton = CGUI::gui->addButton(rect<s32>(50,320,100,350), 0, -1, L"Quit");
    CGUI::FileOpenD = CGUI::gui->addEditBox(L"Map Name", rect<s32>(215,270,370,290));
    CGUI::HelperText = CGUI::gui->addStaticText(L"Only currently working map is \"Intro.xml\"", rect<s32>(215,250,600,270));
    CGUI::HelperText->setOverrideColor(SColor(255, 255, 255, 255));

    CGUI::BackgroundNode = CGUI::smgr->addOctreeSceneNode(CGUI::smgr->getMesh("./Media/Maps/Menu_Warehouse/Warehouse.obj"));

    if(DEBUG_CONSOLE)
    {
        if(CGUI::BackgroundNode)
            cout<<"Loaded background model"<<endl;
        else
        {
            cout<<"Failed to load background model"<<endl;
            return FAILED;
        }
    }

    CGUI::BackgroundNode->setPosition(vector3df(0,0,0));
    CGUI::BackgroundNode->setMaterialFlag(EMF_LIGHTING,LIGHTING);
	CGUI::BackgroundNode->setMaterialFlag(EMF_NORMALIZE_NORMALS,true);
	CGUI::smgr->getMeshManipulator()->recalculateNormals(CGUI::BackgroundNode->getMesh(),true);

    if(LIGHTING)
    {
        ILightSceneNode* light;
        light = smgr->addLightSceneNode(0,vector3df(319,-90,538),SColorf(5,5,5,5),10);
        light = smgr->addLightSceneNode(0,vector3df(700,19,269),SColorf(5,5,5,5),40);
        light = smgr->addLightSceneNode(0,vector3df(-613,-6,-158),SColorf(5,5,5,5),40);
        light = smgr->addLightSceneNode(0,vector3df(-254,232,431),SColorf(5,5,5,5),40);
        light = smgr->addLightSceneNode(0,vector3df(58,194,-883),SColorf(5,5,5,5),10);
        light = smgr->addLightSceneNode(0,vector3df(698,386,-990),SColorf(5,5,5,5),10);
        light = smgr->addLightSceneNode(0,vector3df(743,396,-1461),SColorf(5,5,5,5),20);
        light = smgr->addLightSceneNode(0,vector3df(901,-77,-700),SColorf(5,5,5,5),10);
        light = smgr->addLightSceneNode(0,vector3df(768,-14,-1491),SColorf(5,5,5,5),20);
        light = smgr->addLightSceneNode(0,vector3df(310,-110,-1100),SColorf(5,5,5,5),10);
        light = smgr->addLightSceneNode(0,vector3df(-58,58,-1480),SColorf(5,5,5,5),20);
        light = smgr->addLightSceneNode(0,vector3df(-640,-89,-1386),SColorf(5,5,5,5),20);
        light = smgr->addLightSceneNode(0,vector3df(-908,370,-1475),SColorf(5,5,5,5),10);
    }

    CGUI::Camera = CGUI::smgr->addCameraSceneNode();

    core::array<vector3df> Points;
    Points.push_back(vector3df(395,-122,615));
    Points.push_back(vector3df(444,-132,447));
    Points.push_back(vector3df(713,-105,110));
    Points.push_back(vector3df(-215,26,-94));
    Points.push_back(vector3df(-637,183,393));
    Points.push_back(vector3df(-379,256,-626));
    Points.push_back(vector3df(725,173,-607));
    Points.push_back(vector3df(906,-106,875));
    Points.push_back(vector3df(907,-67,-1419));
    Points.push_back(vector3df(2,-13,-1431));
    Points.push_back(vector3df(-837,-96,-1409));
    Points.push_back(vector3df(-773,218,-1179));
    Points.push_back(vector3df(-339,246,-1149));
    Points.push_back(vector3df(139,414,-1590));
    Points.push_back(vector3df(634,402,-1324));
    Points.push_back(vector3df(682,383,-970));
    Points.push_back(vector3df(540,-77,222));
    Points.push_back(vector3df(395,-122,615));

    ISceneNodeAnimator* anim = CGUI::smgr->createFollowSplineAnimator(0,Points,0.5f,0.5f,true,false);
    CGUI::Camera->addAnimator(anim);
    anim->drop();

    CGUI::device->getCursorControl()->setVisible(true);

    while(CGUI::device->run() && !CGUI::receiver->IsKeyDown(KEY_ESCAPE))
    {
        if(CGUI::QuitButton->isPressed())
        {
            mapName = L"^^%&&--QUIT!@#321";
            return mapName;
        }

        if(CGUI::LoadMapButton->isPressed())
        {
            mapName = CGUI::FileOpenD->getText();
            CGUI::PreviousMapFile = CGUI::FileOpenD->getText();
            break;
        }

        CGUI::driver->beginScene(true, true);

        CGUI::smgr->drawAll();
        CGUI::gui->drawAll();

        CGUI::driver->endScene();
    }

    if(mapName != L"_")
        return mapName;
    else
        return FAILED;
}

bool CGUI::QuitMenu()
{
    CGUI::device->getCursorControl()->setVisible(true);

    CGUI::QuitPromptWindow = CGUI::gui->addWindow(rect<s32>(500,500,900,600), true, L"Quit");
    CGUI::QuitPromptText = CGUI::gui->addStaticText(L"Do you wish to leave, or restart the current Map?", rect<s32>(10,30,600,45), false, true, CGUI::QuitPromptWindow);
    CGUI::GotoMenu = CGUI::gui->addButton(rect<s32>(10,55,110,85), CGUI::QuitPromptWindow, -1, L"Goto Menu");
    CGUI::RestartMap = CGUI::gui->addButton(rect<s32>(280,55,390,85), CGUI::QuitPromptWindow, -1, L"Restart Map");
    CGUI::CancelQuit = CGUI::gui->addButton(rect<s32>(130,55,260,85), CGUI::QuitPromptWindow, -1, L"Cancel");

    CGUI::QuitPromptText->setOverrideColor(SColor(255, 255, 255, 255));

    while(CGUI::device->run())
    {
        if(CGUI::GotoMenu->isPressed())
        {
            CGUI::QuitPromptWindow->remove();
            CGUI::device->getCursorControl()->setVisible(false);
            CGUI::ReloadPreviousMap = false;
            return true;
        }

        if(CGUI::RestartMap->isPressed())
        {
            CGUI::QuitPromptWindow->remove();
            CGUI::device->getCursorControl()->setVisible(false);
            CGUI::ReloadPreviousMap = true;
            return true;
        }

        if(CGUI::CancelQuit->isPressed())
        {
            CGUI::QuitPromptWindow->remove();
            CGUI::device->getCursorControl()->setVisible(false);
            return false;
        }

        CGUI::driver->beginScene(true, true);

        CGUI::smgr->drawAll();
        CGUI::gui->drawAll();

        CGUI::driver->endScene();
    }
}

void CGUI::DeathMenu()
{
    CGUI::smgr->clear();
    CGUI::gui->clear();

//    CGUI::Sound->getEngine()->stopAllSounds();

    CGUI::device->getCursorControl()->setVisible(true);

    CGUI::QuitPromptWindow = CGUI::gui->addWindow(rect<s32>(500,500,900,600), true, L"You have Expired");
    CGUI::QuitPromptText = CGUI::gui->addStaticText(L"Do you wish to leave, or restart the current Map?", rect<s32>(10,30,600,50), false, true, CGUI::QuitPromptWindow);
    CGUI::GotoMenu = CGUI::gui->addButton(rect<s32>(10,55,110,85), CGUI::QuitPromptWindow, -1, L"Goto Menu");
    CGUI::RestartMap = CGUI::gui->addButton(rect<s32>(280,55,390,85), CGUI::QuitPromptWindow, -1, L"Restart Map");

    CGUI::QuitPromptText->setOverrideColor(SColor(255, 255, 255, 255));

    while(CGUI::device->run())
    {
        if(CGUI::GotoMenu->isPressed())
        {
            CGUI::QuitPromptWindow->remove();
            CGUI::device->getCursorControl()->setVisible(false);
            CGUI::ReloadPreviousMap = false;
            break;
        }

        if(CGUI::RestartMap->isPressed())
        {
            CGUI::QuitPromptWindow->remove();
            CGUI::device->getCursorControl()->setVisible(false);
            CGUI::ReloadPreviousMap = true;
            break;
        }

        CGUI::driver->beginScene(true, true);

        CGUI::smgr->drawAll();
        CGUI::gui->drawAll();

        CGUI::driver->endScene();
    }
}

void CGUI::EndMenu()
{
    CGUI::smgr->clear();
    CGUI::gui->clear();

//    CGUI::Sound->getEngine()->stopAllSounds();

    CGUI::device->getCursorControl()->setVisible(true);

    CGUI::QuitPromptWindow = CGUI::gui->addWindow(rect<s32>(500,500,900,600), true, L"You have completed the Map");
    CGUI::QuitPromptText = CGUI::gui->addStaticText(L"Do you wish to leave, or restart?", rect<s32>(10,30,600,50), false, true, CGUI::QuitPromptWindow);
    CGUI::GotoMenu = CGUI::gui->addButton(rect<s32>(10,55,110,85), CGUI::QuitPromptWindow, -1, L"Goto Menu");
    CGUI::RestartMap = CGUI::gui->addButton(rect<s32>(280,55,390,85), CGUI::QuitPromptWindow, -1, L"Restart Map");

    CGUI::QuitPromptText->setOverrideColor(SColor(255, 255, 255, 255));

    while(CGUI::device->run())
    {
        if(CGUI::GotoMenu->isPressed())
        {
            CGUI::QuitPromptWindow->remove();
            CGUI::device->getCursorControl()->setVisible(false);
            CGUI::ReloadPreviousMap = false;
            break;
        }

        if(CGUI::RestartMap->isPressed())
        {
            CGUI::QuitPromptWindow->remove();
            CGUI::device->getCursorControl()->setVisible(false);
            CGUI::ReloadPreviousMap = true;
            break;
        }

        CGUI::driver->beginScene(true, true);

        CGUI::smgr->drawAll();
        CGUI::gui->drawAll();

        CGUI::driver->endScene();
    }
}

void CGUI::cleanMainMenu()
{
    CGUI::smgr->clear();
    CGUI::gui->clear();

    CGUI::device->getCursorControl()->setVisible(false);
}
