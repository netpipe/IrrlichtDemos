#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

/*==============================================================================
  Receiver
==============================================================================*/
class myEventReceiver : public IEventReceiver
{
 bool KeyDown[KEY_KEY_CODES_COUNT];

public:
 bool LEFTBUTTONCLICKED;

 myEventReceiver()
 {
   for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
     KeyDown[i] = false;

   LEFTBUTTONCLICKED = false;
 }

 bool isKeyDown(EKEY_CODE keyCode) const
 {
   return KeyDown[keyCode];
 }

 bool OnEvent(const SEvent& event)
 {
  if (event.EventType == irr::EET_KEY_INPUT_EVENT)
  {
    KeyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
    return false;
  }
  else if (event.EventType == EET_MOUSE_INPUT_EVENT)
  {
    if (event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN)
      LEFTBUTTONCLICKED = true;
    else if(event.MouseInput.Event==EMIE_LMOUSE_LEFT_UP)
     LEFTBUTTONCLICKED = false;
  }
  return false;
 }
};
/*==============================================================================
  draw red box
==============================================================================*/
void tracebox(core::aabbox3d<f32>box, IVideoDriver* driver)
{
    matrix4 mat;
    SMaterial material;
    material.Lighting = false;
    driver->setMaterial(material);
    driver->setTransform(ETS_WORLD, mat);
    driver->draw3DBox (box, SColor(0xffff0000));
}
/*==============================================================================
  get filename from material path
==============================================================================*/
char* baseFileName(char *path)
{
  char *c = path, *ret = path;
  if(c == 0) return 0;
  while(*c)
  {
    if(*c == '\\') ret = c+1;
    c++;
  }
  return ret;
}
/*==============================================================================
  Entry point
==============================================================================*/
int main()
{

  dimension2du Res (600,400);
  bool fullscreen = false;
  IrrlichtDevice* device = createDevice(EDT_OPENGL, Res, 32, fullscreen);
  if (!device) return 0;

  myEventReceiver r;
  device->setEventReceiver(&r);

  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr  = device->getSceneManager();

  device->getCursorControl()->setVisible(false);
  device->setWindowCaption(L"test");

  // GUI

  IGUIEnvironment* guienv = device->getGUIEnvironment();

  IGUIStaticText* txt;
  txt = guienv->addStaticText(L"", core::rect<s32>(100,Res.Height-120,Res.Width-200,Res.Height-80),
                                    false, false, 0, 101, false);
  txt->setOverrideColor(video::SColor(0xff897dad));


  // crosshair

  video::ITexture* tex;
  tex = driver->getTexture("cross.tga");
  position2d<s32> texPos((Res.Width - 32)/2, (Res.Height - 32)/2);
  rect<s32> sourceRect(0,0, 32,32);


  // collision

  ISceneCollisionManager* cmgr;
  cmgr = smgr->getSceneCollisionManager();
  scene::ITriangleSelector* bibiSelector;


  // bibi is the colored guy made of bounding boxes

  IAnimatedMeshSceneNode* bibi;
  IAnimatedMesh* mesh = smgr->getMesh("bibi/bibi.b3d");
  bibi = smgr->addAnimatedMeshSceneNode(mesh);
  if (bibi)
  {
    bibi->setFrameLoop(1,1);
    bibi->setLoopMode(true);
    bibi->setAnimationSpeed(1);
    bibi->setMaterialFlag(video::EMF_LIGHTING, false);
    //bibi->setDebugDataVisible(EDS_BBOX_BUFFERS);
    //bibi->setDebugDataVisible(EDS_BBOX);
    // collision
    bibi->setID(404);
    bibiSelector = smgr->createTriangleSelector(mesh->getMesh(0), bibi);
    bibi->setTriangleSelector(bibiSelector);
    bibiSelector->drop();
  }
  mesh->drop();


  // camera

  scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, .1f);
                           cam->setPosition(vector3df(0,50,100));
                           cam->setTarget(vector3df(0,0,0));
                           cam->setID(0);


  u32 elapsed = 0, then = device->getTimer()->getTime(), nextCheck = 0;

  while(device->run())
  {
    if (!device->isWindowActive()) device->sleep(200);

    if (r.isKeyDown(KEY_ESCAPE)) break;

    u32 now = device->getTimer()->getTime();

    driver->beginScene(true, true, 0);

    // move bibi

 //   vector3df pos = bibi->getPosition();
   // pos.X += .003f * elapsed;///1000.f;
  //  bibi->setPosition(pos);

    // see if its BB is traversed by line from camera. If it does, do deeper
    // checks to determine what part was hit

    core::line3d<f32> line;
    line.start = cam->getPosition();
    line.end = line.start + (cam->getTarget() - line.start).normalize() * 1000.0f;

    ISceneNode* nodeHit = NULL;
    nodeHit = cmgr->getSceneNodeFromRayBB(line);
    if (nodeHit)
	{
      s32 id = nodeHit->getID();
      if (id == 404)
      {
        // look deeper : check body parts BBs

        s32 frame = (s32)bibi->getFrameNr();

        IAnimatedMesh* mesh = bibi->getMesh();

        txt->setText(L"");

        //printf("%d meshbuffers\n", pMesh->getMeshBufferCount()); // 15
        for (u8 b=0; b<mesh->getMeshBufferCount(); ++b)
        {
          IMeshBuffer* mb = mesh->getMeshBuffer(b);

          if (mb->getVertexType() != video::EVT_STANDARD) continue;

          core::aabbox3d<f32>box = mb->getBoundingBox();

          tracebox(box, driver);

          // here it is sadly not true because the meshbuffer BB is still
          // at (0,0,0) after the node was moved :(
          if (box.intersectsWithLine(line))
          {
            video::SMaterial mat = mb->getMaterial();
            core::stringc texname = mat.getTexture(0)->getName();
            txt->setText(stringw(baseFileName((char*)texname.c_str())).c_str());
            break;
          } // if line intersects with meshbuffer bb
        } // for each mesbuffer
      } // if bibi was hit
    } // if a node was hit

    smgr->drawAll();

    driver->draw2DImage(tex, texPos, sourceRect, 0, SColor(0xffffa300), true);

    guienv->drawAll();

    driver->endScene();


    elapsed = now - then;
    then = now;

    if (elapsed > 20)
      device->sleep(20 - (u32)(elapsed/1000.f));
  }

  device->closeDevice();

  return 0;
}
