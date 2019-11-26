void buttonEvents(const SEvent& event)
{
     IGUIEnvironment* env = Device->getGUIEnvironment();
     s32 id = event.GUIEvent.Caller->getID();
     switch(id)
            {
            case 1101:
               {
                  // set scale
                  gui::IGUIElement* root = env->getRootGUIElement();
                  core::vector3df scale;
                  core::stringc s;

                  s = root->getElementFromId(901, true)->getText();
                  scale.X = (f32)atof(s.c_str());
                  s = root->getElementFromId(902, true)->getText();
                  scale.Y = (f32)atof(s.c_str());
                  s = root->getElementFromId(903, true)->getText();
                  scale.Z = (f32)atof(s.c_str());

                  if (Model)
                     {Model->setScale(scale);

                     /*IMeshManipulator * manip = Device->getSceneManager()->getMeshManipulator();
                     manip->scaleMesh(ModelMesh->getMesh(0),core::vector3df(4.0f,4.0f,4.0f));
                     manip->recalculateNormals(ModelMesh->getMesh(0),true);
                     Model->remove();
                     Model=Device->getSceneManager()->addAnimatedMeshSceneNode(ModelMesh);*/
                     }
               }
               break;
            case 11010:
               {
                  // set translate
                  gui::IGUIElement* root = env->getRootGUIElement();
                  core::vector3df translate;
                  core::stringc s;

                  s = root->getElementFromId(904, true)->getText();
                  translate.X = (f32)atof(s.c_str());
                  s = root->getElementFromId(905, true)->getText();
                  translate.Y = (f32)atof(s.c_str());
                  s = root->getElementFromId(906, true)->getText();
                  translate.Z = (f32)atof(s.c_str());

                  if (Model) {
                     printf("translating to new position\n");
                     Model->setPosition( translate );
                  }
               }
               break;
            case 11011:
               {
                  // set rotate
                  gui::IGUIElement* root = env->getRootGUIElement();
                  core::vector3df rotatn;
                  core::stringc s;

                  s = root->getElementFromId(907, true)->getText();
                  rotatn.X = (f32)atof(s.c_str());
                  s = root->getElementFromId(908, true)->getText();
                  rotatn.Y = (f32)atof(s.c_str());
                  s = root->getElementFromId(909, true)->getText();
                  rotatn.Z = (f32)atof(s.c_str());

                  if (Model) {
                     printf("rotating to new position\n");
                     Model->setRotation( rotatn );
                  }
               }
               break;
            case 1102:
               loading=true;
               //env->addFileOpenDialog(L"Please select a model file to open");
               fileRequest.draw(L"Please select a model file to open",false,false,true,false);
               break;
            case 1103:
               showAboutText();
               break;
            case 1104:
               createToolBox();
               break;
            case 1105:
               loading=true;
               //env->addFileOpenDialog(L"Please select your game archive/directory");
               fileRequest.draw(L"Please select a model texture to open",true,false,false,false);
               break;
            case 1106:
               if (moveMDL->isPressed())
                 {viewp.moveitem=true; viewp.rotateitem=false; viewp.scaleitem=false; viewp.stretchitem=false;
                  rotaMDL->setPressed(false);
                  scalMDL->setPressed(false); strcMDL->setPressed(false);
                  displayMove(Model);}
                 else
                 {viewp.moveitem=false;}
               break;
            case 1107:
               if (rotaMDL->isPressed())
                 {viewp.moveitem=false; viewp.rotateitem=true; viewp.scaleitem=false; viewp.stretchitem=false;
                 moveMDL->setPressed(false);
                 scalMDL->setPressed(false); strcMDL->setPressed(false);
                 displayRotate(Model);}
                  else
                 {viewp.rotateitem=false;}
               break;
            case 1108:
               if (scalMDL->isPressed())
                 {viewp.moveitem=false; viewp.rotateitem=false; viewp.scaleitem=true; viewp.stretchitem=false;
                  moveMDL->setPressed(false); displayScale(Model);
                  rotaMDL->setPressed(false); strcMDL->setPressed(false);}
                  else
                 {viewp.scaleitem=false;}
               break;
             case 1109:
               if (strcMDL->isPressed())
                 {viewp.moveitem=false; viewp.rotateitem=false; viewp.scaleitem=false; viewp.stretchitem=true;
                  moveMDL->setPressed(false); displayScale(Model);
                  scalMDL->setPressed(false); rotaMDL->setPressed(false);}
                  else
                 {viewp.stretchitem=false;}
               break;
            case 1110:
                 if (viewp.scaleitem && Model) {Model->setScale(vector3df(1,1,1)); displayScale(Model);}
                 if (viewp.moveitem && Model) {Model->setPosition(vector3df(0,0,0)); displayMove(Model);}
                 if (viewp.rotateitem && Model) {Model->setRotation(vector3df(0,0,0)); displayRotate(Model);}
                 break;

            case 1111: if (Model) {Model->setVisible(!Model->isVisible()); // Hide/display selected model
                        if (!Model->isVisible())
                          {
                             selList->setItemOverrideColor(selNode,EGUI_LBC_TEXT ,SColor(255,180,64,64));
                             selList->setItem(selNode,selList->getListItem(selNode),selList->getIcon(selNode)+1);
                          } else
                          {  selList->setItemOverrideColor(selNode,EGUI_LBC_TEXT ,SColor(255,64,64,64));
                             selList->setItem(selNode,selList->getListItem(selNode),selList->getIcon(selNode)-1);
                          }
                        }
                       break;
            case 1112: if (Model)
                            {Model->setMaterialFlag(EMF_WIREFRAME,true);
                             selList->setItemOverrideColor(selNode,EGUI_LBC_TEXT ,SColor(255,64,220,128));
                             selList->setItem(selNode,selList->getListItem(selNode),selList->getIcon(selNode)+2);
                            }
                       break;
            case 1113: if (Model)
                            {Model->setMaterialFlag(EMF_WIREFRAME,false);
                             selList->setItemOverrideColor(selNode,EGUI_LBC_TEXT ,SColor(255,64,64,64));
                             selList->setItem(selNode,selList->getListItem(selNode),selList->getIcon(selNode)-2);
                            }
                       break;
            case 1200:
                       loadModel(fileRequest.getFilename().c_str()); loading=false;
                       break; // get the filename and launch the fileload
            case 1201:
                       fileRequest.closeLoader(); loading=false;
                       break; // Close the file selector
            case 1202:
                       loading=false;
                       printf ("The windows as been closed!!!");
                       break; // Close the file selector

            case 9000:
            {
              loadModel("powercell/tools/PlayerStart.obj");
            } break;
            case 9001:
            {
              loadModel("powercell/tools/PowerCell.obj");
            } break;
            case 9002:
            {
              loadModel("powercell/tools/HealthCell.obj");
            } break;

            case 64: // reset the animation for the model
                    { if (animated && Model) {Model->setFrameLoop(startAnim,endAnim);}
                    }
               break;
            case 66:  if (animated && Model) {startAnim=(int)Model->getFrameNr(); refreshAnimWin();}
               break;
            case 68:  if (animated && Model) {endAnim=(int)Model->getFrameNr(); refreshAnimWin();}
               break;
            case 70:
                  if (animated && Model)
                     {Model->setFrameLoop(0,(ModelMs->getFrameCount()-1));
                     }
               break;
        }
}
