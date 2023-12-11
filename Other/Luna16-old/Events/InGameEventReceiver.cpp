#include "InGameEventReceiver.h"

InGameEventReceiver::InGameEventReceiver()
{
  for(s32 i = 0; i < KEY_KEY_CODES_COUNT; i++)
    keys[i] = false;

	Quit = false;
}

bool InGameEventReceiver::OnEvent ( const SEvent &event )
{
  if(event.EventType == EET_KEY_INPUT_EVENT)
  {
    keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
  }
		if ( event.EventType == EET_MOUSE_INPUT_EVENT )
		{
			//if ( camera != NULL )
			//	camera->OnEvent ( event );
			//printf("event on the mouse button");
		}

  return false;
}

bool InGameEventReceiver::getKeyState(EKEY_CODE key)
{
  return keys[key];
}
