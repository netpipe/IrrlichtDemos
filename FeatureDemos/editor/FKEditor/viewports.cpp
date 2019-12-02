#include "viewports.h"
//
// Check for mouse collision with a open window
//

bool viewprt::checkForWindow(s32 element, s32 x, s32 y)
{
   IGUIEnvironment* env = device->getGUIEnvironment();
   IGUIElement* root = env->getRootGUIElement();
   IGUIElement* e = root->getElementFromId(element, true);
   if (e) {
            core::rect< s32 > infos;
            infos=e->getAbsolutePosition();
            return infos.isPointInside(position2d<s32>(x,y));
      }
   return false;
}

void viewprt::init(IrrlichtDevice *Dev, int vrs)
{ device=Dev; viewrightspace=vrs;
  viewscale2=5.0f; viewscale3=5.0f; viewscale4=5.0f;
  bool gridvisibility=true;
             bool groundvisibility=true;
             bool gridvispers=true;
}
//
// Set the active camera
//
void viewprt::setActiveCamera ( scene::ICameraSceneNode* newActive )
{
   if ( 0 == device )
      return;

   scene::ICameraSceneNode* active = device->getSceneManager()->getActiveCamera ();

   newActive->setInputReceiverEnabled ( true );
   device->getSceneManager()->setActiveCamera ( newActive );
}


void viewprt::createCameras()
{
   scene::ISceneManager* smgr = device->getSceneManager();
        // add a camera scene node
   Camera[0] = smgr->addCameraSceneNodeMaya(); Camera[0]->setPosition(core::vector3df(0,100,200)); Camera[0]->setTarget(core::vector3df(0,0,0));
   Camera[1] = smgr->addCameraSceneNodeFPS(); // perhaps needed in future (for testing)
   // Create the orthographic cameras
   Camera[2] = smgr->addCameraSceneNode(); Camera[2]->setPosition(core::vector3df(0,16000,0)); Camera[2]->setTarget(core::vector3df(0,0,0));
   //Camera[2]->setFarValue(32000);
   Camera[3] = smgr->addCameraSceneNode(); Camera[3]->setPosition(core::vector3df(0,0,16000)); Camera[3]->setTarget(core::vector3df(0,0,0));
   //Camera[3]->setFarValue(32000);
   Camera[4] = smgr->addCameraSceneNode(); Camera[4]->setPosition(core::vector3df(-16000,0,0)); Camera[4]->setTarget(core::vector3df(0,0,0));
   //Camera[4]->setFarValue(32000);
   //Camera[0]->setFOV(45 * irr::core::DEGTORAD); // Nice clean field of view (45 degrees)
   Camera[0]->setFOV(PI/2);
   Camera[0]->setFarValue(32000);

   setActiveCamera ( Camera[0] );
}

void viewprt::setModel(scene::IAnimatedMeshSceneNode* Mod)
{ Model=Mod; }

void viewprt::setRightSpace(int rs)
{ viewrightspace=rs; printf("I've been called %i\n",rs);}

void viewprt::resetViews(bool kind)
{   // Put all views in their default states
    // views are: 0=Perspective, 2=Top, 3=Front, 4=side
    if (kind) {viewscale2=5.0f; viewscale3=5.0f; viewscale4=5.0f;
               Camera[0]->setPosition(core::vector3df(0,100,-200));
               Camera[0]->setTarget(core::vector3df(0,0,0));}
    Camera[2]->setPosition(core::vector3df(0,16000,0));
    Camera[2]->setTarget(core::vector3df(0,0,0));
    Camera[3]->setPosition(core::vector3df(0,0,16000));
    Camera[3]->setTarget(core::vector3df(0,0,0));
    Camera[4]->setPosition(core::vector3df(-16000,0,0));
    Camera[4]->setTarget(core::vector3df(0,0,0));
}

//
// return the detected viewport from the current mouse coordinates
// 0=perspective view, 2=Top left, 3=bottom left, 4=bottom right
int viewprt::tellViewport(float mouseX, float mouseY)
{
    video::IVideoDriver* driver = device->getVideoDriver();
    f32 screenx = driver->getScreenSize().Width;
    f32 screeny = driver->getScreenSize().Height;
    screenx-=viewrightspace;
    bool canGo=true;
    if (loading) return -1;
    if ((mouseX>(screenx)) && (viewrightspace>0)) {return -1;}
    if (!validmove)
      {if ((checkForWindow(6002,(s32)mouseX,(s32)mouseY))) {canGo=false; return -1;}
       if ((checkForWindow(5000,(s32)mouseX,(s32)mouseY))) {canGo=false; return -1;}
      }
    if ((currentview==1) && (mouseX>screenx/2) && (mouseX<screenx) && (mouseY>45) && (mouseY<screeny/2)) return 0;
    if ((currentview==1) && canGo)
    {
      if (((mouseX>0) && (mouseX<screenx/2) && (mouseY>45) && (mouseY<screeny/2))) return 2;
      if (((mouseX>0) && (mouseX<screenx/2) && (mouseY>screeny/2) && (mouseY<screeny))) return 4;
      if ((mouseX>screenx/2) && (mouseX<screenx) && (mouseY>screeny/2) && (mouseY<screeny)) return 3;
    }
    if ((currentview>1) && canGo)
        {if ((mouseX>0) && (mouseX<screenx) && (mouseY>45) && (mouseY<screeny)) return currentview;}
    if (!viewports) return 0;
    return -1;
}

//
// Move the selected viewport or selected model
// 0=perspective view, 2=Top left, 3=bottom left, 4=bottom right
// Movetype=FALSE -> Move the view, TRUE -> Move the selected model
void viewprt::moveViewport(int viewport, float mouseX, float mouseY, bool movetype)
{
   video::IVideoDriver* driver = device->getVideoDriver();
   f32 screenx = driver->getScreenSize().Width;
   f32 screeny = driver->getScreenSize().Height;
   screenx-=viewrightspace;
   device->getSceneManager()->setActiveCamera ( Camera[viewport] ); // Select the active cam
   if (!moveviews){
   switch (viewport)
   {
   case 0:
        {
          if (moveitem) // For moving the items in the perspective viewport
          { Camera[0]->setInputReceiverEnabled(false);
            float distance=Camera[0]->getPosition().getDistanceFrom(Camera[0]->getTarget());
            core::vector3df vect = Camera[0]->getTarget() - Camera[0]->getPosition();
            vect = vect.getHorizontalAngle();

            float MX=Model->getPosition().X;
            float MY=Model->getPosition().Y;
            float MZ=Model->getPosition().Z;
            float PX=-((mouseX/screenx)*(distance*2))-(distance);
            float PY=((mouseY/screeny)*(distance*2))-(distance);

            if (!perspalternate) // Left mouse button, move the X-Z value of the model
              { vector3df resultvector = vector3df(PX,0,PY);
                if (setOffset)
                     {  oldcampos[0]=vector3df(MX,MY,MZ);
                        resultvector.rotateXZBy(-vect.Y,Model->getPosition());
                        oldcampos[1]=oldcampos[0]+resultvector;
                        setOffset=false;
                     } else
                     { resultvector.rotateXZBy(-vect.Y,oldcampos[0]);
                       if (Model) Model->setPosition(oldcampos[1]-resultvector);
                     }
              } else
              { vector3df resultvector = vector3df(0,PY,0);
                if (setOffset) // Right mouse button, move the Y value of the model
                     { oldcampos[0]=vector3df(MX,MY+PY,MZ);
                       setOffset=false;
                     } else
                     { if (Model) Model->setPosition(oldcampos[0]-resultvector);
                     }
              }
          }
          if (rotateitem) // For rotating objects in the perspective viewport
          { Camera[0]->setInputReceiverEnabled(false);
            float RX=Model->getRotation().X;
            float RY=Model->getRotation().Y;
            float RZ=Model->getRotation().Z;
            float PX=((mouseX/screenx)*360)-180;
            float PY=-((mouseY/screeny)*90)-45;
            if (!perspalternate)
            {  if (validrotation) {oldrot=Model->getRotation(); oldrot.X+=PY; oldrot.Y+=PX; validrotation=false;}
                 if (Model) Model->setRotation(core::vector3df(oldrot.X-PY,oldrot.Y-PX,RZ));
            } else
            {  if (validrotation) {oldrot=Model->getRotation(); oldrot.Z-=PX; validrotation=false;}
               if (Model) Model->setRotation(core::vector3df(RX,RY,oldrot.Z+PX));
            }
          }
        break;
         //printf("Top Righ viewport is selected\n");
        }
   case 2: // Top view processing
        {
         Camera[0]->setInputReceiverEnabled(false);
         float viewscale=0.0f;
         if (currentview==1) {viewscale=(viewscale2/2.0f);}
         else {viewscale=viewscale2;}
         camZ=(mouseX-(screenx/4))/(viewscale);
         //camX=-((mouseY-50)/screeny-50)*(screeny-50) / (viewscale*0.85f);
         camX=-((mouseY-50)-(screeny/1.35)) / (viewscale);
         core::vector3df currPos=core::vector3df(0,0,0);
         if (!movetype)
            {currPos = Camera[2]->getPosition();
            if (setOffset) {oldcampos[2]=currPos; setOffset=false; oldcampos[2].X+=camX; oldcampos[2].Z+=-camZ;}
            } else
            {currPos = Model->getPosition();
            if (setOffset) {oldcampos[2]=currPos; setOffset=false; oldcampos[2].X+=-camX; oldcampos[2].Z+=camZ;}
            }

         //printf("Top left viewport selected \n");
         if (!movetype)
            {Camera[2]->setPosition(core::vector3df(oldcampos[2].X-camX,16000,(oldcampos[2].Z+camZ)));
             Camera[2]->setTarget(core::vector3df(oldcampos[2].X-camX,0,(oldcampos[2].Z+camZ)));
            } else
            {if (moveitem && Model) Model->setPosition(core::vector3df(oldcampos[2].X+camX,oldcampos[2].Y,oldcampos[2].Z-camZ));
             if (rotateitem)
                     { float RX=Model->getRotation().X;
                       float RY=Model->getRotation().Y;
                       float RZ=Model->getRotation().Z;
                       float PX=((mouseX/(screenx/2))*500)-250;
                       if (validrotation) {oldrot=Model->getRotation(); oldrot.Y+=PX; validrotation=false;}
                      if (Model) Model->setRotation(core::vector3df(RX,oldrot.Y-PX,RZ));
                      }
             }
         break;
        }
   case 3: //Front view processing
        {
         Camera[0]->setInputReceiverEnabled(false);
         float viewscale=0.0f;
         if (currentview==1) viewscale=viewscale3/2.0f; else viewscale=viewscale3;
         camX=(mouseX-(screenx/4))/(viewscale);
         camY=(mouseY-(10+(screeny/1.35f))) / (viewscale);
         core::vector3df currPos=core::vector3df(0,0,0);
         if (!movetype)
            {currPos = Camera[3]->getPosition();
             if (setOffset) {oldcampos[3]=currPos; setOffset=false; oldcampos[3].X+=-camX; oldcampos[3].Y+=-camY;}
            } else
            {currPos = Model->getPosition();
             if (setOffset) {oldcampos[3]=currPos; setOffset=false; oldcampos[3].X+=camX; oldcampos[3].Y+=camY;}
            }

         //printf("Bottom left viewport is selected\n",camX, camY);
         if (!movetype)
            {Camera[3]->setPosition(core::vector3df((oldcampos[3].X+camX),oldcampos[3].Y+camY,16000));
             Camera[3]->setTarget(core::vector3df((oldcampos[3].X+camX),oldcampos[3].Y+camY,0));
            } else
            {if (moveitem && Model) Model->setPosition(core::vector3df((oldcampos[3].X-camX),oldcampos[3].Y-camY,oldcampos[3].Z));
             if (rotateitem)
                     { float RX=Model->getRotation().X;
                       float RY=Model->getRotation().Y;
                       float RZ=Model->getRotation().Z;
                       float PX=((mouseX/(screenx/2))*500)-250;
                       if (validrotation) {oldrot=Model->getRotation(); oldrot.Z+=PX; validrotation=false;}
                      if (Model) Model->setRotation(core::vector3df(RX,RY,(oldrot.Z-PX)));
                      }
             }
         break;
        }
   case 4: // Side view processing
        {
         Camera[0]->setInputReceiverEnabled(false);
         camZ=0; camX=0;
         float viewscale=0.0f;
         if (currentview==1) viewscale=viewscale4/2.0f; else viewscale=viewscale4;
         camZ=(mouseX-(screenx/1.35f))/(viewscale);
         camY=-(mouseY-(10+(screeny/1.35f))) / (viewscale);
         core::vector3df currPos;
         if (!movetype)
            {currPos = Camera[4]->getPosition();
             if (setOffset) {oldcampos[4]=currPos; setOffset=false; oldcampos[4].Y+=camY; oldcampos[4].Z+=-camZ;}
            } else
            {currPos = Model->getPosition();
             if (setOffset) {oldcampos[4]=currPos; setOffset=false; oldcampos[4].Y+=-camY; oldcampos[4].Z+=camZ;}
            }

         //printf("Bottom right viewport is selected\n",camY, camZ);
         if (!movetype)
            {Camera[4]->setPosition(core::vector3df(-16000,oldcampos[4].Y-camY,oldcampos[4].Z+camZ));
             Camera[4]->setTarget(core::vector3df(0,oldcampos[4].Y-camY,oldcampos[4].Z+camZ));
            } else
            {if (moveitem && Model) Model->setPosition(core::vector3df(oldcampos[4].X,oldcampos[4].Y+camY,oldcampos[4].Z-camZ));
             if (rotateitem)
                     { float RX=Model->getRotation().X;
                       float RY=Model->getRotation().Y;
                       float RZ=Model->getRotation().Z;
                       float PX=(((mouseX/2)/((screenx-120)/2))*720)-360;
                       if (validrotation) {oldrot=Model->getRotation(); oldrot.X-=PX; validrotation=false;}
                      if (Model) Model->setRotation(core::vector3df(oldrot.X+PX,RY,RY));
                      }
              }
         break;
        }
        case 5:
             {if (!viewports) viewport=0;} break;

  }  }
}



void viewprt::drawGrid(int view, bool flag)
{    video::IVideoDriver* driver = device->getVideoDriver();
     switch (view)
     { case 0 :
           {ground->setVisible(groundvisibility); grid->setVisible(gridvispers);
            grid->setRotation(core::vector3df(0,0,0));
            grid->setPosition(core::vector3df(0,0,0));
            grid->SetSize(2000); grid->SetSpacing(10);
            break; } // reset the grid rotation & position
       case 2 :
           {ground->setVisible(false); grid->setVisible(gridvisibility);

            grid->setRotation(core::vector3df(0,0,0));
            grid->setPosition(core::vector3df(0,-16000,0));
            if ((viewscale2>0.6f) && (viewscale2<1000.0f)) {grid->SetSize(2000); grid->SetSpacing(10);}
            if ((viewscale2>0.1f) && viewscale2<0.6f) {grid->SetSize(8000); grid->SetSpacing(100);}
            if ((viewscale2>0.0001f) && viewscale2<0.1f) {grid->SetSize(32000); grid->SetSpacing(1000);}
            break;} // reset the grid rotation & position
       case 3 :
           {ground->setVisible(false); grid->setVisible(gridvisibility);
            grid->setRotation(core::vector3df(-90,0,0));
            grid->setPosition(core::vector3df(0,0,-16000));
            if ((viewscale3>0.6f) && (viewscale3<1000.0f)) {grid->SetSize(2000); grid->SetSpacing(10);}
            if ((viewscale3>0.1f) && viewscale3<0.6f) {grid->SetSize(8000); grid->SetSpacing(100);}
            if ((viewscale3>0.0001f) && viewscale3<0.1f) {grid->SetSize(32000); grid->SetSpacing(1000);}
            break;} // rotate the grid to be in the background
       case 4 :
           {ground->setVisible(false); grid->setVisible(gridvisibility);
            grid->setRotation(core::vector3df(0,0,-90));
            grid->setPosition(core::vector3df(16000,0,0));
            if ((viewscale4>0.6f) && (viewscale4<1000.0f)) {grid->SetSize(2000); grid->SetSpacing(10);}
            if ((viewscale4>0.1f) && viewscale4<0.6f) {grid->SetSize(8000); grid->SetSpacing(100);}
            if ((viewscale4>0.0001f) && viewscale4<0.1f) {grid->SetSize(32000); grid->SetSpacing(1000);}
            break;} // rotate the grid to be in the background
     }
     if (flag)
     {SkyBox->setVisible(false);
      grid->setMaterialFlag(EMF_LIGHTING,false);
      //grid->setMaterialFlag(EMF_WIREFRAME,true);
      //grid->setMaterialTexture(0,0);
      //bill->setVisible(true);
     } else
     { //bill->setVisible(false);
       //grid->setMaterialFlag(EMF_WIREFRAME,false);
       //grid->setMaterialTexture(0, driver->getTexture("media/ui/terrain-texture.jpg"));
     }
}

void viewprt::drawPerspectiveView()
{
     video::IVideoDriver* driver = device->getVideoDriver();
     scene::ISceneManager* smgr = device->getSceneManager();
     IGUIEnvironment* env = device->getGUIEnvironment();
     s32 screenx = driver->getScreenSize().Width;
     s32 screeny = driver->getScreenSize().Height;
     screenx-=viewrightspace;
     f32 aspect = (f32)screenx/(f32)(screeny-50); // this correct the display ratio

     Camera[0]->setAspectRatio(aspect);
     grid->SetAxisLineXColor(SColor(0,150,0,0));
     grid->SetAxisLineZColor(SColor(0,0,0,150));
     drawGrid(0,false);
     setActiveCamera ( Camera[0] );
}

void viewprt::drawTopView()
{    // left top view - TOP
    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* env = device->getGUIEnvironment();

    core::matrix4 mat2;

    s32 screenx = driver->getScreenSize().Width;
    s32 screeny = driver->getScreenSize().Height;
    screenx-=viewrightspace;
    mat2.buildProjectionMatrixOrthoLH(((screenx)/(viewscale2)),((screeny)/(viewscale2)),-32000.0f, 32000.0f);
    Camera[2]->setProjectionMatrix(mat2);
    grid->SetAxisLineXColor(SColor(0,150,0,0));
    grid->SetAxisLineZColor(SColor(0,0,0,150));
    drawGrid(2,true);
    smgr->setActiveCamera ( Camera[2] );


}

void viewprt::drawFrontView()
{    // left top view - TOP
    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* env = device->getGUIEnvironment();

    core::matrix4 mat2;
    s32 screenx = driver->getScreenSize().Width;
    s32 screeny = driver->getScreenSize().Height;
    screenx-=viewrightspace;
    //mat2.buildProjectionMatrixOrthoLH(((screenx/2)/(viewscale3)),((screeny/2)/(viewscale3)),-4000.0f, 4000.0f);
    mat2.buildProjectionMatrixOrthoLH(((screenx)/(viewscale3)),((screeny)/(viewscale3)),-32000.0f, 32000.0f);
    Camera[3]->setProjectionMatrix(mat2);
    grid->SetAxisLineXColor(SColor(0,150,0,0));
    grid->SetAxisLineZColor(SColor(0,0,150,0));
    drawGrid(3,true);
    smgr->setActiveCamera ( Camera[3] );


}

void viewprt::drawSideView()
{    // left top view - TOP
    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* env = device->getGUIEnvironment();

    core::matrix4 mat2;
    s32 screenx = driver->getScreenSize().Width;
    s32 screeny = driver->getScreenSize().Height;
    screenx-=viewrightspace;
    //mat2.buildProjectionMatrixOrthoLH(((screenx/2)/(viewscale4)),((screeny/2)/(viewscale4)),-4000.0f, 4000.0f);
    mat2.buildProjectionMatrixOrthoLH(((screenx)/(viewscale4)),((screeny)/(viewscale4)),-32000.0f, 32000.0f);
    Camera[4]->setProjectionMatrix(mat2);
    grid->SetAxisLineZColor(SColor(0,0,0,150));
    grid->SetAxisLineXColor(SColor(0,0,150,0));
    drawGrid(4,true);
    smgr->setActiveCamera ( Camera[4] );
}

void viewprt::drawSingle(int view)
{
     video::IVideoDriver* driver = device->getVideoDriver();
     scene::ISceneManager* smgr = device->getSceneManager();
     IGUIEnvironment* env = device->getGUIEnvironment();
     s32 screenx = driver->getScreenSize().Width;
     s32 screeny = driver->getScreenSize().Height;
     driver->beginScene(true, true, video::SColor(150,50,50,50));

     switch (view)
     {case 0 : {drawPerspectiveView(); break;}
      case 2 : {drawTopView(); break;}
      case 3 : {drawFrontView(); break;}
      case 4 : {drawSideView(); break;}
     }
     driver->setViewPort(core::rect<s32>(0,50,screenx-viewrightspace,screeny));
     smgr->drawAll();
     driver->setViewPort(core::rect<s32>(0,0,driver->getScreenSize().Width,screeny));
     env->drawAll();
     driver->endScene();
}

void viewprt::drawViewPorts(float scale2, float scale3, float scale4, float scalepers)
{
     // Left top view
    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* env = device->getGUIEnvironment();

         s32 screenx = driver->getScreenSize().Width;
         s32 screeny = driver->getScreenSize().Height;
         screenx-=viewrightspace;
         bool skyb = SkyBox->isVisible();


         // Right top view - perspective
         driver->beginScene(true, true, video::SColor(150,50,50,50));
         if (skyb) SkyBox->setVisible(true); else SkyBox->setVisible(false); // show the skybox

         drawPerspectiveView();
         driver->setViewPort(core::rect<s32>((screenx/2),50,screenx,(screeny/2)-5));
         smgr->drawAll();
          // hide the skybox not working for ortho viewports
         drawTopView();
         driver->setViewPort(core::rect<s32>(0,50,screenx/2,(screeny/2)-5));
         smgr->drawAll();

         drawSideView();
         driver->setViewPort(core::rect<s32>(0,(screeny/2),screenx/2,screeny));
         smgr->drawAll();

         drawFrontView();
         driver->setViewPort(core::rect<s32>((screenx/2),(screeny/2)+2,screenx,screeny));
         smgr->drawAll();

         // restore the view for GUI drawing
         driver->setViewPort(core::rect<s32>(0,0,driver->getScreenSize().Width,screeny));
         bill->setVisible(false);
         // draw the viewport boundary
         smgr->setActiveCamera(Camera[0]);
         driver->draw2DRectangle(video::SColor(255,20,20,20),
				core::rect<s32>(0, screeny/2-5, screenx, screeny/2-1));
         driver->draw2DRectangle(video::SColor(255,20,20,20),
				core::rect<s32>(screenx/2-2, 50, screenx/2+2, screeny));
		 if (skyb) SkyBox->setVisible(true); else SkyBox->setVisible(false); // redisplay the viewport

		 env->drawAll();
		 driver->endScene();
}

void viewprt::createSkyBox()
{ // add skybox
   scene::ISceneManager* smgr = device->getSceneManager();
   video::IVideoDriver* driver = device->getVideoDriver();
   SkyBox = smgr->addSkyBoxSceneNode(
   	    driver->getTexture("media/skybox/irrlicht2_up.jpg"),
		driver->getTexture("media/skybox/irrlicht2_dn.jpg"),
		driver->getTexture("media/skybox/irrlicht2_rt.jpg"),
		driver->getTexture("media/skybox/irrlicht2_lf.jpg"),
		driver->getTexture("media/skybox/irrlicht2_ft.jpg"),
		driver->getTexture("media/skybox/irrlicht2_bk.jpg"));
   SkyBox->setVisible(false); // hide the Skybox per default;
   SkyBox->setRotation(vector3df(0,0,0));
   ISceneNode * Theroot = smgr->getRootSceneNode();
}

void viewprt::createGround()
{
   // Try to add a ground plane grid
   scene::ISceneManager* smgr = device->getSceneManager();
   video::IVideoDriver* driver = device->getVideoDriver();
   IAnimatedMesh *plane = smgr->addHillPlaneMesh("test",core::dimension2d<f32>(20,20),core::dimension2d<u32>(60,60));
   ground = smgr->addWaterSurfaceSceneNode(plane->getMesh(0), 0.0f, 1000, 10);
   ground->setPosition(core::vector3df(0,-1,0));
   ground->setMaterialFlag(EMF_LIGHTING,false);
   ground->setMaterialFlag(EMF_BACK_FACE_CULLING,false);
   ground->setMaterialType(EMT_SOLID);
   ground->setMaterialTexture(0, driver->getTexture("media/ui/terrain-texture.jpg"));
}


void viewprt::createGrid()
{
    scene::ISceneManager* smgr = device->getSceneManager();
    grid = new CGridSceneNode(smgr->getRootSceneNode(), smgr);
    grid->SetAxisLineActive(true);
    grid->SetGridColor(SColor(0,100,100,100));
    grid->SetAccentlineColor(SColor(0,135,135,155));
    grid->SetSize(2000); grid->SetSpacing(10);
    grid->drop();
}


void viewprt::createBillboard()
{
   //grid->setMaterialTexture(0, driver->getTexture("../../media/irrlicht2_dn.jpg"));
    // add billboard
   scene::ISceneManager* smgr = device->getSceneManager();
   video::IVideoDriver* driver = device->getVideoDriver();
   bill = smgr->addBillboardSceneNode();
   bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
   bill->setMaterialTexture(0, driver->getTexture("media/ui/particle.bmp"));
   bill->setMaterialFlag(video::EMF_LIGHTING, false);
   bill->setMaterialFlag(video::EMF_ZBUFFER, false);
   bill->setSize(core::dimension2d<f32>(5.0f, 5.0f));
   bill->setVisible(false);
}

