/*

Event-Receiver

*/

bool Keys[KEY_KEY_CODES_COUNT];

class cEventReceiver : public IEventReceiver
{
public:

	cEventReceiver() {
		for (int i=0; i<KEY_KEY_CODES_COUNT; i++) {
			Keys[i] = false;
			KeysTemp[i] = false;
		}

		mouseX=mouseY=mouseDeltaX=mouseDeltaY=0;
	}

	~cEventReceiver() {
	}

	virtual bool OnEvent(const SEvent& event) {
		if (event.EventType==irr::EET_KEY_INPUT_EVENT) {
			Keys[event.KeyInput.Key] = false;
			Keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
			return true;
		}    
		
		if (event.EventType==irr::EET_MOUSE_INPUT_EVENT){
			mouseDeltaX = event.MouseInput.X - mouseX;
			mouseDeltaY = event.MouseInput.Y - mouseY;
			return true;	
		}		
		return false;    
	}


	s32 mouseDeltaX;
	s32 mouseDeltaY;

	bool Keys[KEY_KEY_CODES_COUNT];
	bool KeysTemp[KEY_KEY_CODES_COUNT];
	
private:
	
	s32 mouseX;
	s32 mouseY;
};
