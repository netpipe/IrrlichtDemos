
/** Example 004 Movement
modified by KH. June 13, 2009
*/


#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class CMorphTargetAnimation
{
   public:

   scene::ISceneManager* smgr;
   scene::IMeshSceneNode * node;
   s32 poseCount;
   s32 vertexCount;
   ///Here I'm using a vector3df array .You could also use a Vertex  or float array.
   array<vector3df> VectorArray;
   vector3df* pSourceVerts;
   vector3df* pDestinationVerts;

   video::S3DVertex* pFinalVerts;
   s32  src,dest;
   f32 time;


    CMorphTargetAnimation(scene::ISceneManager* smgr_,scene::IMeshSceneNode * node_):smgr(smgr_),node(node_),poseCount(0),vertexCount(0)
    ,pSourceVerts(0),pDestinationVerts(0),pFinalVerts(0),src(0),dest(0),time(0)
    {
         _IRR_DEBUG_BREAK_IF(smgr)
         _IRR_DEBUG_BREAK_IF(node)
          node->grab();
          smgr->grab();
          IMesh* msh = node->getMesh();
          msh->grab();

//from what I've seen in the forum if you don't remove it from the cache
//any other nodes that use the same mesh will also get morphed
          smgr->getMeshCache()->removeMesh (msh);
          pFinalVerts = (S3DVertex*)msh->getMeshBuffer(0)->getVertices();
          vertexCount = msh->getMeshBuffer(0)->getVertexCount();
///scene::IMeshCache::removeMesh  ( const IMesh *const   mesh   )

    }

    s32 addPose(IMesh* mesh,bool remove = 1)
    {
       if(!mesh) return 0;
       IMeshBuffer* meshBuff = mesh->getMeshBuffer(0);
       video::S3DVertex* pvertBuff = (video::S3DVertex*) meshBuff->getVertices();

       if(vertexCount != meshBuff->getVertexCount())
            return 0;

       for (u32 i=0; i <vertexCount; i++)
       {
          VectorArray.push_back(pvertBuff[i].Pos);
          ///VertexArray.push_back(pvertBuff[i]);
       }
       poseCount++;

       dest=poseCount-1;

       pSourceVerts = VectorArray.pointer();
       pDestinationVerts = VectorArray.pointer() + (dest*vertexCount);

       if(remove)
      {
           smgr->getMeshCache()->removeMesh (mesh);
      }
       return poseCount;

    }

    void destroy()
    {
        if(node)
        {
            node->getMesh()->drop();
            node->removeAll();
            node->remove();
        }
         node=0;
         if(smgr) smgr->drop();
    }

    ~CMorphTargetAnimation()
    {
         destroy();
    }


    void swapPoses()
    {
          vector3df* tmp = pSourceVerts;
          pSourceVerts = pDestinationVerts;
          pDestinationVerts = tmp;
    }

    void  animatePoses(f32 speed)
    {
        time+=speed;
        if(time>1.f)
        {
           ///to do this as simply as I can,I'll just swap poses
           ///another way is to mult. time by a + or - direction
          swapPoses();
          time=0;
        }

        if (poseCount>1)
        {
            for (int i=0; i< vertexCount; i++)
            {
                 pFinalVerts[i].Pos = pSourceVerts[i].getInterpolated(pDestinationVerts[i],time);
            }
        }
    }


};


class MyEventReceiver : public IEventReceiver
{
public:
   // This is the one method that we have to implement
   virtual bool OnEvent(const SEvent& event)
   {
      // Remember whether each key is down or up
      if (event.EventType == irr::EET_KEY_INPUT_EVENT)
         KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

      return false;
   }

   // This is used to check whether a key is being held down
   virtual bool IsKeyDown(EKEY_CODE keyCode) const
   {
      return KeyIsDown[keyCode];
   }

   MyEventReceiver()
   {
      for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
         KeyIsDown[i] = false;
   }

private:
   // We use this array to store the current state of each key
   bool KeyIsDown[KEY_KEY_CODES_COUNT];
};




CMorphTargetAnimation* Morph;


int main()
{
   // let user select driver type

   video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

   printf("Please select the driver you want for this example:\n"\
      " (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
      " (d) Software Renderer\n (e) Burning's Software Renderer\n"\
      " (f) NullDevice\n (otherKey) exit\n\n");


   // create device
   MyEventReceiver receiver;

   IrrlichtDevice* device = createDevice(driverType,
         core::dimension2d<u32>(640, 480), 16, false, false, false, &receiver);

   if (device == 0)
      return 1; // could not create selected driver.

   video::IVideoDriver* driver = device->getVideoDriver();
   scene::ISceneManager* smgr = device->getSceneManager();

   /*
   Create the node which will be moved with the WSAD keys. We create a
   sphere node, which is a built-in geometry primitive. We place the node
   at (0,0,30) and assign a texture to it to let it look a little bit more
   interesting. Because we have no dynamic lights in this scene we disable
   lighting for each model (otherwise the models would be black).
   */
///this is the mesh you will see
   scene::IMeshSceneNode * node = smgr->addMeshSceneNode(smgr->getMesh("./media/earth.x"));
   if (node)
   {
      //node->setPosition(core::vector3df(0,0,0));
      node->setMaterialTexture(0, driver->getTexture("./media/wall.bmp"));
      node->setMaterialFlag(video::EMF_LIGHTING, false);


      Morph = new CMorphTargetAnimation(smgr,node);
///these 2 are just temporary to get vertex positions from
      IAnimatedMesh* mesh = smgr->getMesh("./media/earth.x");
      Morph->addPose(mesh->getMesh(0));


      mesh = smgr->getMesh("./media/earth.x");
      Morph->addPose(mesh->getMesh(0));



   }



    scene::ISceneNode * PlaneMesh = smgr->addMeshSceneNode(smgr->addHillPlaneMesh("ground",core::dimension2d<f32>(64,64),core::dimension2d<u32>(8,8))->getMesh(0));
    PlaneMesh->setMaterialTexture(0, driver->getTexture("./media/stones.jpg"));
    PlaneMesh->setMaterialFlag(EMF_LIGHTING, false);


   /*
   To be able to look at and move around in this scene, we create a first
   person shooter style camera and make the mouse cursor invisible.
   */
   smgr->addCameraSceneNodeFPS();
   smgr->getActiveCamera()->setPosition(core::vector3df(0,20,-50));
   device->getCursorControl()->setVisible(false);



//   gui::IGUIStaticText* help = device->getGUIEnvironment()->addStaticText(
//      L"Press UP/down  left/right keys to start morph", core::rect<s32>(10, 10, 400, 20));
//   diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));

   /*
   We have done everything, so lets draw it. We also write the current
   frames per second and the name of the driver to the caption of the
   window.
   */
   int lastFPS = -1;

   // In order to do framerate independent movement, we have to know
   // how long it was since the last frame
   u32 then = device->getTimer()->getTime();

   // This is the movemen speed in units per second.
   const f32 MOVEMENT_SPEED = 1.1f;

   while(device->run())
   {
      // Work out a frame delta time.
      const u32 now = device->getTimer()->getTime();
      const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
      then = now;

      /* Check if keys W, S, A or D are being held down, and move the
      sphere node around respectively. */
      core::vector3df nodePosition = node->getPosition();
//
      if(receiver.IsKeyDown(irr::KEY_KEY_W))
         nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
      else if(receiver.IsKeyDown(irr::KEY_KEY_S))
         nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;

      if(receiver.IsKeyDown(irr::KEY_KEY_A))
         nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
      else if(receiver.IsKeyDown(irr::KEY_KEY_D))
         nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;

      node->setPosition(nodePosition);

        Morph->animatePoses(MOVEMENT_SPEED * frameDeltaTime);

      driver->beginScene(true, true, video::SColor(255,113,113,133));

      smgr->drawAll(); // draw the 3d scene
      device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

      driver->endScene();

      int fps = driver->getFPS();

      if (lastFPS != fps)
      {
         core::stringw tmp(L"Movement Example - Irrlicht Engine [");
         tmp += driver->getName();
         tmp += L"] fps: ";
         tmp += fps;

         device->setWindowCaption(tmp.c_str());
         lastFPS = fps;
      }
   }

   /*
   In the end, delete the Irrlicht device.
   */
   delete Morph;
   device->drop();

   return 0;
}

/*
That's it. Compile and play around with the program.
**/

