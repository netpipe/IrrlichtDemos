void comboEvents(const SEvent& event)
{
     IGUIEnvironment* env = Device->getGUIEnvironment();
     s32 id = event.GUIEvent.Caller->getID(); 
     // control anti-aliasing/filtering 
     if (id == 108) 
        { 
         s32 pos = ((IGUIComboBox*)event.GUIEvent.Caller)->getSelected(); 
         switch (pos) 
         { 
           case 0: 
           if (Model) 
              { 
                Model->setMaterialFlag(video::EMF_BILINEAR_FILTER, false); 
                Model->setMaterialFlag(video::EMF_TRILINEAR_FILTER, false); 
                Model->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, false); 
               } 
           break; 
           case 1: 
           if (Model) 
              { 
               Model->setMaterialFlag(video::EMF_BILINEAR_FILTER, true); 
               Model->setMaterialFlag(video::EMF_TRILINEAR_FILTER, false); 
               } 
           break; 
           case 2: 
           if (Model) 
              { 
              Model->setMaterialFlag(video::EMF_BILINEAR_FILTER, false); 
              Model->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true); 
              } 
           break; 
           case 3: 
           if (Model) 
              { 
              Model->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); 
              } 
           break; 
           case 4: 
           if (Model) 
              { 
              Model->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, false); 
              } 
           break; 
        }
     }
     if (id==900)
     { s32 pos = ((IGUIComboBox*)event.GUIEvent.Caller)->getSelected();
       fileRequest.comboAction(pos);
     }
      
}
