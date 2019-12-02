/// ==============================
/// MyEventReceiver
/// ==============================
class MyEventReceiver : public IEventReceiver
{

   public:
   // If you want each of the events
   bool getKeyboardEvents;
   bool getMouseEvents;
   bool getEnterPress;
   virtual bool OnEvent(const SEvent& event)
	{ 
		//getevents 1 = full keyboard
		//getevents 3 = full keyboard
		//getevents 4 = enter only
		if(getevents==1||getevents==3){
			//KEYBOARD EVENTS
			if(event.EventType == irr::EET_KEY_INPUT_EVENT)
			{
				keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
				return true;
			}
		}
		if(getevents==4)
		{
			if(event.EventType == irr::EET_KEY_INPUT_EVENT&&event.KeyInput.Key==13)
			{
				keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
				return true;
			}
		}
		//MOUSE EVENTS
		//getevents 2 = full mouse
		//getevents 3 = full mouse
		//waitformouse 0 = full mouse
		//waitformouse 1 = position
		//waitformouse 2 = wheel
		//waitformouse 4 = MMB

		//waitformouse 5 = RMB, LMB
	if(getevents==2||getevents==3)
	{
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
            //Mouse changed position
            if(waitformouseevent==1||waitformouseevent==0)
			{
				if (event.MouseInput.Event == EMIE_MOUSE_MOVED)
				{
				   mouse.Y = event.MouseInput.Y;
				   mouse.X = event.MouseInput.X;
				}
			}
            //Wheel moved.
			if(waitformouseevent==2||waitformouseevent==0)
			{
				if (event.MouseInput.Event == EMIE_MOUSE_WHEEL)
				{
				   mouse.wheel += event.MouseInput.Wheel;
				}
			}

            //Left Mouse Button Pressed
			if(waitformouseevent==5||waitformouseevent==0)
			{
				if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
				{
				   //
				   if (mouseButtonState[0] == 0 || mouseButtonState[0] == 3)
				   {
					  mouseButtonState[0] = 1;
				   }
				   else
				   {
					  mouseButtonState[0] = 2;
				   }
				}
				//Left Mouse Button Rleased
				if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
				{
				   //
				   if (mouseButtonState[0] != 0)
				   {
					  mouseButtonState[0] = 3;
					  waitformouseevent=0;
				   }
				}
			}

            //Middle Mouse Button Pressed
			if(waitformouseevent==4||waitformouseevent==0){
            if (event.MouseInput.Event == EMIE_MMOUSE_PRESSED_DOWN)
            {
               //
               if (mouseButtonState[1] == 0 || mouseButtonState[1] == 3)
               {
                  mouseButtonState[1] = 1;
               }
               else
               {
                  mouseButtonState[1] = 2;
               }
            }

            //Middle Mouse Button Rleased
            if (event.MouseInput.Event == EMIE_MMOUSE_LEFT_UP)
            {
               //
               if (mouseButtonState[1] != 0)
               {
                  mouseButtonState[1] = 3;
				  waitformouseevent=0;
               }
            }
			}

			//Right Mouse Button Pressed
			if(waitformouseevent==5||waitformouseevent==0){
            if (event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
            {
				
               //
               if (mouseButtonState[2] == 0)// || mouseButtonState[2] == RELEASED)
               {
                  mouseButtonState[2] = 1;
               }
               else
               {
                  mouseButtonState[2] = 2;
               }
            }

            //Right Mouse Button Rleased
            if (event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
            {
                  mouseButtonState[2] = 0;
					waitformouseevent=0;
            }
			}
      
      return false;
   } 
}
	

	  //gui events
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			//IGUIEnvironment* env = device->getGUIEnvironment();
			switch(event.GUIEvent.EventType)
			{
				case EGET_BUTTON_CLICKED:
					if(id==2)
					{
						//quit
						runloop=false;
					}
					if(id==6)
					{
						//launch rofh.exe, used in patcher to play
						ShellExecute(GetDesktopWindow(), "open", "rofh.exe", NULL, NULL, SW_SHOWNORMAL);
						runloop=false;
					}
					
				break;
			}
		}
		return false;
	}   
};