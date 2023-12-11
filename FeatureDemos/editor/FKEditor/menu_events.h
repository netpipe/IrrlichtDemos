void menuEvents(const SEvent& event)
{
     // a menu item was clicked
    IGUIEnvironment* env = Device->getGUIEnvironment();
    IGUIContextMenu* menu = (IGUIContextMenu*)event.GUIEvent.Caller;
    s32 id = menu->getItemCommandId(menu->getSelectedItem());
       switch(id)
       {
       case 98: // load the default model
            Device->getFileSystem()->changeWorkingDirectoryTo(appPath.c_str());
            Device->getFileSystem()->addFolderFileArchive ( "media/" );
            loadModel(StartUpModelFile.c_str());
            break;
       case 99: // clear the current model
            if (Model)
                {
                 Model->remove(); //ModelMesh[selNode]=0;
                 Model = 0; ThePoly = 0;
                 int oldsel = selNode;
                 //moveviews=false; moveMDL->setPressed(false);
                 selList->clear(); selNode=0; lastNode=0; fakecounter=0;
                 SceneAnalys(Device->getSceneManager()->getRootSceneNode());
                 selList->setSelected(oldsel);
                 //removeItemfromList(selNode,lastNode+1);  // Remove the item from the selection list
                 //createSelList();                         // Recreate the selection list gui
                }
            break;
       case 100: // File -> Open Model
                  loading=true;
                  //env->addFileOpenDialog(L"Please select a model file to open");
                  fileRequest.draw(L"Pleases select a model file to open",false,false,true,false);
                  break;
               case 101: // File -> Set Model Archive
                  loading=true;
                  fileRequest.draw(L"Please select a model texture to open",true,false,false,false);
                  //env->addFileOpenDialog(L"Please select your game archive/directory");
                  break;
               case 200: // File -> Quit
                  Device->closeDevice();
                  break;
               case 300: // View -> Skybox
                  viewp.SkyBox->setVisible(!viewp.SkyBox->isVisible());
                  menu->setItemChecked(6,viewp.SkyBox->isVisible());
                  break;
               case 301: // View -> Grid
                  if (viewp.gridvisibility) viewp.gridvisibility=false; else viewp.gridvisibility=true;
                  menu->setItemChecked(7,viewp.gridvisibility);
                  //grid->setVisible(!grid->isVisible());
                  break;
               case 302: // View -> Grid
                  img->setVisible(!img->isVisible());
                  menu->setItemChecked(11,img->isVisible());
               break;
               case 303: // View -> Ground
                  if (viewp.groundvisibility) viewp.groundvisibility=false; else viewp.groundvisibility=true;
                  menu->setItemChecked(8,viewp.groundvisibility);
                  //grid->setVisible(!grid->isVisible());
                  break;
               case 304: // View -> Grid
                  if (viewp.gridvispers) viewp.gridvispers=false; else viewp.gridvispers=true;
                  menu->setItemChecked(9,viewp.gridvispers);
                  //grid->setVisible(!grid->isVisible());
                  break;
               case 305:
                    { if (tab1->isVisible()) menu->setItemChecked(10,false); else menu->setItemChecked(10,true);
                      tab1->setVisible(!tab1->isVisible());
                      selList->setVisible(!selList->isVisible());
                      seltitle->setVisible(!seltitle->isVisible());
                      coords->setVisible(!coords->isVisible());
                      if (viewp.viewrightspace==180) viewp.viewrightspace=0; else viewp.viewrightspace=180;
                      if (tab1->isVisible()) vptopright->move(core::position2d< s32 >(-180,0)); else vptopright->move(core::position2d< s32 >(180,0));
                      if (tab1->isVisible()) vpbottomright->move(core::position2d< s32 >(-180,0)); else vpbottomright->move(core::position2d< s32 >(180,0));
                    }
                   break;
               case 410: // View -> Debug Information
                  if (Model)
                     Model->setDebugDataVisible(scene::EDS_BBOX);
                  break;
               case 420: // View -> Debug Information
                  if (Model)
                     Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible()^scene::EDS_NORMALS));
                  break;
               case 430: // View -> Debug Information
                  if (Model)
                     Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible()^scene::EDS_SKELETON));
                  break;
               case 440: // View -> Debug Information
                  if (Model)
                     Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible()^scene::EDS_MESH_WIRE_OVERLAY));
                  break;
               case 450: // View -> Debug Information
                  if (Model)
                     Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible()^scene::EDS_HALF_TRANSPARENCY));
                  break;
               case 460: // View -> Debug Information
                  if (Model)
                     Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible()^scene::EDS_BBOX_BUFFERS));
                  break;
               case 500: // Help->About
                  showAboutText();
                  break;
               case 510:
                    viewp.ground->setMaterialFlag(EMF_LIGHTING,true);
                    if (Model) {Model->setMaterialFlag(EMF_LIGHTING,true);}
                    break;
               case 520:
                    viewp.ground->setMaterialFlag(EMF_LIGHTING,false);
                    if (Model) {Model->setMaterialFlag(EMF_LIGHTING,false);}
                    break;
               case 530:
                    if (Model){ addShadow();}
                    break;
               case 540:
                    {removeShadow();}
                    break;
               case 552:
                    loading=true;
                    //env->addFileOpenDialog(L"Please select a IRR scene file to open");
                    fileRequest.draw(L"Please select an IRR scene file to open",false,true,false,false);
                    break;
               case 553:
                    loading=true;
                    fileRequest.loader=false;
                    fileRequest.draw(L"Saving the IRRScene",false,true,false,false);
                    break;
               case 554: // clear the current scene
                    Device->getSceneManager()->getRootSceneNode()->removeAll();
                    Model=0;
                    ModelMs=0;
                    Device->getFileSystem()->changeWorkingDirectoryTo(appPath.c_str());
                    viewp.createCameras();
                    createLights();
                    viewp.createSkyBox();
                    viewp.createBillboard();
                    viewp.createGround();
                    viewp.createGrid();
                    selList->clear();
                    lastNode=0; selNode=0; fakecounter=0;
                    SceneAnalys(Device->getSceneManager()->getRootSceneNode());
                    break;
               case 610: // View -> Material -> Solid
                  if (Model)
                     Model->setMaterialType(video::EMT_SOLID);
                  break;
               case 620: // View -> Material -> Transparent
                  if (Model)
                     Model->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
                  break;
               case 630: // View -> Material -> Reflection
                  if (Model)
                     Model->setMaterialType(video::EMT_SPHERE_MAP);
                  break;

               case 1000:
                  viewp.setActiveCamera ( viewp.Camera[0] );
                  break;
               case 1100:
                  viewp.setActiveCamera ( viewp.Camera[1] );
                  break;
               case 2000:  // enable single view
                  {viewp.viewports=false; viewp.currentview=0; moveMDL->setVisible(true); rotaMDL->setVisible(true); scalMDL->setVisible(true); strcMDL->setVisible(false);
                  vptopright->setText(L"Perspective view");
                  vptopleft->setVisible(false);
                  vpbottomleft->setVisible(false);
                  vpbottomright->setVisible(false);
                  menu->setItemChecked(0,true);
                  menu->setItemChecked(1,false);
                  menu->setItemChecked(2,false);
                  menu->setItemChecked(3,false);
                  menu->setItemChecked(5,false);

                  break;}
               case 2001:  // enable single view
                  viewp.viewports=true; viewp.currentview=2;
                  if (Model)
                     {moveMDL->setVisible(true); rotaMDL->setVisible(true);
                      scalMDL->setVisible(true); strcMDL->setVisible(true);}
                  vptopright->setText(L"Top view");
                  vptopleft->setVisible(false);
                  vpbottomleft->setVisible(false);
                  vpbottomright->setVisible(false);
                  menu->setItemChecked(0,false);
                  menu->setItemChecked(1,true);
                  menu->setItemChecked(2,false);
                  menu->setItemChecked(3,false);
                  menu->setItemChecked(5,false);
                  break;
               case 2002:  // enable single view
                  viewp.viewports=true; viewp.currentview=3;
                  if (Model)
                    {moveMDL->setVisible(true); rotaMDL->setVisible(true);
                     scalMDL->setVisible(true); strcMDL->setVisible(true);}
                  vptopright->setText(L"Front view");
                  vptopleft->setVisible(false);
                  vpbottomleft->setVisible(false);
                  vpbottomright->setVisible(false);
                  menu->setItemChecked(0,false);
                  menu->setItemChecked(1,false);
                  menu->setItemChecked(2,true);
                  menu->setItemChecked(3,false);
                  menu->setItemChecked(5,false);
                  break;
               case 2003:  // enable single view
                  viewp.viewports=true; viewp.currentview=4;
                  if (Model)
                      {moveMDL->setVisible(true); rotaMDL->setVisible(true);
                       scalMDL->setVisible(true); strcMDL->setVisible(true);}
                  vptopright->setText(L"Side view");
                  vptopleft->setVisible(false);
                  vpbottomleft->setVisible(false);
                  vpbottomright->setVisible(false);
                  menu->setItemChecked(0,false);
                  menu->setItemChecked(1,false);
                  menu->setItemChecked(2,false);
                  menu->setItemChecked(3,true);
                  menu->setItemChecked(5,false);
                  break;
               case 2010: // enable quad view
                  viewp.viewports=true; viewp.currentview=1;
                  if (Model)
                    {moveMDL->setVisible(true); rotaMDL->setVisible(true);
                     scalMDL->setVisible(true); strcMDL->setVisible(true);}
                  vptopright->setText(L"Perspective view");
                  vptopleft->setVisible(true);
                  vpbottomleft->setVisible(true);
                  vpbottomright->setVisible(true);
                  menu->setItemChecked(0,false);
                  menu->setItemChecked(1,false);
                  menu->setItemChecked(2,false);
                  menu->setItemChecked(3,false);
                  menu->setItemChecked(5,true);
                  break;
               case 2020: // Reset The views
                  viewp.resetViews(true);
                  break;
               case 2021: // Reset The views
                  viewp.resetViews(false);
                  break;
               case 2022: // Reset The views
                  {core::vector3df tgt=viewp.Camera[0]->getTarget();
                  viewp.Camera[2]->setPosition(core::vector3df(tgt.X,viewp.Camera[2]->getPosition().Y,tgt.Z));
                  viewp.Camera[2]->setTarget(core::vector3df(tgt.X,viewp.Camera[2]->getTarget().Y,tgt.Z));
                  viewp.Camera[3]->setPosition(core::vector3df(tgt.X,tgt.Y,viewp.Camera[3]->getPosition().Z));
                  viewp.Camera[3]->setTarget(core::vector3df(tgt.X,tgt.Y,viewp.Camera[3]->getTarget().Z));
                  viewp.Camera[4]->setPosition(core::vector3df(viewp.Camera[4]->getPosition().X,tgt.Y,tgt.Z));
                  viewp.Camera[4]->setTarget(core::vector3df(viewp.Camera[4]->getTarget().X,tgt.Y,tgt.Z));
                  printf ("Here the current target selected: %f,%f,%f \n",tgt.X,tgt.Y,tgt.Z);
                  printf ("Here the current cam selected: %f,%f,%f \n",viewp.Camera[2]->getPosition().X,viewp.Camera[2]->getPosition().Y,viewp.Camera[2]->getPosition().Z);
                  printf ("Here the current cam target: %f,%f,%f \n",viewp.Camera[2]->getTarget().X,viewp.Camera[2]->getTarget().Y,viewp.Camera[2]->getTarget().Z);
                  break;  }
               case 2023:
                    {
                     vector3df itemtarget=Model->getPosition();
                     viewp.Camera[2]->setPosition(vector3df(itemtarget.X,16000,itemtarget.Z));
                     viewp.Camera[2]->setTarget(vector3df(itemtarget.X,0,itemtarget.Z));
                     viewp.Camera[3]->setPosition(vector3df(itemtarget.X,itemtarget.Y,16000));
                     viewp.Camera[3]->setTarget(vector3df(itemtarget.X,itemtarget.Y,0));
                     viewp.Camera[4]->setPosition(vector3df(-16000,itemtarget.Y,itemtarget.Z));
                     viewp.Camera[4]->setTarget(vector3df(0,itemtarget.Y,itemtarget.Z));
                     vector3df diff = viewp.Camera[0]->getTarget()-Model->getPosition();
                     viewp.Camera[0]->setTarget(Model->getPosition());
                     viewp.Camera[0]->setPosition(viewp.Camera[0]->getPosition()-diff);

                    }
                    break;
               case 2024: // Position the model in the center of the perspective view
                    { Model->setPosition(viewp.Camera[0]->getTarget());
                    }
               case 2098:
                     {if (Model)
                     Model->setFrameLoop(0,(ModelMs->getFrameCount()-1));
                     break;}
               case 2099:
                    { if (Model) {Model->setFrameLoop(startAnim,endAnim);}
                     break;}
               case 2100:
                    if (Model) Model->setAnimationSpeed(0);
                    break;
               case 2101:
                    if (Model) Model->setAnimationSpeed(6);
                    break;
               case 2102:
                    if (Model) Model->setAnimationSpeed(12);
                    break;
               case 2103:
                    if (Model) Model->setAnimationSpeed(24);
                    break;
               case 2104:
                    if (Model) Model->setAnimationSpeed(30);
                    break;
               case 2105:
                    if (Model) Model->setAnimationSpeed(-6);
                    break;
               case 2106:
                    if (Model) Model->setAnimationSpeed(-12);
                    break;
               case 2107:
                    if (Model) Model->setAnimationSpeed(-24);
                    break;
               case 2108:
                    if (Model) Model->setAnimationSpeed(-30);
                    break;
               case 6001:
                  {createToolBox();tab->setActiveTab(0);}
                  break;
               case 6002:
                  createAnimationToolBox();
                  break;
               case 6004:
                  {createToolBox();tab->setActiveTab(0);}
               break;
               case 6005:
                  {createToolBox();tab->setActiveTab(1);}
               break;
               }

}
