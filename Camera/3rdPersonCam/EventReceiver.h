
bool MBRIGHT=0;
bool MBLEFT=0;

bool Keys_Down[256];
int MWHEEL=0;
int JUMPING=0;
int WALKING=0;
int MX, MY, OLDMX, OLDMY,OLDMPOSX,OLDMPOSY,MOUSEX,MOUSEY;


class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(SEvent event)
	{
		MWHEEL=0;
if (event.EventType == irr::EET_KEY_INPUT_EVENT&&event.KeyInput.PressedDown)
			{int n;
			for(n=1;n<256;n++){
				if(event.KeyInput.Key==n)Keys_Down[n]=true;}
			}
if (event.EventType == irr::EET_KEY_INPUT_EVENT&&!event.KeyInput.PressedDown)
			{int n;
			for(n=1;n<256;n++){
				if(event.KeyInput.Key==n)Keys_Down[n]=false;}
			}
if (event.EventType == irr::EET_MOUSE_INPUT_EVENT){
		if(event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN){
			OLDMPOSX=MOUSEX;
			OLDMPOSY=MOUSEY;
			MBRIGHT=1;
			}
		if(event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP){
			MBRIGHT=0;
			}
		if(event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN){
			OLDMPOSX=MOUSEX;
			OLDMPOSY=MOUSEY;
			MBLEFT=1;
			}
		if(event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP){
			MBLEFT=0;
			}
		
		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL){
		MWHEEL=event.MouseInput.Wheel;
		
		}
		if (event.MouseInput.Event == EMIE_MOUSE_MOVED){
			//get pos difference
			MOUSEX=event.MouseInput.X;
			MOUSEY=event.MouseInput.Y;
			}
		}
		return false;

	}

};