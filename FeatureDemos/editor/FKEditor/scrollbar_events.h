void scrollbarEvents(const SEvent& event)
{
     IGUIEnvironment* env = Device->getGUIEnvironment();
     s32 id = event.GUIEvent.Caller->getID();
     // control skin transparency 
     if (id == 104) 
        { 
          s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos(); 
          for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i) 
            { 
             video::SColor col = env->getSkin()->getColor((EGUI_DEFAULT_COLOR)i); 
             col.setAlpha(pos); 
             env->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col); 
            } 
        } 
            
     if (id == 62) // scrolling for the current animation frame
        {
          if (animated) 
          {
          s32 pos = (((IGUIScrollBar*)event.GUIEvent.Caller)->getPos());
          //printf("Here is the current position on the scroller: %i\n",pos);
          changeAnimFrame(Model,(pos/10));
          int currentFRM = (int)Model->getFrameNr();
          core::stringw str=L"";
          str.append((core::stringw)currentFRM);
          str+="/";
          
          str.append((core::stringw)(int)(retrieveMesh(Model)->getFrameCount()-1));
          animframes->setText(str.c_str());
          //printf("Competed that part\n");
          }
          // if (Model) checkModel(Model);
        }
}            
