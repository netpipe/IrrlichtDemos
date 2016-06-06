
#ifndef RECEIVER_H
#define RECEIVER_H

#include "Globals.h"

class MyEventReceiver : public irr::IEventReceiver
{
public:
   virtual bool OnEvent(const SEvent& event)
   {
      if (event.EventType == EET_GUI_EVENT)
      {
         s32 id = event.GUIEvent.Caller->getID();
         device->getGUIEnvironment();

         switch(event.GUIEvent.EventType)
         {

         case EGET_BUTTON_CLICKED:

            if (id == 101)
            {
                    gameclock.setClockSpeed(1.0f);
                    gamespeed = 1.0f;
               return true;
            }

            if (id == 102)
            {
               gameclock.setClockSpeed(0.0f);
               return true;
            }

            if (id == 103)
            {
               gameclock.setClockSpeed(gamespeed);
               return true;
            }

                if (id == 104)
                {
                    if(gamespeed < 65535.0f)
                    {
                        gamespeed += 1.0f;
                        gameclock.setClockSpeed(gamespeed);
                    }
                    return true;
                }

                if (id == 105)
                {
                    if(gamespeed < 32768.0f)
                    {
                        gamespeed *= 2.0f;
                        gameclock.setClockSpeed(gamespeed);
                    }
                    return true;
                }

            if (id == 106)
            {
               return true;
            }

            if (id == 107)
            {
               return true;
            }

            break;
         default:
            break;
         }
      }

      return false;
   }
};

#endif
