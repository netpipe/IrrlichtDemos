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
					if(id==1)
					{
						//login
						//return to login page after an error
						returnEnvId=1;
						//remove background
						//env->clear();
						if(connected==0){
							//if connected
							envId=5;
							buildNewEnv=true;
							pauseScreen=false;
							stringc username;
							stringc password;
							stringw boxtxt;
							boxtxt=globaltxtbox1->getText();
							convertString(username, boxtxt, 1);
							stringc loginclient="1|";
							loginclient.append(username);
							loginclient.append('|');
							boxtxt=globaltxtbox2->getText();
							convertString(password, boxtxt, 1);
							loginclient.append(password);
							loginclient.append('|');
							if(username.size()<=5||password.size()<=5)
							{
								envId=9;
								buildNewEnv=true;
								pauseLength=5000;
								pauseScreen=true;
							}else
							{
								send(serverSocket,loginclient.c_str(), loginclient.size(),0);
							}
						}else
						{
							connectToServer();
							if(connected==0)
							{
								//if connected
								envId=5;
								buildNewEnv=true;
								pauseScreen=false;
								stringc username;
								stringc password;
								stringw boxtxt;
								boxtxt=globaltxtbox1->getText();
								convertString(username, boxtxt, 1);
								stringc loginclient="1|";
								loginclient.append(username);
								loginclient.append('|');
								boxtxt=globaltxtbox2->getText();
								convertString(password, boxtxt, 1);
								loginclient.append(password);
								loginclient.append('|');
								if(username.size()<=5||password.size()<=5)
								{
									envId=9;
									buildNewEnv=true;
									pauseLength=5000;
									pauseScreen=true;
								}else
								{
									send(serverSocket,loginclient.c_str(), loginclient.size(),0);
								}
							}
						}
					}
					if(id==2)
					{
						//quit
						runloop=false;
					}
					if(id==3)
					{
						// to register page
						envId=6;
						buildNewEnv=true;
						pauseScreen=false;
						
					}
					if(id==4)
					{
						// to login page
						envId=1;
						buildNewEnv=true;
						pauseScreen=false;
						
					}
					if(id==5)
					{
						//register
						returnEnvId=6;
						//remove background
						//env->clear();
						if(connected==0){
							//if connected
							envId=7;
							buildNewEnv=true;
							pauseScreen=false;
							stringc username;
							stringc password;
							stringc email;
							stringw boxtxt;
							boxtxt=globaltxtbox1->getText();
							convertString(username, boxtxt, 1);
							stringc loginclient="2|";
							loginclient.append(username);
							loginclient.append('|');
							boxtxt=globaltxtbox2->getText();
							convertString(password, boxtxt, 1);
							loginclient.append(password);
							loginclient.append('|');
							boxtxt=globaltxtbox4->getText();
							convertString(email, boxtxt, 1);
							loginclient.append(email);
							loginclient.append('|');
							stringw pass1, pass2;
							pass1=globaltxtbox2->getText();
							pass2=globaltxtbox3->getText();
							if(pass1!=pass2){
								//pws don't match
								envId=8;
								buildNewEnv=true;
								pauseLength=5000;
								pauseScreen=true;
							} else 
							{
								stringc username;
								stringc password;
								stringc email;
								stringw boxtxt;
								boxtxt=globaltxtbox1->getText();
								convertString(username, boxtxt, 1);
								stringc loginclient="2|";
								loginclient.append(username);
								loginclient.append('|');
								boxtxt=globaltxtbox2->getText();
								convertString(password, boxtxt, 1);
								loginclient.append(password);
								loginclient.append('|');
								boxtxt=globaltxtbox4->getText();
								convertString(email, boxtxt, 1);
								loginclient.append(email);
								loginclient.append('|');
								if(username.size()<3||password.size()<5||email.size()<3){
									envId=9;
									buildNewEnv=true;
									pauseLength=5000;
									pauseScreen=true;
								} else 
								{
									send(serverSocket,loginclient.c_str(), loginclient.size(),0);
								}
							}
						}else
						{
							//****************//
							//if not connected//
							//****************//
							connectToServer();
							if(connected==0)
							{
								//if connected
								envId=7;
								buildNewEnv=true;
								pauseScreen=false;
								stringc username;
								stringc password;
								stringc email;
								stringw boxtxt;
								boxtxt=globaltxtbox1->getText();
								convertString(username, boxtxt, 1);
								stringc loginclient="2|";
								loginclient.append(username);
								loginclient.append('|');
								boxtxt=globaltxtbox2->getText();
								convertString(password, boxtxt, 1);
								loginclient.append(password);
								loginclient.append('|');
								boxtxt=globaltxtbox4->getText();
								convertString(email, boxtxt, 1);
								loginclient.append(email);
								loginclient.append('|');
								stringw pass1, pass2;
								pass1=globaltxtbox2->getText();
								pass2=globaltxtbox3->getText();
								if(pass1!=pass2){
									//pws don't match
									envId=8;
									buildNewEnv=true;
									pauseLength=5000;
									pauseScreen=true;
								} else 
								{
									stringc username;
									stringc password;
									stringc email;
									stringw boxtxt;
									boxtxt=globaltxtbox1->getText();
									convertString(username, boxtxt, 1);
									stringc loginclient="2|";
									loginclient.append(username);
									loginclient.append('|');
									boxtxt=globaltxtbox2->getText();
									convertString(password, boxtxt, 1);
									loginclient.append(password);
									loginclient.append('|');
									boxtxt=globaltxtbox4->getText();
									convertString(email, boxtxt, 1);
									loginclient.append(email);
									loginclient.append('|');
									if(username.size()<=5||password.size()<=5||email.size()<=5)
									{
										envId=9;
										buildNewEnv=true;
										pauseLength=5000;
										pauseScreen=true;
									} else 
									{
										send(serverSocket,loginclient.c_str(), loginclient.size(),0);
									}
								}
							}
						}
					}
					if(id==6)
					{
						//launch rofh.exe, used in patcher to play
						ShellExecute(GetDesktopWindow(), "open", "rofh.exe", NULL, NULL, SW_SHOWNORMAL);
						runloop=false;
					}
					if(id==104)
					{
						printf("inventory");
						GUISystem.buildInventory();
					}
					if(id==105)
					{
						printf("cha");
					}
					if(id==106)
					{
						printf("quest");
					}
					if(id==107)
					{
						printf("skill");
					}
					if(id==108)
					{
						printf("friends");
					}
					if(id==109)
					{
						printf("settings");
					}
					//equipment 110-128
					//inventory 129-169
					if(id>=129&&id<=169)
					{
						//save what slot is selected, to be used when use is clicked
						//Player.setInvSelect(id);
					}
					//QUICK SLOTS!
					if(id==300)
					{
						printf("QS0");
					}
					if(id==301)
					{
						printf("QS1");
					}
					if(id==302)
					{
						printf("QS2");
					}
					if(id==303)
					{
						printf("QS3");
					}
					if(id==304)
					{
						printf("QS4");
					}
					if(id==305)
					{
						printf("QS5");
					}
					if(id==306)
					{
						printf("QS6");
					}
					if(id==307)
					{
						printf("QS7");
					}
					if(id==308)
					{
						printf("QS8");
					}
					if(id==309)
					{
						printf("QS9");
					}
					
				break;
			}
		}
		return false;
	}   
};