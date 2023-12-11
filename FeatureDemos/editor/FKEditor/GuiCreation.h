#include "CGUITexturedSkin.h"

void createSkin()
{
    IGUIEnvironment* env = Device->getGUIEnvironment();
    IGUISkin* skin = env->getSkin();

    IGUIFont* font = Device->getGUIEnvironment()->getFont("media/fonts/fontlucida.png");

    if (font)
        skin->setFont(font, EGDF_DEFAULT);
/*
    #define SCOLOR_WHITE      SColor(255,255,255,255)
    #define SCOLOR_LIGHT      SColor(255,200,210,250)
    #define SCOLOR_MID        SColor(255,54,74,84)
    #define SCOLOR_MID2       SColor(255,54,84,104)
    #define SCOLOR_DARK       SColor(255,44,54,74)
    #define SCOLOR_BLACK      SColor(255,0,0,0)
*/

    #define SCOLOR_WHITE      SColor(255,96,96,96)
    #define SCOLOR_LIGHT      SColor(255,61,53,49)
    #define SCOLOR_MID        SColor(255,43,38,35)
    #define SCOLOR_MID2       SColor(255,30,28,28)
    #define SCOLOR_DARK       SColor(255,12,10,11)
    #define SCOLOR_BLACK      SColor(255,0,0,0)

    //! Setup skin.
    skin->setColor(EGDC_BUTTON_TEXT, SCOLOR_WHITE);
    skin->setColor(EGDC_3D_FACE, SCOLOR_MID2);
    skin->setColor(EGDC_3D_HIGH_LIGHT, SCOLOR_LIGHT);
    skin->setColor(EGDC_3D_LIGHT, SCOLOR_MID);

    skin->setColor(EGDC_WINDOW, SCOLOR_DARK);
    skin->setColor(EGDC_3D_DARK_SHADOW, SCOLOR_DARK);
    skin->setColor(EGDC_3D_SHADOW, SCOLOR_BLACK);
    skin->setColor(EGDC_APP_WORKSPACE, SCOLOR_DARK);
    skin->setColor(EGDC_GRAY_TEXT, SCOLOR_LIGHT);
}

void CreateMenus()
{
    IGUIEnvironment* env = Device->getGUIEnvironment();
    // create menu
    gui::IGUIContextMenu* menu = env->addMenu();
    menu->addItem(L"File", -1, true, true);
    menu->addItem(L"Display", -1, true, true);
    menu->addItem(L"Lighting", -1, true, true);
    menu->addItem(L"Animation", -1, true, true);
    menu->addItem(L"Windows", -1, true, true);
    menu->addItem(L"Help", -1, true, true);


    gui::IGUIContextMenu* submenu;
    submenu = menu->getSubMenu(0);
    submenu->addItem(L"Clear model...", 99);
    submenu->addItem(L"Get default model",98);
    submenu->addSeparator();
    submenu->addItem(L"Open Model File...", 100);
    submenu->addItem(L"Open model texture...", 101);
    submenu->addSeparator();
    submenu->addItem(L"Scenes",-1,true,true);
    submenu->addSeparator();
    submenu->addItem(L"Export mesh as ...",-1, true, true);
    submenu->addSeparator();
    submenu->addItem(L"Quit", 200);

    submenu = menu->getSubMenu(0)->getSubMenu(6);
    submenu->addItem(L"Clear current scene", 554,true);
    submenu->addItem(L"Load IRRscene", 552,true);
    submenu->addItem(L"Save IRRscene", 553,true);

    submenu = menu->getSubMenu(0)->getSubMenu(8);
    submenu->addItem(L"IRRmesh format", 550,false);
    submenu->addItem(L"Collada format...", 551,false);

    submenu = menu->getSubMenu(1);
    submenu->addItem(L"toggle model debug information", -1, true, true);
    submenu->addItem(L"model material", -1, true, true );
    submenu->addSeparator();
    submenu->addItem(L"Viewports", -1, true, true );
    submenu->addItem(L"Items",-1,true,true);
    submenu->addSeparator();
    submenu->addItem(L"Show sky box", 300, true, false, false);
    submenu->addItem(L"Show Grid", 301, true,false,true);
    submenu->addItem(L"Show ground", 303, true, false, false);
    submenu->addItem(L"Show perspective grid", 304, true, false, true);
    submenu->addItem(L"Show left toolbar [TAB]",305,true,false,true);
    submenu->addItem(L"Show Irrlicht logo",302, true, false, true);

    submenu = menu->getSubMenu(2);
    submenu->addItem(L"Use Lights on the model",510);
    submenu->addItem(L"No lighting on the model",520);
    submenu->addSeparator();
    submenu->addItem(L"Add the shadow volume",530);
    submenu->addItem(L"Remove the shadow volume",540);

    submenu = menu->getSubMenu(3);
    submenu->addItem(L"Set animation speed",-1,true,true);
    submenu->addItem(L"Play all animations", 2098);
    submenu->addItem(L"Play defined animation", 2099);

    submenu = menu->getSubMenu(1)->getSubMenu(0);
    submenu->addItem(L"None", 410);
    submenu->addItem(L"Normals", 420);
    submenu->addItem(L"Skeleton", 430);
    submenu->addItem(L"Wire overlay", 440);
    submenu->addItem(L"Half-Transparent", 620);
    submenu->addItem(L"Buffers bounding boxes", 460);

    submenu = menu->getSubMenu(1)->getSubMenu(1);
    submenu->addItem(L"Solid", 610);
    submenu->addItem(L"Transparent", 620);
    submenu->addItem(L"Reflection", 630);

    submenu = menu->getSubMenu(1)->getSubMenu(3);
    submenu->addItem(L"Perspective view", 2000, true, false, true);
    submenu->addItem(L"Top view", 2001, true, false, false);
    submenu->addItem(L"Front view", 2002, true, false, false);
    submenu->addItem(L"Side view", 2003, true, false, false);
    submenu->addSeparator();
    submenu->addItem(L"Quad view", 2010, true, false, false);
    submenu->addSeparator();
    submenu->addItem(L"reset views",2020);
    submenu->addItem(L"re-center viewports",2021);


    submenu = menu->getSubMenu(1)->getSubMenu(4);
    submenu->addItem(L"use view from perspective",2022);
    submenu->addItem(L"view center selected item",2023);
    submenu->addItem(L"move item in pers view center",2024);

    submenu = menu->getSubMenu(3)->getSubMenu(0);
    submenu->addItem(L"Stop Animation", 2100);
    submenu->addSeparator();
    submenu->addItem(L"Forward 6 FPS", 2101);
    submenu->addItem(L"Forward 12 FPS", 2102);
    submenu->addItem(L"Forward 24 FPS", 2103);
    submenu->addItem(L"Forward 30 FPS", 2104);
    submenu->addSeparator();
    submenu->addItem(L"Reverse 6 FPS", 2105);
    submenu->addItem(L"Reverse 12 FPS", 2106);
    submenu->addItem(L"Reverse 24 FPS", 2107);
    submenu->addItem(L"Reverse 30 FPS", 2108);

    /*submenu = menu->getSubMenu(2);
    submenu->addItem(L"Maya Style", 1000);
    submenu->addItem(L"First Person", 1100, false);*/

    submenu = menu->getSubMenu(4);
    submenu->addItem(L"Numeric panel", 6001);
    //submenu->addItem(L"Selection tool",6004,true);
    submenu->addItem(L"Model surface",6005,true);
    submenu->addItem(L"Animation Control",6002,true);
    submenu->addItem(L"Texture preview",6003,false);
    submenu->addItem(L"Configuration",6003,false);

    submenu = menu->getSubMenu(5);
    submenu->addItem(L"About", 500);
}

void createToolbar()
{
// create toolbar
    IGUIEnvironment* env = Device->getGUIEnvironment();
    video::IVideoDriver* driver = Device->getVideoDriver();
    gui::IGUIToolBar* bar = env->addToolBar();

    f32 screenx = driver->getScreenSize().Width;
    f32 screeny = driver->getScreenSize().Height;

    video::ITexture* image = driver->getTexture("media/ui/open.png");
    bar->addButton(1102, 0, L"Open a model",image, 0, false, true);

    image = driver->getTexture("media/ui/zip.png");
    bar->addButton(1105, 0, L"Open model texture",image, 0, false, true);

    image = driver->getTexture("media/ui/tools.png");
    bar->addButton(1104, 0, L"Open Tools",image, 0, false, true);

    image = driver->getTexture("media/ui/help.png");
    bar->addButton(1103, 0, L"Open Help", image, 0, false, true);

    image = driver->getTexture("media/ui/cell_start.png");
    bar->addButton(9000, 0, L"Add Player Start", image, 0, false, true);

    image = driver->getTexture("media/ui/cell_power.png");
    bar->addButton(9001, 0, L"Add Power Cell", image, 0, false, true);

    image = driver->getTexture("media/ui/cell_heart.png");
    bar->addButton(9002, 0, L"Add Heart Cell", image, 0, false, true);

    //image = driver->getTexture("media/ui/move.png");
    //moveMDL=bar->addButton(1106, 0, L"Move the model", image, 0, true, true);
    //moveMDL->setVisible(false);

    //image = driver->getTexture("media/ui/move.png");
    //rotaMDL=bar->addButton(1107, 0, L"Rotate the model", image, 0, true, true);
    //rotaMDL->setVisible(false);

    // status info (mostly coordinates
    // create fps text
    fpstext = env->addStaticText(L"", core::rect<s32>((s32)(screenx-400),4,(s32)(screenx-10),23), true, false, bar);
    fpstext->setAlignment(EGUIA_LOWERRIGHT , EGUIA_LOWERRIGHT , EGUIA_UPPERLEFT , EGUIA_UPPERLEFT );

    // Create the View description text
    vptopright = env->addStaticText(L"Perspective view", core::rect<s32>((s32)(screenx-150)-viewrightspace,(s32)60,(s32)(screenx-10)-viewrightspace,(s32)80), false, true);
    vptopleft = env->addStaticText(L"Top view", core::rect<s32>((s32)10,(s32)60,(s32)120,(s32)80), false, true);
    vpbottomleft = env->addStaticText(L"Side view", core::rect<s32>((s32)10,(s32)(screeny/2),(s32)120,(s32)(screeny/2)+20), false, true);

    vpbottomright = env->addStaticText(L"Front view", core::rect<s32>((s32)(screenx-150)-viewrightspace,(s32)(screeny/2),(s32)(screenx-10-viewrightspace),(s32)(screeny/2)+20), false, true);
    // Set the color and positionnement of theses view descriptions
    vptopright->setOverrideColor(SColor(255,255,255,255));
    vptopright->setAlignment(EGUIA_LOWERRIGHT , EGUIA_LOWERRIGHT , EGUIA_UPPERLEFT , EGUIA_UPPERLEFT );
    vptopright->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER);
    vptopleft->setOverrideColor(SColor(255,255,255,255));
    vptopleft->setTextAlignment(EGUIA_UPPERLEFT,EGUIA_CENTER);
    vpbottomleft->setOverrideColor(SColor(255,255,255,255));
    vpbottomleft->setAlignment(EGUIA_UPPERLEFT ,EGUIA_UPPERLEFT  , EGUIA_SCALE , EGUIA_SCALE );
    vpbottomleft->setTextAlignment(EGUIA_UPPERLEFT,EGUIA_CENTER);
    vpbottomright->setOverrideColor(SColor(255,255,255,255));
    vpbottomright->setAlignment(EGUIA_LOWERRIGHT ,EGUIA_LOWERRIGHT  , EGUIA_SCALE , EGUIA_SCALE );
    vpbottomright->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER);
    // hide theses since we start in single perspective view
    vptopleft->setVisible(false);
    vpbottomleft->setVisible(false);
    vpbottomright->setVisible(false);

}

void createSelect()
{
    IGUIEnvironment* env = Device->getGUIEnvironment();
    video::IVideoDriver* driver = Device->getVideoDriver();
    f32 screenx = driver->getScreenSize().Width;
    f32 screeny = driver->getScreenSize().Height;
    //create the bar
    tab1 = env->addTabControl(
               core::rect<s32>((s32)screenx-viewrightspace,55,(s32)screenx,(s32)(screeny/2)-5), 0, true, true);
    tab1->setAlignment(EGUIA_LOWERRIGHT , EGUIA_LOWERRIGHT ,EGUIA_UPPERLEFT , EGUIA_CENTER );
    IGUITab* t2 = tab1->addTab(L"Item");
    IGUITab* t1 = tab1->addTab(L"Modify");
    // Create the buttons
    moveMDL = env->addButton(core::rect< s32 >(10,10,viewrightspace-10,30),t1,1106,L"Move model",L"Move the current model");
    moveMDL->setIsPushButton(true);
    moveMDL->setVisible(true);

    rotaMDL =  env->addButton(core::rect< s32 >(10,35,viewrightspace-10,55),t1,1107,L"Rotate model",L"Rotate the current model");
    rotaMDL->setIsPushButton(true);
    rotaMDL->setVisible(true);

    scalMDL =  env->addButton(core::rect< s32 >(10,60,viewrightspace-10,80),t1,1108,L"Scale model",L"Scale the current model");
    scalMDL->setIsPushButton(true);
    scalMDL->setVisible(true);

    strcMDL =  env->addButton(core::rect< s32 >(10,85,viewrightspace-10,105),t1,1109,L"Stretch model",L"Stretch the current model");
    strcMDL->setIsPushButton(true);
    strcMDL->setVisible(false);

    env->addButton(core::rect< s32 >(10,115,viewrightspace-10,135),t1,1110,L"Reset",L"Reset changes");
    env->addButton(core::rect< s32 >(10,10,viewrightspace-10,30),t2,1111,L"Hide/Unhide",L"Hide the currently selected object");
    env->addButton(core::rect< s32 >(10,35,viewrightspace-10,55),t2,1112,L"Show wire",L"The current mesh will be in wireframe");
    env->addButton(core::rect< s32 >(10,60,viewrightspace-10,80),t2,1113,L"Hide wire",L"The current mesh will be back as normal");
    // Create the selection list
    seltitle = env->addStaticText(L"Selection list", core::rect<s32>((s32)(screenx-viewrightspace),(s32)(screeny/2),(s32)(screenx),(s32)screeny-100), true, true,0,-1,true);
    seltitle->setTextAlignment(EGUIA_CENTER,EGUIA_UPPERLEFT);
    seltitle->setAlignment(EGUIA_LOWERRIGHT , EGUIA_LOWERRIGHT , EGUIA_SCALE , EGUIA_LOWERRIGHT );
    //thistitle->setOverrideColor(SColor(255,255,255,255));
    selList=env->addListBox(core::rect<s32>((s32)(screenx-viewrightspace+2),(s32)(screeny/2)+20,(s32)(screenx),(s32)screeny-100),0,880,false);
    selList->setAlignment(EGUIA_LOWERRIGHT , EGUIA_LOWERRIGHT , EGUIA_SCALE , EGUIA_LOWERRIGHT );

    // Create the status display
    coords = env->addStaticText(L"", core::rect<s32>((s32)(screenx-viewrightspace),(s32)screeny-95,(s32)(screenx),(s32)screeny-10), true, true,0,-1,true);
    coords->setAlignment(EGUIA_LOWERRIGHT , EGUIA_LOWERRIGHT , EGUIA_LOWERRIGHT , EGUIA_LOWERRIGHT );
    coords->setOverrideFont(env->getFont("media/fonts/fonthaettenschweiler.bmp"));
}


void createLogo()
{
    // load the irrlicht engine logo
    IGUIEnvironment* env = Device->getGUIEnvironment();
    video::IVideoDriver* driver = Device->getVideoDriver();
    img = env->addImage(driver->getTexture("media/logo/irrlichtlogo2.png"),
                        core::position2d<s32>(10, driver->getScreenSize().Height - 128));

    // lock the logo's edges to the bottom left corner of the screen
    img->setAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT);
}
