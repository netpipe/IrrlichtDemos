#include "InGameEventReceiver.h"

InGameEventReceiver::InGameEventReceiver()
{
  for(s32 i = 0; i < KEY_KEY_CODES_COUNT; i++)
    keys[i] = false;

	Quit = false;

    lmouse = false;
	rmouse = false;

	//mousex = 0;
	//mousey = 0;
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

		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
    if     (event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN) LEFTBUTTONCLICKED = true;
    else if(event.MouseInput.Event==EMIE_LMOUSE_LEFT_UP     ) LEFTBUTTONCLICKED = false;
    else if(event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN) RIGHTBUTTONCLICKED = true;
    else if(event.MouseInput.Event==EMIE_RMOUSE_LEFT_UP     ) RIGHTBUTTONCLICKED = false;
		}



  return false;
}

bool InGameEventReceiver::getKeyState(EKEY_CODE key)
{
  return keys[key];
}
