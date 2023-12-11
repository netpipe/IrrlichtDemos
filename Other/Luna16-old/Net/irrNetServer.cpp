//#define okNotTesting
#ifdef okNotTesting

#include <unistd.h>
#include <cstdio>
#include <string>

#ifdef WIN32
#include <windows.h>
#endif
#include <irrNet.h>
#include <iostream>

//#include "logging/logging.h"
#include "sqlCon.h"
#include "includes/squirrel.h"
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


void handleConnection();
bool doit;
bool connected;

vector3df tmpvect;
//	u32 address = netManager->getClientAddress(playerId);

class ServerNetCallback : public net::INetCallback
{
 public:
	ServerNetCallback(net::INetManager* netManagerIn) : netManager(netManagerIn) {}

	// Override the "onConnect" function, don't forget to get the method
	// signature correct. Whenever a fresh client connects, their brand
	// spanking new player id will be passed to this function. A "u16"
	// is a typedef for an unsigned short, incase you were wondering.
	virtual void onConnect(const u16 playerId)
	{
	    connected=1;
        //!dont bother sending  anything on connect to the client its deaf --Unit
		//	net::SOutPacket packet;
		//	core::stringc message;
		//	message = "handshake";
			//message += playerId;
			//message += " has just connected.";
		//	packet << message;
		//	netManager->sendOutPacket(packet);
	}

	virtual void onDisconnect(const u16 playerId)
	{
	    doit=0;
	    connected=0;
		net::SOutPacket packet;
		core::stringc message;
		message = "Client number ";
		message += playerId;
		message += " has just left the building.";
		packet << message;
		netManager->sendOutPacket(packet);
	}

	virtual void handlePacket(net::SInPacket& packet)
	{
        core::stringc str;
		packet >> str;

        core::vector3df yes;
		packet >> yes;

            if (str == "handshake"){
                doit=true;
                std::cout << "AUTHORIZED with " << str.c_str();
            }
            if (str == "actPack2"){
                       //        doit=true;
                std::cout <<"Auth Communications 2" << str.c_str();
            }

            tmpvect = vector3df(yes);

	}
private:
	core::array<u32> banList;
	net::INetManager* netManager;
};



int main()
{
                    std::cout << "Create IrrDevice " ;

	IrrlichtDevice *device = createDevice( video::EDT_BURNINGSVIDEO, dimension2d<s32>(640, 480), 16,
                                            false, false, false, 0);

                                            if (!device)return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	guienv->addStaticText(L"Server!",
		rect<s32>(10,10,260,22), true);

	IAnimatedMesh* mesh = smgr->getMesh("../../libs/Irrlicht/media/sydney.md2");
	if (!mesh)return 1;

	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	if (node)
	{   node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("../../libs/Irrlicht/media/sydney.bmp") );
	}
	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

    //log << std::string("loop");
		//  The default port that clients can connect to is set to 45000.
		net::INetManager* netManager = net::createIrrNetServer(0);
		ServerNetCallback* serverCallback = new ServerNetCallback(netManager);
		netManager->setNetCallback(serverCallback);
        //  netManager->setVerbose(true);


    //! TEST AREA FOR SCRIPTING AND SQL
    Squirrel *SQ;
    SQ->do_main();
    SQ->LoadFile("test.nut");

   // sqlCon *sq =new sqlCon("ha.db");
  //  sq->execute("CREATE TABLE learnTb (a integer, b string);");
    //! -------EOA

	while(device->run())
	{
		while(netManager->getConnectionStatus() != net::EICS_FAILED)
		{
			netManager->update(100); //update(delay);
            net::SOutPacket packet;// To send a packet, first you create an SOutPacket object.
          if (connected){
            if (doit)
            {
                std::cout << "Sending ActPacket \n " ;
                packet << "actPack2";
            }else{
                std::cout << "Sending Handshake \n " ;
                packet << "handshake"; // has to send something or it crashes the client
            }


            packet << vector3df(tmpvect);
                      std::cout << "sending X=" << tmpvect.X << "Y=" << tmpvect.Y << "Z=" << tmpvect.Z ;

            netManager->sendOutPacket(packet);
          }
          else{
         // std::cout << "Sleeping for 150 units\n " ;
          device->sleep(150,0);
          };
        break;
        }
     //   std::cout << vector3df(tmpvect);
		driver->beginScene(true, true, SColor(255,100,101,140));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
	}

	device->drop();
    delete netManager;
    delete serverCallback;
    return 0;
}

#endif
