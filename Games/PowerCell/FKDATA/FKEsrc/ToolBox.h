

/* 
   The three following functions do several stuff used by the mesh viewer. 
   The first function showAboutText() simply displays a messagebox with a caption 
   and a message text. The texts will be stored in the MessageText and 
   Caption variables at startup. 
*/ 
void showAboutText() 
{ 
   // create modal message box with the text 
   // loaded from the xml file. 
   Device->getGUIEnvironment()->addMessageBox( 
      Caption.c_str(), MessageText.c_str()); 
        
} 
//
// Create the animation toolbar
//
void createAnimationToolBox()
{
   IGUIEnvironment* env = Device->getGUIEnvironment(); 
   IGUIElement* root = env->getRootGUIElement(); 
   IGUIElement* e = root->getElementFromId(6002, true); 
   if (e) e->remove(); 
   IGUIWindow* wnd = env->addWindow(core::rect<s32>(10,40,440,245), 
      false, L"Animation control", 0, 6002); 

   // create tab control and tabs 
   IGUITabControl* tab = env->addTabControl( 
      core::rect<s32>(2,20,400,200), wnd, true, true); 
      //core::rect<s32>(600,20,800,480), 0, true, true); 
   IGUITab* t1 = tab->addTab(L"Frames"); 
   IGUITab* t2 = tab->addTab(L"Properties");
   //IGUITab* t3 = tab->addTab(L""); 
   env->addButton(core::rect<s32>(10,20,180,35), t1, 64, L"Play defined anim");
   env->addButton(core::rect<s32>(200,20,380,35), t1, 66, L"Define Begin frame");
   env->addButton(core::rect<s32>(200,40,380,55), t1, 68, L"Define End Frame");
   env->addButton(core::rect<s32>(10,90,180,105), t1, 70, L"Play all anims");
   // Slider bar with frame meter
   env->addStaticText(L"Position to a specific frame:", core::rect<s32>(10,110,220,125), false, false, t1);
   AnimScroll = env->addScrollBar(true, core::rect<s32>(10,130,320,145),t1,62); 
   if (!Model) {AnimScroll->setMax(0);AnimScroll->setPos(0);} 
   if (Model && animated) 
      { 
        int lstfrm = ModelMs->getFrameCount()-1;
        int currentFRM = (int)Model->getFrameNr();
        AnimScroll->setMax(lstfrm*10);
        AnimScroll->setPos(currentFRM*10);
        core::stringw frms=L"";
        frms.append((core::stringw)currentFRM);
        frms+=L"/";
        frms.append((core::stringw)lstfrm);
        animframes = env->addStaticText(frms.c_str(), core::rect<s32>(330,120,390,135), true, false, t1);
        core::stringw seqf=L"Begin: ";
        seqf.append((core::stringw)startAnim);
        seqf+=L" / End:";
        seqf.append((core::stringw)endAnim);
        animseq = env->addStaticText(L"Begin: 0 / End:0", core::rect<s32>(200,2,380,19), true, false, t1);
        } else if (Model && !animated)
        {AnimScroll->setMax(0);
         AnimScroll->setPos(0);
         animframes = env->addStaticText(L"Not animated", core::rect<s32>(330,120,390,135), true, false, t1);
         animseq = env->addStaticText(L"Begin: 0 / End:0", core::rect<s32>(200,2,380,19), true, false, t1);
        }
 
}


/* 
   Finally, the third function creates a toolbox window. In this simple mesh viewer, 
   this toolbox only contains a tab control with three edit boxes for changing 
   the scale of the displayed model. 
*/ 
void createToolBox() 
{ 
   // remove tool box if already there 
   IGUIEnvironment* env = Device->getGUIEnvironment(); 
   IGUIElement* root = env->getRootGUIElement(); 
   IGUIElement* e = root->getElementFromId(5000, true); 
   if (e) e->remove(); 
   
   // create the toolbox window 
   IGUIWindow* wnd = env->addWindow(core::rect<s32>(10+550,40,230+550,600), 
      false, L"Toolset", 0, 5000); 
   // create tab control and tabs 
   tab = env->addTabControl( 
      core::rect<s32>(2,20,800-602,580-27), wnd, true, true); 
      //core::rect<s32>(600,20,800,480), 0, true, true); 
   //IGUITab* t3 = tab->addTab(L"Selections"); 
   IGUITab* t1 = tab->addTab(L"Modify");
   IGUITab* t2 = tab->addTab(L"Surfaces");
   // add some edit boxes and a button to tab one 
   env->addStaticText(L"Scale model:", core::rect<s32>(10,10,180,30), false, false, t1);
   core::stringw sclX="1.0",sclY="1.0",sclZ="1.0";
   if (Model)
   {
     sclX=(core::stringw)Model->getScale().X;
     sclY=(core::stringw)Model->getScale().Y;
     sclZ=(core::stringw)Model->getScale().Z;
   }
   env->addEditBox(sclX.c_str(), core::rect<s32>(10,30,180,50), true, t1, 901); 
   env->addEditBox(sclY.c_str(), core::rect<s32>(10,60,180,80), true, t1, 902); 
   env->addEditBox(sclZ.c_str(), core::rect<s32>(10,90,180,110), true, t1, 903); 

   env->addButton(core::rect<s32>(10,120,180,140), t1, 1101, L"set"); 
   
    // add some edit boxes and a button to tab one 
   env->addStaticText(L"Translate model:", core::rect<s32>(10,150,180,170), false, false, t1);
   core::stringw posX="0.0",posY="0.0",posZ="0.0";
   if (Model)
   {
     posX=(core::stringw)Model->getPosition().X;
     posY=(core::stringw)Model->getPosition().Y;
     posZ=(core::stringw)Model->getPosition().Z;
   }
   env->addEditBox(posX.c_str(), core::rect<s32>(10,170,180,190), true, t1, 904); 
   env->addEditBox(posY.c_str(), core::rect<s32>(10,200,180,220), true, t1, 905); 
   env->addEditBox(posZ.c_str(), core::rect<s32>(10,230,180,250), true, t1, 906); 
   env->addButton(core::rect<s32>(10,260,180,280), t1, 11010, L"set");
   
   // add some edit boxes and a button to tab one 
    env->addStaticText(L"Rotate model:", core::rect<s32>(10,290,180,310), false, false, t1);
    core::stringw rotX="0.0",rotY="0.0",rotZ="0.0";
    if (Model)
   {
     rotX=(core::stringw)Model->getRotation().X;
     rotY=(core::stringw)Model->getRotation().Y;
     rotZ=(core::stringw)Model->getRotation().Z;
   }
   env->addEditBox(rotX.c_str(), core::rect<s32>(10,310,180,330), true, t1, 907); 
   env->addEditBox(rotY.c_str(), core::rect<s32>(10,340,180,360), true, t1, 908); 
   env->addEditBox(rotZ.c_str(), core::rect<s32>(10,370,180,390), true, t1, 909); 
   env->addButton(core::rect<s32>(10,400,180,420), t1, 11011, L"set"); 

   // add senseless checkbox 
   env->addCheckBox(true, core::rect<s32>(10,430,200,445), t1, -1, L"Senseless Checkbox"); 

   // add undocumentated transparent control 
   env->addStaticText(L"Transparent Control:", core::rect<s32>(10,455,180,470), false, false, t1); 
   IGUIScrollBar* scrollbar = env->addScrollBar(true, core::rect<s32>(10,480,180,495), t1, 104); 
   scrollbar->setMax(255); 
   scrollbar->setPos(255); 

  // create a combobox with some senseless texts 

   gui::IGUIComboBox* box = env->addComboBox(core::rect<s32>(5,20,120,43), t2, 108); 
   box->addItem(L"No filtering"); 
   box->addItem(L"Bilinear"); 
   box->addItem(L"Trilinear"); 
   box->addItem(L"Anisotropic"); 
   box->addItem(L"Isotropic"); 
   // bring irrlicht engine logo to front, because it 
   // now may be below the newly created toolbox 
   //root->bringToFront(root->getElementFromId(666, true)); 
} 

void refreshAnimWin()
{
   IGUIEnvironment* env = Device->getGUIEnvironment(); 
   IGUIElement* root = env->getRootGUIElement(); 
   IGUIElement* e = root->getElementFromId(6002, true); 
   if (Model) displayNode(Model);
   if (e && animated) {
         if (ModelMs)
         {  int lstfrm = ModelMs->getFrameCount()-1;
            int currentFRM = (int)Model->getFrameNr();
            AnimScroll->setMax(lstfrm*10);
            AnimScroll->setPos(currentFRM*10);
            core::stringw frms=L"";
            frms.append((core::stringw)currentFRM);
            frms+=L"/";
            frms.append((core::stringw)lstfrm);
            if (animframes) animframes->setText(frms.c_str());
            core::stringw seqf=L"Begin: ";
            seqf.append((core::stringw)startAnim);
            seqf+=L" / End:";
            seqf.append((core::stringw)endAnim);
            if (animseq) animseq->setText(seqf.c_str());
            refreshanimwin=false;
            }
     } else if (e && !animated)
     {    AnimScroll->setMax(0);
          AnimScroll->setPos(0);
          animframes->setText(L"No animation");
     }
}

