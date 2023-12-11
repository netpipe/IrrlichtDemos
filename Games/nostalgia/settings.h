
#ifndef _SETTINGS_H_INCLUDED
#define _SETTINGS_H_INCLUDED
#include <irrlicht.h>
//#include <irrKlang.h>
//#ifndef _CONSTANTS_H_INCLUDED
#include "constants.h"
//#endif //CONSTANTS INCLUDED
//using namespace irr;

void load_settings();//loads settings on start up
void change_settings();//interface to modify settings
void modify_settings(irr::s32);//apply newe settings to param Irr creation struct
void save_settings(irr::SIrrlichtCreationParameters);//write settings to file
void save_parameters();
void load_parameters();

/*class GUIEventReceiver : public IEventReceiver
{
public:
   virtual bool OnEvent(const SEvent & event)
   {
      return false;
   }
};*/

#endif //settings INCLUDED
