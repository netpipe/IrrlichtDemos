#include "Renderer.h"

/*----------------------------------------------------------------------------*/
Renderer::Renderer(IrrlichtDevice* devic, SColor diffuse)
{
  device = devic;
  driver = device->getVideoDriver();
  smgr = device->getSceneManager();
  Diffuse = diffuse;
  META = smgr->createMetaTriangleSelector();
  device->getCursorControl()->setVisible(false);
  device->setWindowCaption(L"Occlusion Culling");

  //load map
  cull = new OcclusionCulling(smgr, "data/models/occlusion/map.bxp");
}

/*----------------------------------------------------------------------------*/
Renderer::~Renderer()
{
    delete cull;
    device->closeDevice();
}

/*----------------------------------------------------------------------------*/
void Renderer::initGUI()
{
    guienv = device->getGUIEnvironment();
    IGUIFont* font = guienv->getFont("data/models/occlusion/cityblueprint.bmp");
    guienv->getSkin()->setFont(font);

    texte = guienv->addStaticText(L"",
                  core::rect<s32>(10,10,200,100), false, true, 0, -1, false);
    texte->setOverrideColor(video::SColor(255,255,150,100));
}

/*----------------------------------------------------------------------------*/
void Renderer::drawGUI()
{
      vector3df pos(cam->getPosition());
      char c[255];
      sprintf(c,"FPS:%d\nTris:%d\n%.2f,%.2f,%.2f", driver->getFPS(),
           driver->getPrimitiveCountDrawn(), pos.X, pos.Y, pos.Z);
      stringw s(c);
      texte->setText(s.c_str());

    guienv->drawAll();
}

/*----------------------------------------------------------------------------*/
void Renderer::addCam(f32 speed, vector3df pos, vector3df tgt, bool gravity)
{
    cam = smgr->addCameraSceneNodeFPS(0, 100.0f, speed);
                             cam->setPosition(pos);
                             cam->setTarget(tgt);
                             cam->setFarValue(30000);

    if (gravity)
    {
     scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(META,cam,
          core::vector3df(10,40,10), // ellipsoidRadius
          core::vector3df(0,-10.0f,0), // gravityPerSecond
          core::vector3df(0,24,0) // ellipsoidTranslation
                             // slidingValue = 0.0005f
     );
     cam->addAnimator(anim);
     anim->drop();
    }

    /* first person weapon
    IAnimatedMesh* mesh = smgr->getMesh("../bras/hands.b3d");
    IAnimatedMeshSceneNode *bras = smgr->addAnimatedMeshSceneNode(mesh);
    if (bras)
    {
 	  //bras->setFrameLoop(80, 100);
 	  //bras->setFrameLoop(60, 70);
	  //bras->setAnimationSpeed(10);
      bras->setParent(cam);
      //bras->setRotation(vector3df(0,180,0));
      bras->setPosition(vector3df(10,-20,25));
    }
    */
}

/*----------------------------------------------------------------------------*/
void Renderer::apply_light(SMaterial* mat)
{
  //SMaterial* mat = &terrain->getMaterial(0);
  //mat->EmissiveColor = SColor(0,200,162,97);
  mat->Lighting  = true;
  mat->FogEnable = true;
  //mat->SpecularColor = SColor(255, 255,255,255);
  //mat->AmbientColor =  SColor(0, 0,200,0);
}

/*----------------------------------------------------------------------------*/
void Renderer::addTerrain()
{
    terrain = smgr->addTerrainSceneNode("../bertrand/1H.png");
    terrain->setScale(core::vector3df(128.f, 8.f, 128.f));
    terrain->setPosition(vector3df(0,-600,0));

    //terrain->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    //terrain->setMaterialFlag(video::EMF_LIGHTING, true);
    terrain->setMaterialTexture(0, driver->getTexture("../bertrand/1T.png"));
    //terrain->scaleTexture(32.0f, 32.0f);
           SMaterial* mat = &terrain->getMaterial(0);
                      mat->EmissiveColor = SColor(0,100,100,100);
                      mat->Lighting  = true;
                      mat->FogEnable = true;

	// create triangle selector for the terrain
	scene::ITriangleSelector* selector = smgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(selector);
    META->addTriangleSelector(selector);
    selector->drop();
}

/*----------------------------------------------------------------------------*/
void Renderer::addMy3DScene(char* scene, f32 X, f32 Y, f32 Z)
{
   scene::IAnimatedMesh* mesh;
   mesh = smgr->getMesh(scene);

   scene::ISceneNode* my3d = smgr->addOctTreeSceneNode(mesh->getMesh(0));
   //my3d->setPosition(irr::core::vector3df(X,terrain->getHeight(X,Z)+Y,Z));
   my3d->setPosition(irr::core::vector3df(X,Y,Z));
   //my3d->setScale(irr::core::vector3df(2,2,2));
   //my3d->setRotation(irr::core::vector3df(0,0,0));
   my3d->setMaterialFlag(video::EMF_LIGHTING, false);
           //SMaterial* mat = &my3d->getMaterial(0);
                      //mat->EmissiveColor = SColor(0,100,100,100);
                      //mat->Lighting  = false;
                      //mat->FogEnable = true;
/*
           apply_light(mat);
/*
my3d->getMaterial(0).SpecularColor = irr::video::SColor(255,255,255,255);
my3d->getMaterial(0).Shininess = 255.0f;
my3d->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
*/
/*
	scene::ITriangleSelector* selector = 0;
	selector = smgr->createOctTreeTriangleSelector(mesh->getMesh(0), my3d, 128);
    my3d->setTriangleSelector(selector);

    META->addTriangleSelector(selector);
    selector->drop();
*/
}

/*----------------------------------------------------------------------------*/
void Renderer::addSky(char *skytex)
{
  scene::ISceneNode* skydome = smgr->addSkyDomeSceneNode(NULL, 8,4, 1.f,1.3f, NULL, -1);

  video::IImage *img1 = driver->createImageFromFile(skytex);
  video::ITexture* tex = driver->addTexture(skytex,img1);
				   tex->grab();
				   skydome->setMaterialTexture(0, tex);
				   tex->drop();

	  /*obtient le 220e pixel: il détermine le fog*/
	  img1->lock();
	  video::SColor p1 = img1->getPixel(0,180);
	  img1->unlock();
	  driver->setFog(p1, true, 1000.f, 30000.f);
}

/*----------------------------------------------------------------------------*/
void Renderer::addLight()
{
  ILightSceneNode* light;

  light = smgr->addLightSceneNode(  0, core::vector3df(0,512,0),
      video::SColorf(1.f,1.f,1.f, 1.0f), 300.0f);
  //light->getLightData().AmbientColor = video::SColorf(0.5f, 1.f, 0.5f, 1.0f);

  light = smgr->addLightSceneNode( 0, core::vector3df(1024,512,1024),
      video::SColorf(1.f,1.f,1.f, 1.0f), 300.0f);
/*
  ILightSceneNode *light = smgr->addLightSceneNode(
      0, core::vector3df(50,50,50),
      video::SColorf(1.f, 0.7f, 0.3f, 1.f), 20.0f);
  //light->getLightData().AmbientColor = video::SColorf(0.5f, 1.f, 0.5f, 1.0f);
* /

   ILightSceneNode * light = smgr->addLightSceneNode(
      0, core::vector3df(0,1000,0),
      //video::SColorf(1.f,0.9f,0.65f, 1.f), 2000.0f);
      video::SColorf(.2f,.6f,1.f, 1.f), 2000.0f); //bleu pour la nuit
*/
  //émission ombres bleues
  //light->getLightData().AmbientColor = video::SColorf(0.1f,.1f,.5f, 1.0f);
  //light->getLightData().AmbientColor = video::SColorf(.1f,.1f,1.f, .1f); //tout bleu pour la nuit
  //light->getLightData().SpecularColor = video::SColorf(1.f,1.f,1.f, 1.0f);
}



/*----------------------------------------------------------------------------*/
void Renderer::addWater(char* texture)
{
  /* méthode 1 avec addHillPlaneMesh(). animé.
  IAnimatedMesh* mesh = smgr->addHillPlaneMesh("myHill",
      core::dimension2d<f32>(10000,10000),
      core::dimension2d<u32>(16,16), 0, 0,
      core::dimension2d<f32>(0,0),
      core::dimension2d<f32>(200,200));

   u8 OPACITY = 100;
   smgr->getMeshManipulator()->setVertexColorAlpha(mesh->getMesh(0), OPACITY);

   scene::ISceneNode* node = 0;
   node = smgr->addWaterSurfaceSceneNode(mesh->getMesh(0), 3.0f, 300.0f, 30.0f);
   node->setPosition(core::vector3df(0,7,0));
   node->setScale(core::vector3df(64,0.1,64));

   node->setMaterialTexture(0, driver->getTexture(texture));
   node->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
   node->setMaterialFlag(EMF_FOG_ENABLE, true);
   //node->getMaterial(0).getTextureMatrix(0).setTextureScale(200,200);

/* Méthode 2 avec addcubeSceneNode()*/
  scene::ISceneNode* waterNode = 0;
      waterNode = smgr->addCubeSceneNode();
      waterNode->setMaterialFlag(EMF_LIGHTING, false);
      waterNode->setMaterialFlag(EMF_FOG_ENABLE, true);
      waterNode->setPosition(core::vector3df(0,0,0));
      waterNode->setScale(core::vector3df(100000,0.1,100000));
      waterNode->setRotation(core::vector3df(0,0,0));
      waterNode->setMaterialTexture(0, driver->getTexture(texture));
      waterNode->getMaterial(0).getTextureMatrix(0).setTextureScale(200,200);
      //waterNode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
      //waterNode->setMaterialType(video::EMT_TRANSPARENT_REFLECTION_2_LAYER);

}

/*----------------------------------------------------------------------------*/
void Renderer::loadModel(char *modelfile, vector3df pos)
{
  IAnimatedMesh* mesh = smgr->getMesh(modelfile);
  IAnimatedMeshSceneNode *node;
  node = smgr->addAnimatedMeshSceneNode(mesh);
  if (node)
  {
    //node->setScale(vector3df(1,1,1));
    node->setPosition(pos);
    //node->setFrameLoop(206, 250);
    //node->setAnimationSpeed(30);
    node->setMaterialFlag(video::EMF_LIGHTING, true);
    //node->setMaterialTexture(0, driver->getTexture("models/nskinwh.jpg"));

    //smgr->addTextSceneNode(font, L"ninja", SColor(255,230,10,10), node, vector3df(0,100,0), -1);

          SMaterial *mat= &node->getMaterial(0); //fog
          apply_light(mat);
              //mat->Lighting      = true;
              //mat->EmissiveColor = Diffuse;
              //mat->DiffuseColor  = Diffuse;
              //mat->FogEnable     = true;
          //if (texisalpha==1) node->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

	scene::ITriangleSelector* selector = 0;
	selector = smgr->createOctTreeTriangleSelector(mesh->getMesh(0), node, 128);
    node->setTriangleSelector(selector);

    META->addTriangleSelector(selector);
    selector->drop();
  }
}

/*----------------------------------------------------------------------------*/
void Renderer::loadAnimatedModel(char *modelfile, vector3df pos)
{
  IAnimatedMesh* mesh = smgr->getMesh(modelfile);
  IAnimatedMeshSceneNode *node;
  node = smgr->addAnimatedMeshSceneNode(mesh);
  if (node)
  {
    node->setScale(vector3df(16,16,16));
    node->setPosition(pos);
    node->setFrameLoop(206, 250);
    node->setAnimationSpeed(30);
    node->setMaterialFlag(video::EMF_LIGHTING, true);
    //node->setMaterialTexture(0, driver->getTexture("models/nskinwh.jpg"));

    //smgr->addTextSceneNode(font, L"ninja", SColor(255,230,10,10), node, vector3df(0,100,0), -1);

          SMaterial *mat= &node->getMaterial(0); //fog
          apply_light(mat);
              //mat->Lighting      = true;
              //mat->EmissiveColor = Diffuse;
              //mat->DiffuseColor  = Diffuse;
              //mat->FogEnable     = true;
          //if (texisalpha==1) node->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
  }
}

/*----------------------------------------------------------------------------*/
void Renderer::updateCulling()
{
 cull->update(cam->getPosition());
}

