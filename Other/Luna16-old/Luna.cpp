//! LUNA ENGINE 1.0

#include "Luna.h"
#include "TerrainFactory/WaterNode/CReflectedWater.h"
#include "Net/CGUIChatBox.h"
#include "TerrainFactory/Terrain.h"

#include "Scene/Obstacle.hpp"
#include "Scene/Elevator.hpp"
#include "Physics/Collision.hpp"
#include "GUI/CHUD.h"
#include "GUI/CodeEditor/CGUIEditBoxIRB.h"
#include <vector>
using namespace std;
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//----------------
// Include the headers for post processing
#include "Scene/PostProcessing/CRendererPostProc.h"
#include "Scene/PostProcessing/CEffectPostProc.h"
#include "Scene/PostProcessing/CTransitionPostProc.h"
#include "Scene/PostProcessing/CSplitPostProc.h"
#include "Scene/PostProcessing/CLensFlarePostProc.h"
#include "Scene/PostProcessing/CWaterPostProc.h"
///--------------
//#define PYTHON
#include "Scripting/Python.h"
//#include "GUI/Video/videoPlayer.h"
#include "Scene/ATMOsphere.cpp"
#include "Scene/flares/CLensFlareSceneNode.h"
#include "GUI/compass/Compass.h"
#include "Scene/boids/Flock.h"
#include "Net/IRRc/Application.h"
#include "Scene/CBeamSceneNode.h"
#include "Scene/BoltSceneNode.h"
#include "Physics/Ragdoll.h"
#include "Scene/Formation.h"
#include "GUI/cImage2D.h"
#include "Scene/XEffects/effectWrapper.h"
#include <irrNet.h>

#include "./FileSystem/IrrBlend.h"
#include "./FileSystem/BulletBlendReader.h"
#include "./FileSystem/blenderUp.h"

#include "./Equipment/firstPersonWeapon.h"
#include "./Scene/occlusion/Renderer.h"
#include "./TerrainFactory/CloudSceneNode/CCloudSceneNode.h"
#include "./GUI/Video/CVideoMaster.h"
//#include "./Scene/decalManager/DecalManager.h"
#include "./Scene/Flag/SimpleFlag.h"
#include "Scene/Flag/CFlagSceneNode.h"

//--------------
//#define SPARKA
    #ifdef SPARKA
        #include "Scene/SPARK/SPK.h"
        #include "Scene/SPARK/SPK_IRR.h"
        using namespace SPK;
        using namespace SPK::IRR;
    #endif

    #define DPHYSICS
    #define IRRCD   // irrlicht Collision Detection
//--------------
    //#define DSOUND  // sound
    //#define IRRc
    //#define PostProcess
    //#define ReflectiveWater
    //        #define ReflectWater-Main
    #define TERRAIN //crashes in software mode ?
    //#define TREES //very cpu intensive
    #define ATMOSPHERE ///  freezes when no video acceleration is used
                        //  or as a user without access to video rights is used
    //#define EDITOR
   // #define FLAG
    //#define FLAG2
    //#define COMPASS
    //#define BOIDS
    //#define occlusion
    //#define FPSWEAPON // no uses diff event receiver than player so wont drive car or shoot cubes
    #define BULLETCAR // if FPSWEAPON is enabled the car wont work. diff event rec
    #define LOADLEVELS // dont use this without bullet or with occlusion its slow?
    //#define BULLETBLEND
    //#define RAG   //bulletRagdoll
    //#define EXTRAS
    //#define DESTRUCTION
    //#define FORMATIONDEMO
    //#define HUD
    //#define PLAYER
    //#define Image2D // Just testing out
//--------------
    #ifdef ReflectiveWater
        CReflectedWater *water;
    #endif

    #ifdef TERRAIN
        Terrain *terr;
            Vehicle   *m_cVehicle2;
                btRigidBody *ha;
    #endif
    #ifdef HUD
    CHUD* CHUD2 = new CHUD();
    #endif

    bool works;
    bool login=0;
  //  net::SOutPacket rotationPacket;
    vector3df tmpvect;

    SKeyMap keyMap[8]; // KEYMAP

 int icount=0;


Luna::Luna ( int argc, char** argv ){}

Luna::~Luna(){}

int Luna::devloop()
{
    #include "./DevLoop.h"
    system("PAUSE");
    return 0;
}

// main purpose of this is to get networking up and flying
int Luna::mainloop()
{
    #include "./MainLoop.h"
    system("PAUSE");
    return 0;
}

class ClientNetCallback : public net::INetCallback
{
public:

	 //	u16 playerId = packet.getPlayerId();
	virtual void handlePacket(net::SInPacket& packet)
	{
		//packet.decryptPacket("hushthisissecret");
		//packet.deCompressPacket();
		core::stringc str;
		packet >> str;

		core::vector3df vec;
		packet >> vec;
        //	f32 height;
        //	packet >> height;
        //	rotationPacket << str;
        if (!login){
            if (str == "handshake"){
			login=1;
            std::cout << "Authorized" << str.c_str() << std::endl;
            }
        }
        // }else{}
        // resetV=true;
        // m_cVehicle2->resetVehicle();//( core::vector3df(vec[0],vec[1],vec[2]));
        tmpvect=vector3df(vec);
        std::cout << "vehicle   coords xyz=" << vec.X << vec.Y << vec.Z << std::endl;
        //login=false;
	}
};

int Luna::doLogin ( const std::wstring &username, const std::wstring &password )
{
    // 0 is ok, -1 is no connection , 1 is wrong password
    std::string uname ( username.begin(), username.end() );
    std::string pword ( password.begin(), password.end() );

		if (netManager->getConnectionStatus() != net::EICS_FAILED)
		{
			netManager->update(0);
			net::SOutPacket packet;
			if (login){
						packet << "actPack2";
                        netManager->sendOutPacket(packet);
                        return 0;
			}else{
                        packet << "handshake";  // seems this has to be sent first or nothing works
                        netManager->sendOutPacket(packet);
                        return 3;  // nothing loop around again
            }
			 packet.compressPacket();
			 packet.encryptPacket("hushthisissecret"); //16 char max
             netManager->sendOutPacket(packet);
		}
    return -1;
}




int Luna::handleMessages(){return 0;}

int Luna::shutdown()
{
//    delete netManager;
    delete m_cPhysics;
    delete m_cScene;
    delete m_cVehicle;
    delete m_cPlayer;
    guienv->drop();
    smgr->drop();
    device->drop();
    delete device;
    delete smgr;
    delete guienv;
    return 0;
}

int Luna::init()
{
    bool FULLSCREEN=1;
    resolution[0]= 640; resolution[1]= 480;
    //IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);
    //core::dimension2du res = nulldevice->getVideoModeList()->getDesktopResolution();
    //nulldevice -> drop();
    device = createDevice ( EDT_OPENGL,dimension2du (resolution[0],resolution[1]), 24, 0);
    //device = createDevice ( EDT_OPENGL,dimension2du (res.Width,res.Height ), 32, true, true, false, 0 );

    //device->setWindowCaption ( L"Luna Engine v1-initial" );
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    device->setEventReceiver ( &m_cInGameEvents );

    //Physics init
    m_cPhysics = new Physics();
    m_cPhysics->registerIrrDevice(device);

    //networking
        #ifdef NDEBUG
            ClientNetCallback* clientCallback = new ClientNetCallback();
            netManager = net::createIrrNetClient(clientCallback, "127.0.0.1");
            netManager->setVerbose(true);           // debug messages
        #endif

    keyMap[0].Action = EKA_MOVE_FORWARD;    keyMap[0].KeyCode = KEY_KEY_W;
    keyMap[1].Action = EKA_MOVE_FORWARD;    keyMap[1].KeyCode = KEY_KEY_W;
    keyMap[2].Action = EKA_MOVE_BACKWARD;   keyMap[2].KeyCode = KEY_KEY_S;
    keyMap[3].Action = EKA_MOVE_BACKWARD;   keyMap[3].KeyCode = KEY_KEY_S;
    keyMap[4].Action = EKA_STRAFE_LEFT;     keyMap[4].KeyCode = KEY_KEY_A;
    keyMap[5].Action = EKA_STRAFE_LEFT;     keyMap[5].KeyCode = KEY_KEY_A;
    keyMap[6].Action = EKA_STRAFE_RIGHT;    keyMap[6].KeyCode = KEY_KEY_D;
    keyMap[7].Action = EKA_STRAFE_RIGHT;    keyMap[7].KeyCode = KEY_KEY_D;

#ifndef ATMOSPHERE
  smgr->addLightSceneNode(NULL, core::vector3df(0,10,0), video::SColorf(5.0f, 255.0f, 255.0f), 2500, -1);
#endif

    return 0;
}




int Luna::Run()
{

    if ( init() < 0 ) return -1;
    events.devLogin=0;
    #ifndef NDEBUG
    events.devLogin=1;
    #endif
    while ( device->run() && !m_cInGameEvents.Quit)
    {
        	if ( events.devLogin )
                {
                    device->setEventReceiver ( &m_cInGameEvents );
                    devloop();
                }else{
                    if ( lobby() < 1 ) {  shutdown();  break; }
                    device->setEventReceiver ( &m_cInGameEvents );
                    mainloop();
                }
    }
      //      getchar();
    shutdown();
    return 0;
}


btRigidBody* ha2;
bool bingo = true;
bool yesim = true;
scene::IBillboardSceneNode * bill;
int nextDecal = 0;
void Luna::CheckKeyStates(void)
{
    if (m_cInGameEvents.getKeyState(    KEY_ESCAPE))
                    {m_cInGameEvents.Quit = true;}


    if (m_cInGameEvents.getKeyState(    KEY_SPACE))    //SPACEBAR
    {    //  m_cPlayer->jump();
            vector3df ha = camera->getAbsolutePosition();
      //  printf("Jump position: %f %f %f \n", pos[0], pos[1], pos[2]);
        camera->setPosition(vector3df( ha.X, ha.Y+40, ha.Z));
     //   camera->setTarget(vector3df(1000,1000,1000));
    }

#ifdef BULLETCAR
    if (m_cInGameEvents.getKeyState(    KEY_KEY_P))
    {
        btVector3 pos = m_cVehicle->getVehiclePosition();
        vector3df ha = vector3df(pos[0], pos[1], pos[2]);
        printf("Vehicle position: %f %f %f \n", pos[0], pos[1], pos[2]);
        camera->setPosition(vector3df( ha.X, ha.Y, ha.Z));
    }

    if (m_cInGameEvents.getKeyState(    KEY_KEY_1))
    {
        vector3df pos = camera->getPosition();
        vector3df scl = vector3df(1,1,1);
        m_cPhysics->createBox( btVector3(pos.X, pos.Y, pos.Z), btVector3(scl.X, scl.Y, scl.Z), 10); //weight
    }


    if (m_cInGameEvents.getKeyState(    KEY_KEY_4))    //SPACEBAR
    {
        btVector3 Normal;
//      vector3df pos = camera->getPosition();
        vector3df upvect = camera->getUpVector();
        vector3df target = camera->getTarget();

        core::line3d<f32> line;
		line.start = camera->getPosition();
		line.end = line.start + (camera->getTarget() - line.start).normalize() * 1000.0f;

        btVector3 rayHit = m_cPhysics->RaycastWorld(btVector3(line.start.X, line.start.Y, line.start.Z),btVector3(line.end.X, line.end.Y, line.end.Z),Normal);
        printf("ray position position: %f %f %f \n", rayHit[0], rayHit[1], rayHit[2]);
        printf("hit normal vector: %f %f %f \n", Normal[0], Normal[1], Normal[2]);

///just testing
//        if (yesim)
//        {
//             bill = smgr->addBillboardSceneNode();
//             yesim = false;
//        }
//        bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
//        bill->setMaterialTexture(0, driver->getTexture("./media/particle.bmp"));
//        bill->setMaterialFlag(video::EMF_LIGHTING, false);
//        bill->setMaterialFlag(video::EMF_ZBUFFER, false);
//        bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
////        bill->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it
//        bill->setPosition(vector3df(rayHit[0], rayHit[1], rayHit[2]));
     //    if (smgr->getSceneCollisionManager()->getCollisionPoint(line, selector, intersection, tri,outNode))
     //    {
            //Setup decal sprite
         if ( rayHit[0] != 0)
         {
             vector3df hitsend = vector3df(rayHit[0], rayHit[1], rayHit[2]);
             vector3df norm = vector3df(Normal[0], Normal[1], Normal[2]);

            decals[nextDecal]->Setup(norm,hitsend);
            nextDecal++;
            if (nextDecal >= MAX_DECALS)
               nextDecal = 0;
         }
    }


    if (m_cInGameEvents.getKeyState(    KEY_SPACE))    //SPACEBAR
                    {m_cVehicle->brake();   }

    if (m_cInGameEvents.getKeyState(    KEY_KEY_2))
    {
        //if ( icount > 15){ //sphere limiter
            vector3df pos = camera->getPosition();
            vector3df rot = camera->getRotation();

            if (bingo) {
            ha2 = m_cPhysics->createSphere( btVector3(pos.X, pos.Y, pos.Z),2,5);
            ha2->setAngularVelocity(btVector3(400,400,400));
            bingo= false;
            } else {
                btVector3 pos2 = ha2->getCenterOfMassPosition();
                camera->setPosition(vector3df(pos2[0],pos2[1],pos2[2]));
            }
          //  ha2->clearForces();
           // ha2->applyDamping(100);
           // ha2->applyForce(btVector3(50,8000,50),btVector3(110,111,110));
           // icount=0;
            //m_cPhysics->clearBodies(); crashes
   // }
     //         icount++;
    }
    if (m_cInGameEvents.getKeyState(    KEY_KEY_R))
                    {m_cVehicle->resetVehicle();}
    if (m_cInGameEvents.getKeyState(    KEY_UP))
                    {m_cVehicle->accelerate(10);}
    else if (!m_cInGameEvents.getKeyState(  KEY_UP) && (m_cVehicle->getState() != EVEHICLE_REVERSING))
                    {m_cVehicle->accelerate(-20);}   //wind resistance

    if (m_cInGameEvents.getKeyState(    KEY_DOWN))
                    {m_cVehicle->reverse(10);    }
    else if (!m_cInGameEvents.getKeyState(  KEY_DOWN) && (m_cVehicle->getState() != EVEHICLE_ACCELERATING))
                    {m_cVehicle->reverse(-20);}      // wind resistance
    if (m_cInGameEvents.getKeyState(    KEY_LEFT))
                    {m_cVehicle->steer_left(); }
    if (m_cInGameEvents.getKeyState(    KEY_RIGHT))
                    {m_cVehicle->steer_right(); }
    if (!m_cInGameEvents.getKeyState(   KEY_LEFT) && !m_cInGameEvents.getKeyState(KEY_RIGHT))
                    {m_cVehicle->steer_reset(); }
#endif
//vector3df ha = camera->getAbsolutePosition();
 //   camera->setPosition(vector3df( ha.X, ha.Y-11, ha.Z));
}




