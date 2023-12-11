#include "core.h"
#include "irrlicht.h"
bool CCore::Init(std::string Settings_Filename)
{
    cout<<"Reading Settings from: "<<Settings_Filename.c_str()<<endl;
    IrrXMLReader* xml = createIrrXMLReader(Settings_Filename.c_str());

    if(xml==NULL)
    {
        cout<<"***ERROR: Failed to create XML reader for "<<Settings_Filename.c_str()<<endl;
        return false;
    }

	std::string d;

	int x,y,z,f,vs,sb,aa;
	x=y=z=f=vs=sb=aa=0;

    while(xml && xml->read())
    {
        switch(xml->getNodeType())
        {
            case EXN_ELEMENT:
            {
                if (!strcmp("Screen", xml->getNodeName()))
                {
                    x = xml->getAttributeValueAsInt("X");
                    y = xml->getAttributeValueAsInt("Y");
                    z = xml->getAttributeValueAsInt("Z");
                    f = xml->getAttributeValueAsInt("F");
                }
                else
                if (!strcmp("Render", xml->getNodeName()))
                {
                    aa = xml->getAttributeValueAsInt("AA");
                    sb = xml->getAttributeValueAsInt("SB");
                    vs = xml->getAttributeValueAsInt("VS");
                    d = xml->getAttributeValue("Driver");
                }
            }
            break;
        }
    }
    delete xml;

    cout<<"Got X"<<x<<" Y"<<y<<" Z"<<z;

    if(f)
        cout<<" Fullscreen"<<endl;
    if(!f)
        cout<<" Windowed"<<endl;

    if(aa)
        cout<<"Anti-Aliasing Enabled"<<endl;
    if(sb)
        cout<<"Stenci Buffer Enabled"<<endl;
    if(vs)
        cout<<"Vertical Sync Enabled"<<endl;

    E_DRIVER_TYPE driver;

	if(d!="gl")
		if(d!="dx")
			d="so";

    if(d=="gl")
        driver=EDT_OPENGL;
    if(d=="dx")
        driver=EDT_DIRECT3D9;
	if(d=="so")
		driver=EDT_BURNINGSVIDEO;

    CCore::receiver = new CEventReceiver();

	SIrrlichtCreationParameters params;
	params.AntiAlias=aa;
	params.Stencilbuffer=sb;
	params.Vsync=vs;
	params.WindowSize=dimension2d<s32>(x,y);
	params.Bits=z;
	params.Fullscreen=f;
	params.EventReceiver=CCore::receiver;
	params.DriverType=driver;

	CCore::device = createDeviceEx(params);
	CCore::driver = CCore::device->getVideoDriver();
	CCore::smgr = CCore::device->getSceneManager();
	CCore::gui = CCore::device->getGUIEnvironment();
	CCore::timer = CCore::device->getTimer();
	CCore::device->getCursorControl()->setVisible(false);

    IGUISkin* skin = CCore::gui->getSkin();
	IGUIFont* font = CCore::gui->getFont(".//Media//GUI//fontcourier.bmp");
	skin->setFont(font);

	return true;
};

CEventReceiver* CCore::getReceiver()
{
    return CCore::receiver;
};

IrrlichtDevice* CCore::getDevice()
{
    return CCore::device;
};

IVideoDriver* CCore::getDriver()
{
    return CCore::driver;
};

ISceneManager* CCore::getSMGR()
{
    return CCore::smgr;
};

IGUIEnvironment* CCore::getGUI()
{
    return CCore::gui;
};

CGUI* CCore::CreateGUI()
{
    CCore::GUI = new CGUI(CCore::gui, CCore::driver, CCore::smgr, CCore::device, CCore::receiver, CCore::Sound);
    return CCore::GUI;
}

CMap* CCore::LoadMap(core::stringc filename)
{
    CCore::Map = new CMap(filename, CCore::smgr, CCore::driver, CCore::Sound);
    CCore::PlayerMoveSpeed = CCore::Map->getPlayerInformation().X;
    CCore::PlayerRotSpeed = CCore::Map->getPlayerInformation().Y;
    CCore::PlayerJumpSpeed = CCore::Map->getPlayerInformation().Z;
    return CCore::Map;
};

CFlowManager* CCore::CreateFlowManager()
{
    CCore::FlowManager = new CFlowManager(CCore::driver, CCore::smgr, CCore::Player, CCore::Map, CCore::Sound);
    return CCore::FlowManager;
}

CPlayer* CCore::CreatePlayer()
{
    CCore::Player = new CPlayer(CCore::Map->getCamPosition(), CCore::Map->getCamTarget(), CCore::PlayerMoveSpeed, CCore::PlayerRotSpeed, CCore::PlayerJumpSpeed, CCore::smgr, CCore::driver, CCore::device, CCore::timer, CCore::Map, CCore::receiver, CCore::EffectsManager);
    return CCore::Player;
};

CEffectsManager* CCore::CreateEffectsManager()
{
    CCore::EffectsManager = new CEffectsManager(CCore::smgr, CCore::driver, CCore::timer);
    return CCore::EffectsManager;
}

CPhysics* CCore::CreatePhysicsWorld(btVector3 Gravity)
{
    CCore::Physics = new CPhysics(CCore::smgr, CCore::driver, CCore::timer, Gravity, CCore::Map);
    CCore::Player->setPhysics(CCore::Physics);
    CCore::Physics->setFullDir(CCore::Map->fulldir);
    return CCore::Physics;
}

CSound* CCore::CreateSoundEngine()
{
    CCore::Sound = new CSound();
    return CCore::Sound;
}

CHUD* CCore::CreateHUD()
{
    CCore::HUD = new CHUD(CCore::gui, CCore::driver);
    CCore::Player->setPlayerHUD(CCore::HUD);
    return CCore::HUD;
}

CPowerup* CCore::CreatePowerup(int Type, vector3df Position, int Value)
{
    CPowerup* Powerup = new CPowerup(Type, Position, Value, CCore::smgr, CCore::driver, CCore::Player);
    CCore::Powerups.push_back(Powerup);
    return Powerup;
}

void CCore::Update()
{
    for(list<CPowerup *>::Iterator Iterator = CCore::Powerups.begin(); Iterator != CCore::Powerups.end();)
    {
        CPowerup* Powerup = (*Iterator);
        Powerup->Update();
        if(Powerup->getNode()->getTransformedBoundingBox().intersectsWithBox(CCore::Player->getWeapon()->getTransformedBoundingBox()))
        {
            Powerup->PickedUp();
            Iterator = CCore::Powerups.erase(Iterator);
            CCore::HUD->setAmmo(CCore::Player->Ammo[CCore::Player->CurrentWeapon]);
        }
        else
        {
            Iterator++;
        }
    }
}

bool CCore::isMap()
{
    return CCore::MapExists;
};

bool CCore::isPlayer()
{
    return CCore::PlayerExists;
};

bool CCore::isWeapons()
{
    return CCore::WeaponsLoaded;
};

ITimer* CCore::getTimer()
{
    return CCore::timer;
};

CNPCManager* CCore::CreateNPCManager()
{
    CCore::NPCManager = new CNPCManager(CCore::Player, CCore::Physics, CCore::smgr, CCore::driver, CCore::gui, CCore::timer);
    return CCore::NPCManager;
}

void CCore::Clean()
{
    CCore::EffectsManager->remove();
    CCore::FlowManager->remove();
    CCore::NPCManager->remove();
    CCore::HUD->remove();
    CCore::Player->remove();
    CCore::Physics->remove();
    CCore::Map->remove();

    if(DEBUG_CONSOLE)
        cout<<"Cleaned Everything"<<endl;
}
