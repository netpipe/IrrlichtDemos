/*
This tutorial will briefly show how to use the terrain renderer of Irrlicht. It will also
show the terrain renderer triangle selector to be able to do collision detection with
terrain.

Note that the Terrain Renderer in Irrlicht is based on Spintz' GeoMipMapSceneNode, lots
of thanks go to him.
DeusXL provided a new elegant simple solution for building larger area on small heightmaps
-> terrain smoothing.
In the beginning there is nothing special. We include the needed header files and create
an event listener to listen if the user presses the 'W' key so we can switch to wireframe
mode and if he presses 'D' we toggle to material between solid and detail mapped.
*/
#include <irrlicht.h>
#include <iostream>

using namespace irr;

#pragma comment(lib, "Irrlicht.lib")

//------------------------------------------------------------------------------
//! sets up a quad mesh of given dimension
void SetupQuadMesh( scene::SMeshBuffer &mesh, f32 f32Width )
{
   f32 f32HalfWidth = f32Width/2.0f;
    mesh.Vertices.set_used(4);
    mesh.Indices .set_used(6);

    video::SColor white(255, 255, 255, 255);

    mesh.Vertices[0] = video::S3DVertex(-f32HalfWidth, -f32HalfWidth, 0.f, 0.f, 0.f, 1.f, white, 0.f, 1.f);
    mesh.Vertices[1] = video::S3DVertex(-f32HalfWidth,  f32HalfWidth, 0.f, 0.f, 0.f, 1.f, white, 0.f, 0.f);
    mesh.Vertices[2] = video::S3DVertex( f32HalfWidth,  f32HalfWidth, 0.f, 0.f, 0.f, 1.f, white, 1.f, 0.f);
    mesh.Vertices[3] = video::S3DVertex( f32HalfWidth, -f32HalfWidth, 0.f, 0.f, 0.f, 1.f, white, 1.f, 1.f);

    mesh.Indices[0] = 0;
    mesh.Indices[1] = 1;
    mesh.Indices[2] = 2;
    mesh.Indices[3] = 2;
    mesh.Indices[4] = 3;
    mesh.Indices[5] = 0;

    mesh.getMaterial().Lighting = false;
    //mesh.getMaterial().BackfaceCulling = false;
    //mesh.getMaterial().Wireframe = true;
    mesh.getMaterial().MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
}

class VxCompass : public gui::IGUIElement
{
public:
   //=== constructor ===//
   VxCompass( core::rect<s32> rect, gui::IGUIEnvironment* env, gui::IGUIElement* parent );

   //! set body of compass texture
   void SetCompassBodyTexture( video::ITexture* texture );
   //! set needle of compass texture
   void SetCompassNeedleTexture( video::ITexture* texture );

   //! set compass heading
   void SetCompassHeading( f32 deg );

   //! render the compass
   virtual void draw();

private:
   scene::SMeshBuffer BodyMesh;
   scene::SMeshBuffer NeedleMesh;
   core::matrix4 Matrix;
};
//------------------------------------------------------------------------------
//=== constructor ===//
VxCompass::VxCompass(core::rect<s32> rect, gui::IGUIEnvironment* env, gui::IGUIElement* parent)
      : gui::IGUIElement(gui::EGUIET_ELEMENT, env, parent, -1, rect)
{
   SetupQuadMesh( BodyMesh, 1.0f );
   SetupQuadMesh( NeedleMesh, 1.0f );

}

//------------------------------------------------------------------------------
//! set body of compass texture
void VxCompass::SetCompassBodyTexture(video::ITexture* texture)
{
    BodyMesh.getMaterial().setTexture(0,texture);

}
//------------------------------------------------------------------------------
//! set needle of compass texture
void VxCompass::SetCompassNeedleTexture(video::ITexture* texture)
{
    NeedleMesh.getMaterial().setTexture(0,texture);
}
//------------------------------------------------------------------------------
//! set compass heading
void VxCompass::SetCompassHeading( f32 deg )
{
    Matrix.makeIdentity();
    Matrix.setRotationDegrees(core::vector3df(0, 0, deg));
}

//------------------------------------------------------------------------------
//! render the compass
void VxCompass::draw()
{
    video::IVideoDriver* driver = Environment->getVideoDriver();
    if (! (driver && IsVisible))
        return;

    core::rect<s32> oldViewPort = driver->getViewPort();
    driver->setViewPort(getAbsolutePosition());

    // clear the projection matrix
    core::matrix4 oldProjMat = driver->getTransform(video::ETS_PROJECTION);
    driver->setTransform(video::ETS_PROJECTION, core::matrix4());

    // clear the view matrix
    core::matrix4 oldViewMat = driver->getTransform(video::ETS_VIEW);
    driver->setTransform(video::ETS_VIEW, core::matrix4());

    driver->setTransform(video::ETS_WORLD, Matrix);

   // draw compass body
    driver->setMaterial(BodyMesh.Material);
    driver->drawMeshBuffer(&BodyMesh);

    driver->setTransform(video::ETS_WORLD, core::matrix4());

   // draw the needle
   driver->setMaterial(NeedleMesh.Material);
    driver->drawMeshBuffer(&NeedleMesh);

    // restore view matrix
    driver->setTransform(video::ETS_VIEW, oldViewMat);

    // restore projection matrix
    driver->setTransform(video::ETS_PROJECTION, oldProjMat);



    // restore the view area
    driver->setViewPort(oldViewPort);
}




class MyEventReceiver : public IEventReceiver
{
public:

   MyEventReceiver(scene::ISceneNode* terrain)
   {
      // store pointer to terrain so we can change its drawing mode
      Terrain = terrain;
   }

   virtual bool OnEvent(const SEvent& event)
   {
      // check if user presses the key 'W' or 'D'
      if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
      {
         switch (event.KeyInput.Key)
         {
         case irr::KEY_KEY_W: // switch wire frame mode
            Terrain->setMaterialFlag(video::EMF_WIREFRAME, !Terrain->getMaterial(0).Wireframe);
            Terrain->setMaterialFlag(video::EMF_POINTCLOUD, false);
            return true;
         case irr::KEY_KEY_P: // switch wire frame mode
            Terrain->setMaterialFlag(video::EMF_POINTCLOUD, !Terrain->getMaterial(0).PointCloud);
            Terrain->setMaterialFlag(video::EMF_WIREFRAME, false);
            return true;
         case irr::KEY_KEY_D: // toggle detail map
            Terrain->setMaterialType(
               Terrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
               video::EMT_DETAIL_MAP : video::EMT_SOLID);
            return true;
         }
      }

      return false;
   }

private:
   scene::ISceneNode* Terrain;
};


/*
The start of the main function starts like in most other example. We ask the user
for the desired renderer and start it up.
*/
int main()
{
   // let user select driver type

   // create device

   IrrlichtDevice* device = createDevice(video::EDT_OPENGL, core::dimension2d<s32>(640, 480));

   if (device == 0)
      return 1; // could not create selected driver.


   /*
   First, we add standard stuff to the scene: A nice irrlicht engine
   logo, a small help text, a user controlled camera, and we disable
   the mouse cursor.
   */

   video::IVideoDriver* driver = device->getVideoDriver();
   scene::ISceneManager* smgr = device->getSceneManager();
   gui::IGUIEnvironment* env = device->getGUIEnvironment();

   driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

   // add irrlicht logo
   env->addImage(driver->getTexture("../../media/irrlichtlogo2.png"),
      core::position2d<s32>(10,10));

   //set other font
   env->getSkin()->setFont(env->getFont("../../media/fontlucida.png"));

   // add some help text
   gui::IGUIStaticText* text = env->addStaticText(
      L"Press 'W' to change wireframe mode\nPress 'D' to toggle detail map",
      core::rect<s32>(10,440,250,475), true, true, 0, -1, true);

   // add camera
   scene::ICameraSceneNode* camera =
      smgr->addCameraSceneNodeFPS(0,100.0f,.1f);

   camera->setPosition(core::vector3df(1900*2,255*2,3700*2));
   camera->setTarget(core::vector3df(2397*2,343*2,2700*2));
   camera->setFarValue(12000.0f);

   // disable mouse cursor
   device->getCursorControl()->setVisible(false);

   /*
   Here comes the terrain renderer scene node: We add it just like any
   other scene node to the scene using ISceneManager::addTerrainSceneNode().
   The only parameter we use is a file name to the heightmap we use. A heightmap
   is simply a gray scale texture. The terrain renderer loads it and creates
   the 3D terrain from it.
   To make the terrain look more big, we change the scale factor of it to (40, 4.4, 40).
   Because we don't have any dynamic lights in the scene, we switch off the lighting,
   and we set the file terrain-texture.jpg as texture for the terrain and
   detailmap3.jpg as second texture, called detail map. At last, we set
   the scale values for the texture: The first texture will be repeated only one time over
   the whole terrain, and the second one (detail map) 20 times.
   */

   // add terrain scene node
   scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
      "../../media/terrain-heightmap.bmp",
      0,                              // parent node
      -1,                              // node id
      core::vector3df(0.f, 0.f, 0.f),         // position
      core::vector3df(0.f, 0.f, 0.f),         // rotation
      core::vector3df(40.f, 4.4f, 40.f),      // scale
      video::SColor ( 255, 255, 255, 255 ),   // vertexColor,
      5,                              // maxLOD
      scene::ETPS_17,                     // patchSize
      4                              // smoothFactor
      );

   terrain->setMaterialFlag(video::EMF_LIGHTING, false);

   terrain->setMaterialTexture(0, driver->getTexture("../../media/terrain-texture.jpg"));
   terrain->setMaterialTexture(1, driver->getTexture("../../media/detailmap3.jpg"));

   terrain->setMaterialType(video::EMT_DETAIL_MAP);

   terrain->scaleTexture(1.0f, 20.0f);
   //terrain->setDebugDataVisible ( true );

   /*
   To be able to do collision with the terrain, we create a triangle selector.
   If you want to know what triangle selectors do, just take a look into the
   collision tutorial. The terrain triangle selector works together with the
   terrain. To demonstrate this, we create a collision response animator
   and attach it to the camera, so that the camera will not be able to fly
   through the terrain.
   */

   // create triangle selector for the terrain
   scene::ITriangleSelector* selector
      = smgr->createTerrainTriangleSelector(terrain, 0);
   terrain->setTriangleSelector(selector);
   selector->drop();

   // create collision response animator and attach it to the camera
   scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
      selector, camera, core::vector3df(60,100,60),
      core::vector3df(0,0,0),
      core::vector3df(0,50,0));
   camera->addAnimator(anim);
   anim->drop();

   /*
   To make the user be able to switch between normal and wireframe mode, we create
   an instance of the event reciever from above and let Irrlicht know about it. In
   addition, we add the skybox which we already used in lots of Irrlicht examples.
   */

   // create event receiver
   MyEventReceiver receiver(terrain);
   device->setEventReceiver(&receiver);

      // create skybox
   driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

   smgr->addSkyBoxSceneNode(
      driver->getTexture("../../media/irrlicht2_up.jpg"),
      driver->getTexture("../../media/irrlicht2_dn.jpg"),
      driver->getTexture("../../media/irrlicht2_lf.jpg"),
      driver->getTexture("../../media/irrlicht2_rt.jpg"),
      driver->getTexture("../../media/irrlicht2_ft.jpg"),
      driver->getTexture("../../media/irrlicht2_bk.jpg"));

   driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);


   // add a compass
   core::rect<s32> gCompassRect;
   gCompassRect.UpperLeftCorner.X = 50;
   gCompassRect.UpperLeftCorner.Y = 50;
   gCompassRect.LowerRightCorner.X = 150;
   gCompassRect.LowerRightCorner.Y = 150;
   gui::IGUIElement* pgRootGuiElement = env->getRootGUIElement();

   VxCompass * pgCompass = new VxCompass( gCompassRect, env, pgRootGuiElement );
   video::ITexture * pgCompassBodyTexture =   driver->getTexture("compass_body.tga");
   video::ITexture * pgCompassNeedleTexture =   driver->getTexture("compass_needle.tga");
   pgCompass->SetCompassBodyTexture( pgCompassBodyTexture );
   pgCompass->SetCompassNeedleTexture( pgCompassNeedleTexture );


   /*
   That's it, draw everything. Now you know how to use terrain in Irrlicht.
   */

   int lastFPS = -1;

   while(device->run())
   if (device->isWindowActive())
   {
      driver->beginScene(true, true, 0 );

// BRJ this doesnt work.. replaced with code below
//      core::matrix4 gCamMat = camera->getAbsoluteTransformation();
//      core::vector3df v3fRotation = gCamMat.getRotationDegrees();
//      pgCompass->SetCompassHeading( v3fRotation.Z );

      //BRJ correct way to get heading posted by Vitek
      core::vector3df fore(0, 0, 1);
      camera->getAbsoluteTransformation().rotateVect(fore);
      core::vector3df rot1 = fore.getHorizontalAngle();
      pgCompass->SetCompassHeading( rot1.Y );

      smgr->drawAll();
      env->drawAll();

      driver->endScene();

      // display frames per second in window title
      int fps = driver->getFPS();
      if (lastFPS != fps)
      {
         core::stringw str = L"Terrain Renderer - Irrlicht Engine [";
         str += driver->getName();
         str += "] FPS:";
         str += fps;
         // Also print terrain height of current camera position
         // We can use camera position because terrain is located at coordinate origin
         str += " Height: ";
         str += terrain->getHeight(camera->getAbsolutePosition().X, camera->getAbsolutePosition().Z);

         device->setWindowCaption(str.c_str());
         lastFPS = fps;
      }
   }

   device->drop();

   return 0;
}

