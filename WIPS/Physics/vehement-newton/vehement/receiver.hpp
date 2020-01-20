// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_RECEIVER

class MyEventReceiver : public IEventReceiver
{
public:

	MyEventReceiver() { };

	bool OnEvent(const SEvent& event)
	{
		// store pointer to device
		mydevice = dfactory::instance()->getIrrDevice();
		thedevice = dfactory::instance();

		/* LEFT MOUSE CLICK */
		if(event.EventType == EET_MOUSE_INPUT_EVENT &&
			event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
		{
				//Process any clicks on GUI Components
				thedevice->getStateManager()->processGUI(event.MouseInput.X,event.MouseInput.Y);

			//Dont process these if paused
			if (!dfactory::instance()->getIsPaused())
			{			
				//Process any clicks on the map editor components
				thedevice->getMapEditor()->processGUI(event.MouseInput.X,event.MouseInput.Y);
						
				//Get the node thats under the mouse cursor and push it with our miiiind
				ISceneNode * mynode = dfactory::instance()->getIrrDevice()->getSceneManager()->getSceneCollisionManager()->
				getSceneNodeFromScreenCoordinatesBB(core::position2d<s32>(event.MouseInput.X,event.MouseInput.Y)) ;
				if (mynode && mynode->getType()==ESNT_ANIMATED_MESH
					&& !thedevice->getObject(mynode->getID()).scripted)
					thedevice->pushObject(mynode->getID());

				
				//Is the object scripted?
				if (mynode && mynode->getType()==ESNT_ANIMATED_MESH
					&& thedevice->getObject(mynode->getID()).scripted)
					thedevice->getScripting()->runScript(
					thedevice->getObject(mynode->getID()).script);
	
			}
		}
		/* Left Mouse UP */
		else if (event.EventType == EET_MOUSE_INPUT_EVENT &&
			event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP)
		{
		

		}

		/* RIGHT MOUSE CLICK */
		if(event.EventType == EET_MOUSE_INPUT_EVENT &&
			event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN)
		{
			dfactory::instance()->freeMouse();
			return true;
		}

		/* KEY BOARD EVENT */
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr::KEY_ESCAPE: 
				mydevice->closeDevice();
				return true;
				break;
			case irr::KEY_F12: 
				thedevice->getMapEditor()->createEditor();
				return true;
				break;
			case irr::KEY_F11: 
				thedevice->getPlayerManager()->savePlayer("./data/saves/player1.cobj");
				return true;
				break;
			default:
				return false;
				break;
			}
		}

		return false;
	};

private:
	IrrlichtDevice	* mydevice;
	dfactory		* thedevice;
};

