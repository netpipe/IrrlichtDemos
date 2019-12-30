#ifndef PHYSIC
#define PHYSIC

#include <vector>
#include <string>
#include <sstream>
#include <map>

//#include <pthread.h>

#include "Events/Events.h"
#include "Events/InGameEventReceiver.h"
#include "Scene/customNodes.h"
#include <cAudio.h>

#include "Net/irrNetClient.h"
#include "entities/player.h"
//#include "Encryption/Blowfish.h"
#include "Physics/Physics.h"
#include "Physics/Vehicle.h"
#include "Scene/Scene.h"
#include <irrNet.h>
#include "./Scene/decals/decalscenenode.h"

#include "irrlicht.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Luna
{
	public:
		Luna ( int argc, char** argv );
		~Luna(); // Cleans up the engine
		int Run();
		IrrlichtDevice *device;
		unsigned int resolution[2];
        IVideoDriver *driver;
		ISceneManager *smgr;

	private:
		int init();
		int shutdown();
		int lobby();
		int mainloop();
		int devloop();
		int doLogin ( const std::wstring &username, const std::wstring &password );
		int handleMessages();

		int AddShader();

        void CheckKeyStates(void);

		IGUIEnvironment *guienv;

        #define MAX_DECALS 50
        DecalSceneNode* decals[MAX_DECALS];

		EventRec events;
		InGameEventReceiver m_cInGameEvents;

        // Factory threads
		//pthread_t soundThread;
        cAudio::IAudioManager* manager;
        cAudio::IAudioSource* mysound;

		// Encryption
            //Encryption::Blowfish enc;
		// Network related
        net::INetManager* netManager;
        // Character related
		std::wstring username;
		std::wstring password;

		// Player Physics
        Player *m_cPlayer;
        Scene *m_cScene;
		Physics *m_cPhysics;
		Vehicle *m_cVehicle;

    int MakeTrees();
    void recursiveFillMetaSelector(scene::ISceneNode* node, scene::IMetaTriangleSelector* meta );

    scene::IMetaTriangleSelector* metaSelector;
        //ICameraSceneNode* camera;
        scene::ISceneNodeAnimatorCollisionResponse* anim;
        ICameraSceneNode* camera;

    f32 SpeedMultiplier;// = 1.1f;


};




#endif
