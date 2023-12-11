#include "Events.h"

bool EventRec::OnEvent ( const SEvent &event )
{
	if ( event.EventType == EET_GUI_EVENT )
	{
		s32 cid = event.GUIEvent.Caller->getID();

		switch ( event.GUIEvent.EventType )
		{
			case EGET_BUTTON_CLICKED:
				switch ( cid )
				{
					case 101:
						quit = true;
						return true;
					case 102:
						login = true;
						return true;

					case 104:
						conRefOK = true;
						return true;

					case 105:
						wrongPassOK = true;
						return true;

					case 901:
						devLogin = true;
						return true;
				}
		}
	}
	else if ( ( login || devLogin ) )
	{
		if ( event.EventType == EET_MOUSE_INPUT_EVENT )
		{
			if ( camera != NULL )
				camera->OnEvent ( event );
		}
		else if ( event.EventType == EET_KEY_INPUT_EVENT )
		{
			if ( camera != NULL )
				camera->OnEvent ( event );
		}
	}


	return false;
}

EventRec::EventRec()
{
	quit = false;
	login = false;
	conRefOK = false;
	wrongPassOK = false;
	devLogin = false;
	lmouse = false;
	rmouse = false;

	mousex = 0;
	mousey = 0;

	camera = NULL;
}


void EventRec::registerCamera ( CCameraSceneNode *cam )
{
	camera = cam;
}

