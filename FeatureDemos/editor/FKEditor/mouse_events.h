void mouseEvents(const SEvent& event)
{
     IGUIEnvironment* env = Device->getGUIEnvironment();
     int result=viewp.tellViewport(event.MouseInput.X,event.MouseInput.Y);
     //if (result>1 && rotateitem && Model) {moveViewport(result,event.MouseInput.X,event.MouseInput.Y,true); displayRotate(Model);}
     
     switch(event.MouseInput.Event) 
         { 
            case EMIE_MOUSE_MOVED:
                {
                     if  ((viewp.validmove && mouserightpress))
                         { 
                           //if (result==0) Camera[0]->setInputReceiverEnabled(true); else Camera[0]->setInputReceiverEnabled(false);
                           if (result>0 && !viewp.moveitem && !viewp.rotateitem) {viewp.moveViewport(result,event.MouseInput.X,event.MouseInput.Y,false);}
                           if (result>-1 && viewp.moveitem && Model) {viewp.moveViewport(result,event.MouseInput.X,event.MouseInput.Y,true); displayMove(Model);}
                           if (result>-1 && viewp.rotateitem && Model) {viewp.moveViewport(result,event.MouseInput.X,event.MouseInput.Y,true); displayRotate(Model);}                         
                         }
                } 
                break;                            
            case EMIE_MOUSE_WHEEL: 
               { 
                  if (result==0 && !viewp.scaleitem && !loading)
                    { // Zoom the view with the mousewheel. 
                      float distance=viewp.Camera[0]->getPosition().getDistanceFrom(viewp.Camera[0]->getTarget());
                      if (event.MouseInput.Wheel >= 1 ) distance*=0.9f;
                      if (event.MouseInput.Wheel <= -1) distance*=1.1f;
                      vector3df pos = viewp.Camera[0]->getTarget()-viewp.Camera[0]->getPosition();
                      pos.setLength(-distance);
                      viewp.Camera[0]->setPosition(viewp.Camera[0]->getTarget()+pos);
                    }
                      
                  // Change the scale based on the mousewheel
                  if ((event.MouseInput.Wheel >=1) && (viewp.scaleitem) && !loading)
                  {
                    if (Model) 
                      {vector3df currentscale = Model->getScale(); Model->setScale(currentscale*1.1f);
                       displayScale(Model);
                      }
                  } else 
                  if ((event.MouseInput.Wheel >=-1) && (viewp.scaleitem) && !loading)
                  {
                    if (Model) 
                    {vector3df currentscale = Model->getScale(); Model->setScale(currentscale*0.9f);
                     displayScale(Model);
                    }
                  }                  
                                
                  // change the zoom ratio on the orthographic viewports 
                  if ((event.MouseInput.Wheel >= 1) && (viewp.viewports) && (!viewp.scaleitem) && !loading) 
                  {
                      if (result==2) viewp.viewscale2*=1.1f;
                      if (result==3) viewp.viewscale3*=1.1f;
                      if (result==4) viewp.viewscale4*=1.1f;
                  } 
                  if ((event.MouseInput.Wheel <= -1) && (viewp.viewports) && (!viewp.scaleitem) && !loading)
                  {
                      if (result==2) viewp.viewscale2*=0.9f; if (viewp.viewscale2<0.01f) viewp.viewscale2=0.01f;
                      if (result==3) viewp.viewscale3*=0.9f; if (viewp.viewscale3<0.01f) viewp.viewscale3=0.01f;
                      if (result==4) viewp.viewscale4*=0.9f; if (viewp.viewscale4<0.01f) viewp.viewscale4=0.01f;
                  }  
                  
                  //printf("Mouse Wheel is rotated! \n \n");   
               } 
               break; 
            case EMIE_RMOUSE_PRESSED_DOWN: 
               { 
                 if ((result==0 && viewp.moveitem) || (result==0 && viewp.rotateitem)) 
                   { viewp.perspalternate=true;
                     viewp.Camera[0]->setInputReceiverEnabled(false);
                     viewp.setOffset=true;
                     viewp.validmove=true;
                     mouserightpress=true;
                     viewp.validrotation=true;
                   }
               }
               break; 
            case EMIE_LMOUSE_PRESSED_DOWN: 
               { if (!loading)
                 {if ((result>1) || viewp.rotateitem || viewp.moveitem) viewp.Camera[0]->setInputReceiverEnabled(false);
                 //if (rotateitem || moveitem) Camera[0]->setInputReceiverEnabled(false);                          
                 if (selList->getSelected()!=selNode) getListNode();
                 mouserightpress=true; viewp.setOffset=true;
                 if (viewp.rotateitem && Model) viewp.validrotation=true;
                 //printf("The result of the destination is: %i\n",result);
                 if ((result>1) || (result==0)) viewp.validmove=true; else viewp.validmove=false;}
               } 
               break; 
            case EMIE_LMOUSE_LEFT_UP:
               {    if (selList->getSelected()!=selNode) getListNode();
                    mouserightpress=false; viewp.validmove=false; viewp.validrotation=false;
                    //if (Model) Model->setFrameLoop(0,ModelMesh->getFrameCount());               
                    // printf ("ok released the mouse button\n");
               }
               break;
            case EMIE_RMOUSE_LEFT_UP:
                 { 
                 if ((viewp.moveitem && viewp.perspalternate) || (viewp.rotateitem && viewp.perspalternate))
                     {viewp.perspalternate=false; viewp.setOffset=false;
                      viewp.validmove=false; mouserightpress=false;
                      viewp.validrotation=false;}
                 }
                 
         } 
}
