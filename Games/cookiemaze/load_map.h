#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



void loadMap(core::stringw mapNameb)
{
	video::IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	screenShot_fader = guienv->addInOutFader();
    screenShot_fader->setColor(video::SColor(0,255,255,255));

    escFadeToBlack = guienv->addInOutFader();
    escFadeToBlack->setColor(video::SColor(0,0,0,0));

    smgr->setAmbientLight(video::SColorf(0.0f, 0.5f, 0.5f, 0.5f));

    IGUISkin* game_skin = guienv->getSkin();
    IGUIFont* menu_font = guienv->getFont("data/gui/fonts/f0.xml");
        if (menu_font)
                game_skin->setFont(menu_font);


    //hide the cursor
    isInGame = 1;
    isBackImg = 0;
    device->getCursorControl()->setVisible(false);


        //load map
        core::stringc newMapName = "data/maps/";
        newMapName+= mapNameb;
        newMapName+= "/map.irr";

        smgr->loadScene(newMapName.c_str());


   //camera
   SKeyMap keyMap[9];//was9
   keyMap[0].Action = EKA_MOVE_FORWARD;
   keyMap[0].KeyCode = KEY_UP;
   keyMap[1].Action = EKA_MOVE_FORWARD;
   keyMap[1].KeyCode = KEY_KEY_W;

   keyMap[2].Action = EKA_MOVE_BACKWARD;
   keyMap[2].KeyCode = KEY_DOWN;
   keyMap[3].Action = EKA_MOVE_BACKWARD;
   keyMap[3].KeyCode = KEY_KEY_S;

   keyMap[4].Action = EKA_STRAFE_LEFT;
   keyMap[4].KeyCode = KEY_LEFT;
   keyMap[5].Action = EKA_STRAFE_LEFT;
   keyMap[5].KeyCode = KEY_KEY_A;

   keyMap[6].Action = EKA_STRAFE_RIGHT;
   keyMap[6].KeyCode = KEY_RIGHT;
   keyMap[7].Action = EKA_STRAFE_RIGHT;
   keyMap[7].KeyCode = KEY_KEY_D;

   keyMap[8].Action = EKA_JUMP_UP;
   keyMap[8].KeyCode = KEY_SPACE ;


   //add skydome
   core::stringc newXMLskyValue = "data/maps/";
   newXMLskyValue+= mapNameb;
   newXMLskyValue+= "/sky/skydome.jpg";

   smgr->addSkyDomeSceneNode(driver->getTexture(newXMLskyValue),16,16,1.0f,1.1f);


   gameHUD = driver->getTexture("data/gui/main_menu/hud/gameHUD.png");


   playaIrrNode = smgr->getSceneNodeFromName("player",0);


   playaCamera = smgr->addCameraSceneNodeFPS(0, 45.0f, 0.05f, -1, keyMap, 9, true, 0.0f);//was 1.5f
   playaCamera->setFarValue(2000);
   playaCamera->setID(0);

   if(playaIrrNode){playaCamera->setPosition(core::vector3df(playaIrrNode->getPosition()));}else{playaCamera->setPosition(core::vector3df(0,1000,0));}


    // initialize the score
    scoreText  = guienv->addStaticText(L"SCORE:", core::rect<s32>(10,10,150,50), false, false);
    scoreValue = guienv->addStaticText(L"0", core::rect<s32>(160,10,300,50), false, false);

    // initialize the lives count
    livesText  = guienv->addStaticText(L"LIVES:", core::rect<s32>(screenResX-160,10,screenResX-45,50), false, false);
    livesValue = guienv->addStaticText(L"3", core::rect<s32>(screenResX-40,10,screenResX-5,50), false, false);

    // initialize the timer
    timerMin = guienv->addStaticText(L"00", core::rect<s32>(screenResX/2-55,30,screenResX/2-5,80), false, false);
    timerSec = guienv->addStaticText(L"00", core::rect<s32>(screenResX/2+5,30,screenResX/2+55,80), false, false);

   timerActualSec = 60;
   timerActualMin = 10;

    // Create a meta triangle selector to hold several triangle selectors.
	IMetaTriangleSelector *metaTriangleSelector = smgr->createMetaTriangleSelector();

	core::array<scene::ISceneNode *> nodes;
	smgr->getSceneNodesFromType(scene::ESNT_ANY, nodes); // Find all nodes

	for (u32 i=0; i < nodes.size(); ++i)
	{
		scene::ISceneNode * node = nodes[i];
		scene::ITriangleSelector *triangleSelector = 0;

		switch(node->getType())
		{
		case scene::ESNT_CUBE:
		case scene::ESNT_SPHERE:
		case scene::ESNT_ANIMATED_MESH:
            node->setMaterialFlag(video::EMF_FOG_ENABLE, false);
			triangleSelector = smgr->createTriangleSelectorFromBoundingBox(node);
		break;


        case scene::ESNT_MESH:
        {
		    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);

            u32 nodeNamen = node->getID();
            nodeNameX  = node->getName();

            if (nodeNamen==33 || nodeNamen==99 || nodeNamen==66 )
            {
            //rotate the bonus node
            printf("[INFO]: Node found: %s\n", nodeNameX);
			if (nodeNamen==33){
            scene::ISceneNodeAnimator* rotateAnim = smgr->createRotationAnimator(core::vector3df(0,0.3f,0));
            node->addAnimator(rotateAnim);
			rotateAnim->drop();}else{}

			if (nodeNamen==66) //spike
			{
			//move the spike
			printf("[INFO]: Node found: %s\n", nodeNameX);
			vector3df nodeSpikePos = node->getPosition();
			core::array<core::vector3df> points;

			points.push_back(core::vector3df(nodeSpikePos.X, nodeSpikePos.Y-5, nodeSpikePos.Z));
			points.push_back(core::vector3df(nodeSpikePos.X, nodeSpikePos.Y-5, nodeSpikePos.Z));
			points.push_back(core::vector3df(nodeSpikePos.X, nodeSpikePos.Y-50, nodeSpikePos.Z));

            scene::ISceneNodeAnimator* rotateSpikeAnim = smgr->createFollowSplineAnimator(2000, points, 1.0f, 0.5f);
            node->addAnimator(rotateSpikeAnim);
			rotateSpikeAnim->drop();
			}else{}

			}else{triangleSelector = smgr->createTriangleSelectorFromBoundingBox(node);}

			//if (nodeNamen==99){printf("[INFO]: Exit door node found: %s\n", nodeNameX);}else{}

        }
		break;



		case scene::ESNT_OCTREE:
            {
            node->setMaterialFlag(video::EMF_FOG_ENABLE, false);
            triangleSelector = smgr->createTriangleSelectorFromBoundingBox((scene::IMeshSceneNode*)node);
            }
			break;



		default:
			// Don't create a selector for this node type
			break;
		}

		if(triangleSelector)
		{
			metaTriangleSelector->addTriangleSelector(triangleSelector);
			triangleSelector->drop();
		}


	}

	scene::ISceneNodeAnimator* anim;

    anim = smgr->createCollisionResponseAnimator(
		metaTriangleSelector, playaCamera, core::vector3df(4,8,4),                //elipsoid size
		core::vector3df(0,-5,0),  //was 0,-5,0                                    //gravity
        core::vector3df(0,8,0));                                                  //elipsoid translation



	metaTriangleSelector->drop();                                                 // I'm done with the meta selector now

	playaCamera->addAnimator(anim);
	anim->drop();                                                                 // I'm done with the animator now


  //load the GUI
  IGUISkin* game_skinG = guienv->getSkin();
  IGUIFont* gameEventList_font = guienv->getFont("data/gui/fonts/f0.xml");
        if (gameEventList_font)
                game_skinG->setFont(gameEventList_font);


   //add a Collision Box to the player camera

   playerCollisionBox = smgr->addCubeSceneNode(10);//was 3
   playaCamera->addChild(playerCollisionBox);

    //play audio
   if (musicOn==1)
   {
		int randomSongDice = rand() % 2;
		if (randomSongDice==1){ systemFMOD->playSound(FMOD_CHANNEL_FREE, bkLevelSound, 0, &channel2);} else
							  { systemFMOD->playSound(FMOD_CHANNEL_FREE, bkLevel2Sound, 0, &channel2);}
   } else{}


 printf("[INFO] Map loaded\n");
}
