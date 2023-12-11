#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EventReceiver : public IEventReceiver
{
private:
    bool keys[irr::KEY_KEY_CODES_COUNT];
    bool mouse[irr::EMIE_COUNT];

public:
    EventReceiver()
    {
    }

   bool OnEvent(const SEvent& event)
   {
      if(event.EventType == irr::EET_KEY_INPUT_EVENT)
      {
         //keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
        if(!event.KeyInput.PressedDown)
        {
            keys[event.KeyInput.Key] = false;
            return false;
        }
        else
            keys[event.KeyInput.Key] = true;

      }
	  if(event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	  {
		  mouse[event.MouseInput.Event] = 1;
	  }

      return false;
   }

   bool getKeyState(int key)
   {
       return keys[key];
   }

   void clearKeys()
   {
       for(int i=0;i<KEY_KEY_CODES_COUNT;i++)
       {
           keys[i]=false;
       }
   }
};

