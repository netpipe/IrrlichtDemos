// hudIcons.cpp
///////////////

#include "hudIcons.h"

HudIcons::HudIcons(	IGUIEnvironment* guienv,IVideoDriver* driver)
{
   size = 32;
   spacer = 4;
   guienv = guienv;
   
   for(unsigned i = 0; i < 15; i++){
     int tmp = i + 1;
	 stringc str(L"Media/Gui/");
     str += tmp;
     str += "_ball.png"; 
     blist[i] = guienv->addImage(driver->getTexture(str.c_str()),position2d<s32>(spacer + ( i * size),spacer));
     blist[i]->setVisible(false);
   }
}

HudIcons::~HudIcons()
{
}

void HudIcons::SetIcons(bool billSunk[])
{
     unsigned j = 0;
     
     for(unsigned i = 0; i < 15; i++){
         if(billSunk[i] == false){
             blist[i]->setRelativePosition(rect<s32>(spacer + ( j * size),spacer,(spacer + size) + ( j * size),(spacer + size)));
             blist[i]->setVisible(true);    
             j++; 
         }else       
             blist[i]->setVisible(false);
     }
}
