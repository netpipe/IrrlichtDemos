int main()
{
    // create GUI event receiver object
    GUIEventReceiver GUIreceiver;

    // instantiate a software irrlicht device
    device = createDevice(
        video::EDT_SOFTWARE,
        core::dimension2d<s32>(640,480),
        8,
        false,
        false,
        false,
        &GUIreceiver);

    // create handle to 2D user interface
      IGUIEnvironment* GUIenv = device->getGUIEnvironment();

    // create an empty gui window
   IGUIWindow* guiWindow = GUIenv->addWindow(
        core::rect<s32>(0,0,640,480),
      true,
        L"irrPreSettings_Pure v1.0 build 061120",
        0,
        100);

    // add fullscreen check box
   IGUICheckBox* cbFullScreen = GUIenv->addCheckBox(
        false,
        core::rect<s32>(10,35,100,45),
        guiWindow,
        101,
        L"Fullscreen");

    // add stencil buffer check box
   IGUICheckBox* cbStencilBuffer = GUIenv->addCheckBox(
        false,
        core::rect<s32>(105,35,195,45),
        guiWindow,
        102,
        L"Stencilbuffer");

    // add vsync check box
   IGUICheckBox* cbVsync = GUIenv->addCheckBox(
        false,
        core::rect<s32>(200,35,290,45),
        guiWindow,
        103,
        L"Vsync");

    // add label for display driver combobox
    IGUIStaticText* lbDisplayDriver = GUIenv->addStaticText(
        L"Display Driver",
        core::rect<s32>(10,65,90,80),
        false,
        false,
        guiWindow,
        104,
        false);

    // add display driver combobox
    IGUIComboBox* cmbDisplayDriver = GUIenv->addComboBox(
        core::rect<s32>(100,65,250,80),
        guiWindow,
        105);

    // populate display driver combobox
    cmbDisplayDriver->addItem(L"Direct3D 9 Renderer");
    cmbDisplayDriver->addItem(L"Direct3D 8 Renderer");
    cmbDisplayDriver->addItem(L"OpenGL Renderer");
    cmbDisplayDriver->addItem(L"Software Renderer");
    cmbDisplayDriver->addItem(L"Apfelbaum Software Renderer");
    cmbDisplayDriver->addItem(L"Null Device");

    // add label for screen resolution combobox
    IGUIStaticText* lbScreenResolution = GUIenv->addStaticText(
        L"Screen Resolution",
        core::rect<s32>(300,65,460,80),
        false,
        false,
        guiWindow,
        106,
        false);

    // add screen resolution combobox
    IGUIComboBox* cmbScreenResolution = GUIenv->addComboBox(
        core::rect<s32>(390,65,550,80),
        guiWindow,
        107);

    // query the device to obtain the available video mode
    video::IVideoModeList * availableVideoModeList = device->getVideoModeList();

    // define array to hold a formatted listing of the availabe modes
    wchar_t formated_listing[64] = L" ";

    // go through all available video mode for graphic adapter
    for(int i = 0; i < availableVideoModeList->getVideoModeCount(); ++i)
    {
        // format available modes into desired listing
        swprintf(
            formated_listing,
            64,
            L"%i x %i pixels @ %i-bit colors",
            availableVideoModeList->getVideoModeResolution(i).Width,
            availableVideoModeList->getVideoModeResolution(i).Height,
            availableVideoModeList->getVideoModeDepth(i));

        // add formatted mode to combo list box
        cmbScreenResolution->addItem(formated_listing);
    }

    // add do not display again check box
   IGUICheckBox* cbDontDisplayAgain = GUIenv->addCheckBox(
        false,
        core::rect<s32>(10,95,90,110),
        guiWindow,
        108,
        L"Do not display again");

    // add label for status
    IGUIStaticText* lblStatus = GUIenv->addStaticText(
        L"status test",
        core::rect<s32>(152,95,232,110),
        false,
        false,
        guiWindow,
        109,
        false);

    // add okay button
   IGUIButton* bOK = GUIenv->addButton(
        core::rect<s32>(345,95,405,110),
        guiWindow,
        110,
        L"OK");

    // add apply button
   IGUIButton* bApply = GUIenv->addButton(
        core::rect<s32>(410,95,470,110),
        guiWindow,
        111,
        L"Apply");

    // add cancel button
   IGUIButton* bCancel = GUIenv->addButton(
        core::rect<s32>(475,95,535,110),
        guiWindow,
        112,
        L"Cancel");

    // don't let user resize the window
   device->setResizeAble(false);

    // display app title and version information
   device->setWindowCaption(L"irrPreSettings_Pure v1.0 build 061120");

    // obtain handle to video driver
   video::IVideoDriver* vidDriver = device->getVideoDriver();

    // obtain handle to scene manager
   scene::ISceneManager* sceneManager = device->getSceneManager();

    // add camera scene node
   sceneManager->addCameraSceneNodeMaya();

    // runs until device wants to be deleted and
    // video driver is valid
    while(device->run() && vidDriver)
    {
        // draw stuff only when the app window is active
        if(device->isWindowActive())
        {
            // setup the scene for rendering
            vidDriver->beginScene(
                true,
                false,
                video::SColor(150,50,50,50));

            // draws all the scene nodes
            sceneManager->drawAll();

            // draws all gui elements
            GUIenv->drawAll();

            // shows the rendered scene on screen
            vidDriver->endScene();
        }
    }

    // cleanup
   device->drop();

    // exit
    return 0;
} 
