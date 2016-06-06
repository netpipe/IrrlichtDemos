
// Irrlicht
   #include <irrlicht.h>

   using namespace irr;

   using namespace core;
   using namespace scene;
   using namespace video;
   using namespace io;
   using namespace gui;


   #ifdef _IRR_WINDOWS_
      #pragma comment(lib, "Irrlicht.lib")
      #pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
   #endif


// MAIN

int main()
{
   // create Irrlicht device
      IrrlichtDevice* device = createDevice( video::EDT_OPENGL, dimension2d<s32>(800, 600), 32,
            false, false);

      if (device == 0)
         return 1; // could not create selected driver.

      video::IVideoDriver* driver = device->getVideoDriver();
      scene::ISceneManager* smgr = device->getSceneManager();

   // create camera
      smgr->addCameraSceneNodeFPS(0, 100.0f, .1f);

   // Load Char
      IAnimatedMesh* CharMesh = smgr->getMesh("dwarf.x");
      if (!CharMesh) return 1;

      IAnimatedMeshSceneNode* CharNode = smgr->addAnimatedMeshSceneNode( CharMesh );
      if (!CharNode) return 1;

   // Initilise
      CharNode->setMaterialFlag(EMF_LIGHTING, false);
      CharNode->setAnimationSpeed(10);

   // Prepare for Joint Control
      CharNode->setJointMode(EJUOR_CONTROL); //To write positions to the mesh on render



   int lastFPS = -1;


   f32 headRotation = 0;
   f32 headPosition = 0;


   while(device->run())
   {

   // Animate character manually
      CharNode->animateJoints();


   // Get the Joint you want to MANIPULATE, and its Parent
      IBoneSceneNode* thisJoint = CharNode->getJointNode("head");

      if( thisJoint )
      {
         ISceneNode* parentJoint = thisJoint->getParent();

         if( parentJoint )
         {
         // Get the default (animated) Joint's position and rotation
            vector3df jointPos = thisJoint->getAbsolutePosition();
            vector3df jointRot = thisJoint->getAbsoluteTransformation().getRotationDegrees();

         // Get the absolute INVERSE Transformation matrix of the parent
            matrix4 iparentTransform = parentJoint->getAbsoluteTransformation();
               iparentTransform.makeInverse();

         // Set the Absolute Position or Rotation of the Joint without fear!
            vector3df newJointPos = jointPos + vector3df( 0, 1+(sin(headPosition)*2), 0 );
            vector3df newJointRot = vector3df( 0, headRotation, 0 );

         // Transform the Position by the Parent's Inverse matrix before applying it
            iparentTransform.transformVect( newJointPos );

         // APPLY
            thisJoint->setPosition( newJointPos );
            thisJoint->setRotation( newJointRot );
         }
      }


   // funny
      headPosition += 0.01f;
      if( headPosition > 2*PI ){ headPosition -= 0; }

      headRotation += 0.5f;
      if( headRotation > 360 ){ headRotation -= 360; }


   // do Irrlicht stuff

      driver->beginScene(true, true, video::SColor(255,113,113,133));

      smgr->drawAll(); // draw the 3d scene
      device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

      driver->endScene();
device->sleep(1);
      int fps = driver->getFPS();

      if (lastFPS != fps)
      {
         core::stringw tmp(L"fmx's funny dwarf head joint manipulation demo! [");
         tmp += driver->getName();
         tmp += L"] fps: ";
         tmp += fps;

         device->setWindowCaption(tmp.c_str());
         lastFPS = fps;
      }
   }


   // show cursor
   //   device->getCursorControl()->setVisible(true);



   device->drop();

   return 0;
}
