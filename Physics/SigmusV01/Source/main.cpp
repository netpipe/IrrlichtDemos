#include "globals.h"
#include "main.h"
#include "core.h"
#include "map.h"
#include "player.h"
#include "physics.h"
#include "debugdraw.h"
#include "powerup.h"
#include "sound.h"
#include "explosion.h"
#include "effectsmanager.h"
#include "NPCmanager.h"
#include "turret.h"
#include "zone.h"
#include "flowmanager.h"
#include "GUI.h"

int main()
{
    //Initialize Everything
    CCore* Core = new CCore();

    if(!Core->Init("./Data/settings.xml"))
    {
        cout<<"***ERROR: Failed to Initialize"<<endl;
        return 1;
    }
    cout<<"Initialized Successfully"<<endl;

    IrrlichtDevice* device = Core->getDevice();
	IVideoDriver* driver = Core->getDriver();
	ISceneManager* smgr = Core->getSMGR();
	IGUIEnvironment* guienv = Core->getGUI();
	ITimer* timer = Core->getTimer();
	CSound* Sound = Core->CreateSoundEngine();
	CGUI* GUI = Core->CreateGUI();

	DebugDraw* Debug = new DebugDraw(device);
	if(DEBUG_PHYSICS)
        Debug->setDebugMode(DBG_DrawAabb | DBG_DrawWireframe);

    IPostProcessBloom* Bloom = new IPostProcessBloom(Core->getSMGR()->getRootSceneNode(), Core->getSMGR(), 666);
    IPostProcessInvert *Invert = new IPostProcessInvert(Core->getSMGR()->getRootSceneNode(), Core->getSMGR(), 666);

    PPE_Bloom_Setup setup;
    setup.sampleDist=0.008;
    setup.strength=5;
    setup.multiplier=1;

    Bloom->initiate(1280, 1024,setup,Core->getSMGR());
    Invert->initiate(1280, 1024,Core->getSMGR());

    ////
	//Start of SIGMUS Main Loop
	////

	while(true)
	{
        core::stringw mapName = GUI->MainMenu();
        if(mapName==L"^^%&&--QUIT!@#321")
            break;

        GUI->cleanMainMenu();

        if(mapName=="FAILED")
            return 1;

        ITexture* LoadScreen = driver->getTexture("./Media/Images/LoadScreen.png");

        IGUIImage* Loading = guienv->addImage(LoadScreen, position2d<s32>(0,0));

        //Create Game's Manager(s)
        CMap* Map = Core->LoadMap(core::stringc(mapName.c_str()));
        CEffectsManager* EffectsManager = Core->CreateEffectsManager();
        CPlayer* Player = Core->CreatePlayer();
        CPhysics* Physics = Core->CreatePhysicsWorld(btVector3(0,-500,0));
        CHUD* HUD = Core->CreateHUD();
        Player->setupPlayerPhysics();
        CNPCManager* NPCManager = Core->CreateNPCManager();
        Physics->loadMapObjects();
        Physics->getWorld()->setDebugDrawer(Debug);
        CFlowManager* FlowManager = Core->CreateFlowManager();
        FlowManager->LoadMapZones();
        FlowManager->LoadMapTriggers();

        Loading->remove();

    //	IAnimatedMeshSceneNode* EnemyNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("./Media/Models/NPCs/Cannon1.dae"));
    //	vector3df TScale = vector3df(2);
    //	EnemyNode->setScale(TScale);
    //	EnemyNode->setMaterialFlag(EMF_LIGHTING, false);
    //	EnemyNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
    //	EnemyNode->setPosition(vector3df(-2496,480,-710));
    //	EnemyNode->setID(ID_IsNotPickable);
    //	CTurret* Turret = NPCManager->AddTurret(L"Turret1",2.5f,EnemyNode,vector3df(0,11,250),vector3df(0,11,600));

        SMaterial debugMat;
        debugMat.Lighting = false;
        while(device->run())
        {
            if(Core->getReceiver()->IsKeyDown(KEY_ESCAPE))
                if(GUI->QuitMenu())
                {
                    Core->Clean();
                    break;
                }

            EffectsManager->Update();
            Physics->Update();
            Player->Update();
            FlowManager->CheckZones();

            driver->beginScene(true, true, SColor(0,0,0,255));

            if(Player->BloomActive)
                driver->setRenderTarget(Bloom->rt0, true, true, SColor(0,0,0,0));
            else
                if(Player->InvertActive)
                    driver->setRenderTarget(Invert->rt0, true, true, SColor(0,0,0,0));

            smgr->drawAll();
            guienv->drawAll();

            driver->setMaterial(debugMat);
            driver->setTransform(ETS_WORLD, IdentityMatrix);
            Physics->getWorld()->debugDrawWorld();

            if(Player->BloomActive)
            {
                if(Player->InvertActive)
                    driver->setRenderTarget(Invert->rt0, true, true, video::SColor(0,0,0,0));
                else
                    driver->setRenderTarget(0);
                Bloom->render();
            }

            if(Player->InvertActive)
            {
                driver->setRenderTarget(0);
                Invert->render();
            }


            NPCManager->Update();
            driver->endScene();
            Core->Update();

            if(Player->isDead)
            {
                Core->Clean();
                GUI->DeathMenu();
                break;
            }

            if(Player->FinishedLevel)
            {
                Core->Clean();
                GUI->EndMenu();
                break;
            }
        }

    }
    ////
    //End of SIMGUS Main Loop
    ////

//    Sound->getEngine()->drop();
    device->drop();
    cout<<"Goodbye";
    return 0;
}
