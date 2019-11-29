#include <irrlicht.h>
#pragma comment(lib, "Irrlicht.lib")
#include <iostream>


using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::gui;


IrrlichtDevice *device = 0;
video::IVideoDriver* driver = 0;
scene::ISceneManager* smgr;
scene::IMetaTriangleSelector* metaSelector;
scene::ISceneNodeAnimatorCollisionResponse* anim;
ICameraSceneNode* camera = 0;

bool fin1;


//This function courtesy of christianclavet on the Irrlicht forums
void recursiveFillMetaSelector(scene::ISceneNode* node, scene::IMetaTriangleSelector* meta )
{
  //
  // the following if is basically the same as ISceneNode_assignTriangleSelector
  //
  printf ("Node name is: %s \n",node->getName());
  printf ("Node id is: %d \n",node->getID());
  printf ("Node type:");
  //  printf ("Node type: %s=",smgr->getSceneNodeTypeName());
  if (node->getType() ==   ESNT_UNKNOWN) printf("Unknown mesh type \n\n");
  if (node->getType() ==   ESNT_MESH) printf("Standard Mesh \n\n");
  if (node->getType() ==   ESNT_ANIMATED_MESH) printf("Animated Mesh! \n\n");
  if (node->getType() ==   ESNT_SKY_BOX) printf("SkyBox! \n\n");
  if (node->getType() ==   ESNT_CAMERA_FPS) printf("Fps Camera! \n\n");
  if (node->getType() ==   ESNT_CAMERA_MAYA ) printf("Maya Camera! \n\n");
  if (node->getType() ==   ESNT_CAMERA )
  { printf("STD Camera! \n");
    printf ("The current position of this camera is: %f,%f,%f\n\n",node->getPosition().X,node->getPosition().Y,node->getPosition().Z);
    camera->setPosition(node->getPosition());
  }
  if (node->getType() ==   ESNT_PARTICLE_SYSTEM ) printf("Particles! \n\n");
  if (node->getType() ==   ESNT_LIGHT  ) printf("Light! \n\n");
  if (node->getType() ==   ESNT_OCTREE)
  {
      // Occ Trees are for land
      printf("Occtree! \n");
      io::IAttributes* attribs = device->getFileSystem()->createEmptyAttributes();
      if (attribs)
        {// get the mesh name out
         node->serializeAttributes(attribs);
         core::stringc name = attribs->getAttributeAsString("Mesh");
         attribs->drop();
         // get the animated mesh for the object
         scene::IAnimatedMesh* mesh = smgr->getMesh(name.c_str());
         if (mesh)
         {
            scene::ITriangleSelector* selector =
            smgr->createOctTreeTriangleSelector(mesh->getMesh(0), node, 128);
            node->setTriangleSelector(selector);
            metaSelector->addTriangleSelector(selector);
            selector->drop();
         }

     }

  }
  // now recurse on children...
  core::list<scene::ISceneNode*>::ConstIterator begin = node->getChildren().begin();
  core::list<scene::ISceneNode*>::ConstIterator end   = node->getChildren().end();

  for (; begin != end; ++begin)
    recursiveFillMetaSelector(*begin, meta);
}



int main()
{

	//Creates Irrdevice and the video driver and scene manager
    device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1680, 1050), 32, false, true, true);
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();

	//Creates the gui environment for future use
	IGUIEnvironment* env = device->getGUIEnvironment();

	//Setup the caption on the window
    core::stringw str = "Walkthrough";
    device->setWindowCaption(str.c_str());

    //Define the Keyboard to use for the camera moves
    SKeyMap keyMap[8];
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

	//Loads the Object file and creates a standard skybox
	video::ITexture* top = driver->getTexture("skybox/default_skyboxup.jpg");
	video::ITexture* bottom = driver->getTexture("skybox/default_skyboxdn.jpg");
	video::ITexture* left = driver->getTexture("skybox/default_skybox1.jpg");
	video::ITexture* right = driver->getTexture("skybox/default_skybox3.jpg");
	video::ITexture* front = driver->getTexture("skybox/default_skybox2.jpg");
	video::ITexture* back = driver->getTexture("skybox/default_skybox0.jpg");

	smgr->addSkyBoxSceneNode(top, bottom, left, right, front, back);
	scene::IAnimatedMesh* mesh = smgr->getMesh("model/object.obj");
    ISceneNode* node = smgr->addOctreeSceneNode(mesh);
	node->setMaterialFlag(video::EMF_LIGHTING, false);


	//Sets up the camera node
    camera = smgr->addCameraSceneNodeFPS(0, 120.0f, 200.0f, -1, keyMap, 9, true,0.5f);
	camera->setPosition(vector3df(0, 10,0));
    camera->setFarValue(11000.0);
    camera->setAspectRatio(16/9);

    //Create the metaselector for the collision detection
    metaSelector = smgr->createMetaTriangleSelector();
    recursiveFillMetaSelector( smgr->getRootSceneNode(), metaSelector );

    // Define the collision reponse for the FPS Camera
    anim = smgr->createCollisionResponseAnimator(metaSelector, camera, core::vector3df(10,35,10), core::vector3df(0,-1.0f,0), core::vector3df(0,24,0));
    camera->addAnimator(anim);
    anim->drop();
    metaSelector->drop();

	//Run the game
    while(device->run() && fin1==false)
   {
      if (device->isWindowActive())
      {
         driver->beginScene(true, true, video::SColor(0,0,0,0));
         smgr->drawAll();
		 env->drawAll();
		 driver->endScene();
	  }
   }
   device->closeDevice();
    return 0;
}
