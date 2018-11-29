
#include <irrlicht.h>
#include <iostream>
#include <GL/gl.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif


#define ANAGLYPHMODE

   IrrlichtDevice *device;

   video::IVideoDriver* driver;
    scene::ISceneManager* smgr;
    scene::ICameraSceneNode* Cam;

void drawAnaglyph2()
{
    vector3df oldPosition=Cam->getPosition();
   vector3df oldTarget=Cam->getTarget();

   matrix4 startMatrix=Cam->getAbsoluteTransformation();

   vector3df focusPoint= (Cam->getTarget()-Cam->getAbsolutePosition()).setLength(1) + Cam->getAbsolutePosition() ;

   //Left eye...

   vector3df leftEye;
   matrix4 leftMove;
   leftMove.setTranslation( vector3df(-0.01f,0.0f,0.0f) );
   leftEye=(startMatrix*leftMove).getTranslation();

   //clear the depth buffer, and color
 //   driver->beginScene( true, true, SColor(0,200,200,255) );

   driver->getOverrideMaterial().Material.ColorMask=ECP_RED;
   driver->getOverrideMaterial().EnableFlags=EMF_COLOR_MASK;
   driver->getOverrideMaterial().EnablePasses=ESNRP_SKY_BOX+ESNRP_SOLID+ESNRP_TRANSPARENT+ESNRP_TRANSPARENT_EFFECT+ESNRP_SHADOW;

    Cam->setPosition( leftEye );
    Cam->setTarget( focusPoint );

   smgr->drawAll(); // 'smgr->drawAll();' may go here


   //Right eye...

   vector3df rightEye;
   matrix4 rightMove;
   rightMove.setTranslation( vector3df(0.01f,0.0f,0.0f) );
   rightEye=(startMatrix*rightMove).getTranslation();

   //clear the depth buffer
   driver->clearZBuffer();

   driver->getOverrideMaterial().Material.ColorMask=ECP_GREEN+ECP_BLUE;
   driver->getOverrideMaterial().EnableFlags=EMF_COLOR_MASK;
   driver->getOverrideMaterial().EnablePasses=ESNRP_SKY_BOX+ESNRP_SOLID+ESNRP_TRANSPARENT+ESNRP_TRANSPARENT_EFFECT+ESNRP_SHADOW;

    Cam->setPosition( rightEye );
   Cam->setTarget( focusPoint );

    smgr->drawAll(); // 'smgr->drawAll();' may go here

 //   driver->endScene();

   driver->getOverrideMaterial().Material.ColorMask=ECP_ALL;
   driver->getOverrideMaterial().EnableFlags=0;
   driver->getOverrideMaterial().EnablePasses=0;

    Cam->setPosition( oldPosition );

    Cam->setTarget( oldTarget );

}



static bool DrawAnaglyph( irr::video::IVideoDriver *pDriver,
                    irr::scene::ISceneManager *pSm,
                    irr::scene::ICameraSceneNode *pCamera,
                    irr::video::SColor colBackground,
                    float fWidth, float fFocus, int nDriverType,
                    unsigned long ulREyeKey, unsigned long ulLEyeKey )
{
   // Right eye
    irr::core::vector3df reye = pCamera->getPosition();

   // Left eye
   irr::core::vector3df v( fWidth, 0, 0 );
   irr::core::matrix4 m;
   m.setRotationDegrees( pCamera->getRotation() + irr::core::vector3df( 90.f, 0, 0 ) );
   m.transformVect( v );
   irr::core::vector3df leye = reye + v;

   // Eye target
   irr::core::vector3df oldt = pCamera->getTarget();
   irr::core::vector3df eyet = ( oldt - reye ).normalize() * fFocus;
    pCamera->setTarget( eyet );
#ifdef DX
   IDirect3DDevice9 *pDdx9 = NULL;
   if ( nDriverType == irr::video::EDT_DIRECT3D9
              && pDriver->getExposedVideoData().D3D9.D3DDev9 )
      pDdx9 = pDriver->getExposedVideoData().D3D9.D3DDev9;
    #endif
   // Setup right eye
   if ( nDriverType == irr::video::EDT_OPENGL )
   {   glMatrixMode( GL_MODELVIEW );
      glLoadIdentity();
      glColorMask( 0 != ( ulREyeKey & 0x00ff0000 ),
                 0 != ( ulREyeKey & 0x0000ff00 ),
                 0 != ( ulREyeKey & 0x000000ff ),
                0 != ( ulREyeKey & 0xff000000 ) );
   } // end if
   #ifdef DX
   else if ( pDdx9 )
      pDdx9->SetRenderState( D3DRS_COLORWRITEENABLE,
                        ( ulREyeKey & 0x00ff0000 ? D3DCOLORWRITEENABLE_RED : 0 )
                        | ( ulREyeKey & 0x0000ff00 ? D3DCOLORWRITEENABLE_GREEN : 0 )
                        | ( ulREyeKey & 0x000000ff ? D3DCOLORWRITEENABLE_BLUE : 0 )
                        | ( ulREyeKey & 0xff000000 ? D3DCOLORWRITEENABLE_ALPHA : 0 ) );
    #endif
 //   pDriver->beginScene( true, true, colBackground );
    pSm->drawAll();

   // Left Eye
   if ( nDriverType == irr::video::EDT_OPENGL )
   {   glClear( GL_DEPTH_BUFFER_BIT );
      glMatrixMode( GL_MODELVIEW );
      glLoadIdentity();
      glColorMask( 0 != ( ulLEyeKey & 0x00ff0000 ),
                 0 != ( ulLEyeKey & 0x0000ff00 ),
                 0 != ( ulLEyeKey & 0x000000ff ),
                0 != ( ulLEyeKey & 0xff000000 ) );
   } // end if
   #ifdef DX
   else if ( pDdx9 )
   {   pDdx9->Clear(0 , 0 , D3DCLEAR_ZBUFFER, 0x000000 , 1.0f , 0);
      pDdx9->SetRenderState( D3DRS_COLORWRITEENABLE,
                        ( ulLEyeKey & 0x00ff0000 ? D3DCOLORWRITEENABLE_RED : 0 )
                        | ( ulLEyeKey & 0x0000ff00 ? D3DCOLORWRITEENABLE_GREEN : 0 )
                        | ( ulLEyeKey & 0x000000ff ? D3DCOLORWRITEENABLE_BLUE : 0 )
                        | ( ulLEyeKey & 0xff000000 ? D3DCOLORWRITEENABLE_ALPHA : 0 ) );
   } // end else if
   #endif
    pCamera->setPosition( leye );
   pCamera->OnRegisterSceneNode();

    pSm->drawAll();
 //   pDriver->endScene();

   if ( nDriverType == irr::video::EDT_OPENGL )
      glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
      #ifdef DX
   else if ( pDdx9 )
   if ( pDdx9 )
      pDdx9->SetRenderState( D3DRS_COLORWRITEENABLE,
                        D3DCOLORWRITEENABLE_BLUE
                        | D3DCOLORWRITEENABLE_GREEN
                        | D3DCOLORWRITEENABLE_RED
                        | D3DCOLORWRITEENABLE_ALPHA );
#endif
   // Restore original position
    pCamera->setPosition( reye );
   pCamera->setTarget( oldt );

   return true;
}




int main()
{

   device =
      createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480));

   if (device == 0)
      return 1; // could not create selected driver.

   driver = device->getVideoDriver();
   scene::ISceneManager* smgr = device->getSceneManager();


   device->getFileSystem()->addZipFileArchive("../../../media/map-20kdm2.pk3");

   scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
   scene::ISceneNode* node = 0;

   if (mesh)
//      node = smgr->addOctTreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
      node = smgr->addMeshSceneNode(mesh->getMesh(0));

   /*
   Because the level was not modelled around the origin (0,0,0), we
   translate the whole level a little bit. This is done on
   irr::scene::ISceneNode level using the methods
   irr::scene::ISceneNode::setPosition() (in this case),
   irr::scene::ISceneNode::setRotation(), and
   irr::scene::ISceneNode::setScale().
   */
   if (node)
      node->setPosition(core::vector3df(-1300,-144,-1249));


   Cam = smgr->addCameraSceneNodeFPS();

   /*
   The mouse cursor needs not be visible, so we hide it via the
   irr::IrrlichtDevice::ICursorControl.
   */
   device->getCursorControl()->setVisible(false);

   int lastFPS = -1;

   while(device->run())
   {
      if (device->isWindowActive())
      {
        driver->beginScene(true, true, video::SColor(255,200,200,200));
         #ifdef ANAGLYPHMODE
            DrawAnaglyph(driver,smgr,Cam,video::SColor(255,200,200,200),2,1000,video::EDT_OPENGL ,0x000000ff,0x00ffff00);
         #else
            drawAnaglyph2();
         #endif
         //
    //     smgr->drawAll();
         driver->endScene();

         int fps = driver->getFPS();

         if (lastFPS != fps)
         {
            core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
            str += driver->getName();
            str += "] FPS:";
            str += fps;

            device->setWindowCaption(str.c_str());
            lastFPS = fps;
         }
      }
      else
         device->yield();
   }

   /*
   In the end, delete the Irrlicht device.
   */
   device->drop();
   return 0;
}

/*
That's it. Compile and play around with the program.
**/
