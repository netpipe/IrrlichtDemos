class gamegui
{
      public :
             
             IGUITab* optTab;
             IGUITab* info;
             IGUITab* systeme;
             IGUITab* aboutTab;
             IGUICheckBox* ws;
             IGUICheckBox* sm;
             IGUICheckBox* tl;
             IGUICheckBox* sl;
             
             gui::ICursorControl* cursorcontrol;
             gui::IGUIEnvironment* Gui;
             gui::IGUITabControl* tabctrl;
             
             bool walksim, smooth, tilt, slide;
             
             int currentX, currentY;
             bool gamemode;
             
             bool status;
             core::stringw  test;
             u32 lastime;

////////////////////////////////////////////////////////////////////////////////
// Create the gui panel
// 
////////////////////////////////////////////////////////////////////////////////             
      void createGui(s32 leftX, s32 leftY)
       {
           Gui = device->getGUIEnvironment();
           tabctrl = Gui->addTabControl(core::rect<int>(0,0,512-10,390),0, true, true);
           tabctrl->move(position2d<s32>(800,leftY));
	       systeme = tabctrl->addTab(L"Demo settings");
           optTab = tabctrl->addTab(L"How to use");
	       //info = tabctrl->addTab(L"info");
	       //aboutTab = tabctrl->addTab(L"About");
	       
	       wchar_t* text3 = 
           L"--- Movements:  \n"\
		   L"Mouse movements\n"\
		   L"Keys (W) and (S)\n"\
	       L"Keys (A) and (D)\n"\
	       L"Keys (Q) and (E)\n"\
		   L"(SPACE) KEY\n"\
		   L"(C) KEY\n"\
		   L"(CTRL) KEY\n"\
		   L"(CAP LOCK) KEY\n"\
		   L"(SHIFT) KEY\n\n"\
		   L"--- Game: \n"\
		   L"(TAB) KEY\n"\
		   L"(ESC) KEY";
	       Gui->addStaticText(text3, core::rect<int>(10, 10, 140, 270), false, true, optTab); 
	       
	       wchar_t* text31 = 
	       L"\n"\
		   L"Change the view direction\n"\
		   L"Move forward/Backward \n"\
	       L"Strafe left/right \n"\
	       L"Tilt view left,right \n"\
		   L"Jump \n"\
		   L"Crouch on the knee, flat on ground\n"\
		   L"Crouch on the knee only\n"\
		   L"Run / Walk mode\n"\
		   L"Sprint \n\n"\
		   L"\n"\
		   L"Show/Hide this menu\n"\
		   L"Quit the demo";
	       Gui->addStaticText(text31, core::rect<int>(145, 10, 400, 270),false, true, optTab); 
	       wchar_t* text32 =  
           L"---Please note:\n"\
		   L"This level is only a test level. It will not be included in the final game\n"\
		   L"The level in the game will occur in Egypt.";
	       Gui->addStaticText(text32, core::rect<int>(10, 270, 500, 400),false, true, optTab); 
	       
	        wchar_t* text2 =  
	        L"Specific camera rig settings";
	        Gui->addStaticText(text2, core::rect<int>(10, 10, 500, 80),false, true, systeme); 
	       ws=Gui->addCheckBox(walksim, core::rect<int>(20,85,280,110),systeme, 1, L"Enable walksim");
	       sm=Gui->addCheckBox(smooth, core::rect<int>(20,110,240,135),systeme, 2, L"Enable move smoothing");
	       tl=Gui->addCheckBox(tilt, core::rect<int>(250,85,400,110),systeme, 3, L"Enable tilt on strafe");
	       sl=Gui->addCheckBox(slide, core::rect<int>(250,110,490,135),systeme, 3, L"Slide on stop (smooth enabled)");
	        wchar_t* text21 =  
           L"---Please note:\n"\
		   L"There are more GUI components to implement. (setting values for theses functions. Functions like sprint exaustion delay, etc)\n";
		   
	       Gui->addStaticText(text21, core::rect<int>(10, 270, 500, 400),false, true, systeme); 
       }       
       
////////////////////////////////////////////////////////////////////////////////
// Animate the GUI panel (in/out) 
// 
////////////////////////////////////////////////////////////////////////////////       
      void animate(u32 timeMS, int targetX, int targetY, bool movein)
       
      {
           s32 x=50; s32 y=20; 
           u32 currentime=device->getTimer()->getRealTime();
           float curx1=(currentX/25);
           int curx=int(curx1);
           if (!movein) {tabctrl->move(position2d<s32>(curx,0)); currentX+=curx;} 
           if (movein) {tabctrl->move(position2d<s32>(-curx,0)); currentX-=curx;} 
           if ((movein) && (currentX<=targetX)) {status=false;} 
           if ((!movein) && (currentX>=targetX)) {status=false; tabctrl->setVisible(false);} 
           //if (movein) printf ("movein value is true \n"); else printf ("movein value is false \n");
           //printf ("Here is the current target: %i, %i, index: %f, %i\n",targetX,currentX,curx1,curx);
           //if (status) printf("The status is on\n"); else printf("The status is off\n");
           //if (!direction) {;tabctrl->setVisible(false);}
           //tabctrl->draw();
           lastime=currentime;
      }
////////////////////////////////////////////////////////////////////////////////
// Initialize the GUI 
// 
////////////////////////////////////////////////////////////////////////////////      
      void Init()
      {
           //IrrlichtDevice * intro = createDevice(driverType, core::dimension2d<s32>(512, 400), 32, false, true, false);
           video::IVideoDriver* driver = device->getVideoDriver();
           // test
           driver->setTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_QUALITY , true);
	       driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	       // test
	
           scene::ISceneManager* smgr = device->getSceneManager();
	       // set new Skin
           Gui = device->getGUIEnvironment();
           gui::IGUIInOutFader* fader = Gui->addInOutFader();
           //IGUISkin* skin = gui->createSkin(EGST_BURNING_SKIN);
	       //gui->setSkin(skin);
	       //skin->drop();
	       gui::IGUISkin *skin = Gui->createSkin(EGST_WINDOWS_METALLIC);
	       Gui->setSkin(skin);
	       // change transparency of skin
	       for (s32 i=0; i<EGDC_COUNT ; ++i)
	       {
		       video::SColor col = Gui->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
		       col.setAlpha(200);
		       Gui->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
	        }
	        //IGUISkin* skin = gui->getSkin();
	        gui::IGUIFont* font = Gui->getFont("levels/media/fontlucida.png");
	        if (font) skin->setFont(font);
	        tabctrl=0;
	        optTab=0;
	        systeme=0;
            tilt=false;
            walksim=false;
            smooth=false;
            slide=false;	        
            currentX=800; currentY=20;
	        createGui(currentX,currentY);
	        tabctrl->setVisible(false);
	        status=false;
	        test=L"This is the string\n";
	        gamemode=false;
	        //printf ("Here in the gui init loop\n");
	        
	        
       }
      
       gui::IGUIEnvironment* getGui()
       { return device->getGUIEnvironment();}
       
       gui::IGUITabControl* gettabctrl()
       { return tabctrl;}

////////////////////////////////////////////////////////////////////////////////
// Display the GUI (Will start the animation depending of the input)
// Also Hide/Show the mouse cursor
////////////////////////////////////////////////////////////////////////////////
       void displayGui(bool value)
       {    
             int objX=320; 
             bool direction=true;
             //currentY=tabctrl->getAbsolutePosition().Y;
             //printf ("Current pos: %i,%i \n",currentX,currentY);
             bool vis = tabctrl->isVisible();
             if ((vis) && (!value)) 
                {direction=false; status=true; objX=800; device->getCursorControl()->setVisible(false); }
             if ((!vis) && (value))
                {direction=true; status=true; objX=320; device->getCursorControl()->setVisible(true); tabctrl->setVisible(true);}
             if (status) {animate(device->getTimer()->getRealTime(),objX,currentY,direction);}
            //printf("The current objx= %i\n",objX);
       }
////////////////////////////////////////////////////////////////////////////////
// Change mode: change the rigs mode
// Modes= Walksim, Smoothing, Tilt, etc.
////////////////////////////////////////////////////////////////////////////////
       void change(bool wss, bool sms, bool tls, bool sls)
       {  
           ws->setChecked(tls); walksim=tls;
           sm->setChecked(sms); smooth=sms;
           tl->setChecked(tls); tilt=tls;
           sl->setChecked(sls); slide=sls; if (sm) sl->setEnabled(true);else (sl)->setEnabled(false);
        }
       bool update()
       {
       //if (status) animate(device->getTimer()->getRealTime(),50,20);
       displayGui(gamemode);
       if (!ws->isChecked() && walksim) {walksim=false; return true;}
       if (ws->isChecked() && !walksim) {walksim=true; return true;}
       if (!sm->isChecked() && smooth) {smooth=false; return true;}
       if (sm->isChecked() && !smooth) {smooth=true; return true;}
       if (!tl->isChecked() && tilt) {tilt=false; return true;}
       if (tl->isChecked() && !tilt) {tilt=true; return true;}
       if (!sl->isChecked() && slide) {slide=false; return true;}
       if (sl->isChecked() && !slide) {slide=true; return true;}
       if (!sm->isChecked()) sl->setChecked(false);
       return false;
       }
};
