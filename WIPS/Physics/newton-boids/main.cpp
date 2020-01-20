
#include "NewtonBoid.h"
#include "NewtonLeader.h"
#include "main.h"
#include <stdlib.h>
#include <time.h>
#include <vec3.h>
#include "NewtonLump.h"
#include "mathutils.h"


//////////////////////// globals ////////////////////////////////////
using namespace OpenSteer;

const Vec3 Vec3::zero;
const Vec3 Vec3::side;
const Vec3 Vec3::up;
const Vec3 Vec3::forward;

float g_angleMod = 0.0f;
float g_distMod = 0.0f;

IrrlichtDevice *device = NULL;
scene::IAnimatedMesh* fly = 0;
scene::IAnimatedMeshSceneNode* flyNode = 0;
IVideoDriver* driver = 0;
ISceneManager* smgr = 0;
NewtonWorld *nWorld = 0;
NewtonBoid* flys[MAX_FLYS];
vector3df worldSize;
int groundMaterialID;
int flyMaterialID;
IAnimatedMesh*	 boxMesh =0;
bool debugOn = false;
bool WFramesOn = false;
ICameraSceneNode* camera = NULL;
IGUIStaticText* pText = NULL;
IGUIStaticText* mText = NULL;
NewtonLeader g_leader ;
bool g_wander;
NewtonLump g_lumps[10];
KeyItem keyMap[14];

void LevelCollisionCallback (const NewtonBody* bodyWithTreeCollision,	const NewtonBody* body,
	const dFloat* vertex,	int vertexstrideInBytes, int indexCount, const int* indexArray){

}

int getrand(int range){
 	int rdm;
 	if (range==0) return 0;
 	rdm=  ((int) rand() % range);
	return   rdm;
}

vector3df getRandomForce(){
	vector3df t;
	int x = getrand(100);
	if (x < 50) x = -x ;
	int y = getrand(100);
	if (y < 50) y = -y ;
	int z = getrand(100);
	if (z < 50) z = -z ;
	t.X = (float) x/100;
	t.Y = (float) y/100;
	t.Z = (float) z/100;

	return t;
}

void createNewtonHolder(scene::IAnimatedMesh* iMeshBase){
	float ss = 250.0f;
	IAnimatedMeshSceneNode* boxNode = smgr->addAnimatedMeshSceneNode(iMeshBase);
	boxNode->setMaterialFlag(video::EMF_LIGHTING, false);
	boxNode->setMaterialTexture(0,driver->getTexture("assets/grid.tga"));
 	boxNode->setScale(vector3df(ss,ss,ss));
	boxNode->updateAbsolutePosition();
	NewtonCollision* newtonmap;
	newtonmap = NewtonCreateTreeCollision(nWorld, LevelCollisionCallback);
	NewtonConvexCollisionSetUserID(newtonmap,100);
	NewtonTreeCollisionBeginBuild(newtonmap);
	int  j;
	int v1i, v2i, v3i;
	IMeshBuffer *mb;
	vector3df vArray[3]; // vertex array (3*3 floats)
	int tmpCount = 0;
  int meshCount = iMeshBase->getMesh(0)->getMeshBufferCount();
	mb = iMeshBase->getMesh(0)->getMeshBuffer(0);
	video::S3DVertex* mb_vertices = (irr::video::S3DVertex*) mb->getVertices();
	u16* mb_indices  = mb->getIndices();
	int maxI = mb->getIndexCount();
  // add each triangle from the mesh
	for (j=0; j<mb->getIndexCount(); j+=3){
    v1i = mb_indices[j];
    v2i = mb_indices[j+1];
    v3i = mb_indices[j+2];
    // to make thing easier, her we can use engine data type
    vArray[0] = mb_vertices[v1i].Pos * IrrToNewton*ss;
    vArray[1] = mb_vertices[v2i].Pos * IrrToNewton*ss;
    vArray[2] = mb_vertices[v3i].Pos * IrrToNewton*ss;
    // make sure we do not add degenerated polygons to the tree
    vector3df e0 (vArray[1] - vArray[0]);
    vector3df e1 (vArray[2] - vArray[0]);
    // the area of the face is the cross product
    vector3df area (e0.crossProduct (e1));
    // skip faces with very small area
    float mag = area.dotProduct (area);
    if (mag > 1.0e-6f) {
      NewtonTreeCollisionAddFace(newtonmap, 3, &vArray[0].X, sizeof (vector3df), 1);
    }
	}
	// we definilte wan to optimize the mesh (improve performace and vehaviuor a lot)
	NewtonTreeCollisionEndBuild(newtonmap, 1);
	// create a ridid body to represent the world
	NewtonBody* newtonmapbody;
	newtonmapbody = NewtonCreateBody(nWorld, newtonmap);
	// release the collsion tree (this way the application does not have to do book keeping of Newton objects
	NewtonReleaseCollision (nWorld, newtonmap);
	// set the newton world size based on the bsp size
	vector3df boxP0, boxP1;
	matrix4 matrix;
	NewtonBodyGetMatrix (newtonmapbody, &matrix.M[0]);
  matrix.makeIdentity();
	NewtonCollisionCalculateAABB (newtonmap, &matrix.M[0],  &boxP0.X, &boxP1.X);
 	NewtonSetWorldSize (nWorld, &boxP0.X, &boxP1.X);
	// the world uses the ground material
	NewtonBodySetMaterialGroupID (newtonmapbody, groundMaterialID);
  worldSize.X = (boxP1.X-boxP0.X);
	worldSize.Y = (boxP1.Y - boxP0.Y);
	worldSize.Z = (boxP1.Z-boxP0.Z);
}

void _cdecl NewtonDebugCollision(const NewtonBody* body, int vertexCount, const float* FaceArray, int faceId){
	core::vector3df p0 (FaceArray[0], FaceArray[1], FaceArray[2] );

	const video::SColor c0(255,155,0,0);

	for (int i = 2; i < vertexCount; i ++)
	{
		core::vector3df p1( FaceArray[(i-1) * 3 + 0], FaceArray[(i-1) * 3 + 1], FaceArray[(i-1) * 3 + 2] );
		core::vector3df p2( FaceArray[i * 3 + 0], FaceArray[i * 3 + 1], FaceArray[i * 3 + 2] );

		core::triangle3df t;
		t.set(p1*NewtonToIrr,
			  p2*NewtonToIrr,
			  p0*NewtonToIrr);

	 	driver->draw3DTriangle(t, c0);
	}
}

void _cdecl NewtonDebugBody (const NewtonBody* body){


	matrix4 mat;
	SMaterial material;

	material.Texture1 = 0;
	material.Lighting = false;
 	driver->setTransform(video::ETS_WORLD, mat);
 	driver->setMaterial(material);
	NewtonBodyForEachPolygonDo(body,NewtonDebugCollision);



}

void killFlies(){
	for(int i=0;i<MAX_FLYS;i++)
	{

	   if (flys[i] != NULL)
	   {
			delete flys[i];
			flys[i] = NULL;


	   }
	}


}

void initFlies(){
	int i = 0;
	float x, y, z,xr,yr,zr;
	int dlimit = 40;
	x = y = z = 0.0f;
	xr = yr = zr = 0.0f;
	g_leader.setMaterials(flyMaterialID);
	g_leader.setFlyID(MAX_FLYS+100);
	static vector3df cv = vector3df(5,55,-0);
 	g_leader.Init(smgr,nWorld,&cv,vector3df(0,0,0),"assets//player.x",3.0f,1000.0f);
	float zscale = 25.0f;
  cv = vector3df(-100,75,100);
	g_lumps[0].Init(smgr,nWorld, &cv,vector3df(0,0,0), "assets\\crate3.x",zscale,1.0f);
  cv = vector3df(100,75,100);
	g_lumps[1].Init(smgr,nWorld, &cv,vector3df(0,0,0), "assets\\crate3.x",zscale,1.0f);
  cv = vector3df(-100,-75,100);
	g_lumps[2].Init(smgr,nWorld, &cv,vector3df(0,0,0), "assets\\crate3.x",zscale,1.0f);
  cv = vector3df(100,-75,100);
	g_lumps[3].Init(smgr,nWorld, &cv,vector3df(0,0,0), "assets\\crate3.x",zscale,1.0f);
  cv = vector3df(-100,75,-100);
	g_lumps[4].Init(smgr,nWorld, &cv,vector3df(0,0,0), "assets\\crate3.x",zscale,1.0f);
  cv = vector3df(100,75,-100);
	g_lumps[5].Init(smgr,nWorld, &cv,vector3df(0,0,0), "assets\\crate3.x",zscale,1.0f);
  cv = vector3df(-100,-75,-100);
	g_lumps[6].Init(smgr,nWorld, &cv,vector3df(0,0,0), "assets\\crate3.x",zscale,1.0f);
  cv = vector3df(100,-75,-100);
	g_lumps[7].Init(smgr,nWorld, &cv,vector3df(0,0,0), "assets\\crate3.x",zscale,1.0f);
  cv = vector3df(65,-75,-80);
	g_lumps[8].Init(smgr,nWorld, &cv,vector3df(0,0,0), "assets\\crate3.x",zscale,1.0f);
	vector3df rot(xr,yr,zr);
 	srand((unsigned) time(NULL));
	for(i=0; i<MAX_FLYS; i++){
		flys[i] = NULL;
		x = (float) getrand(dlimit);
		y = (float) getrand(dlimit);
		z = (float) getrand(dlimit);
		if (x > dlimit/2) x = -x;
		if (y > dlimit/2) y = -y;
		if (z > dlimit/2) z = -z;
		xr = (float) getrand(30);
		yr = (float) getrand(30);
		zr = (float) getrand(30);
		vector3df rot(xr,yr,zr);
 		flys[i] = new NewtonBoid();
		flys[i]->setMaterials(flyMaterialID);
		flys[i]->setFlyID(i);
    cv = vector3df(x,y,z);
		flys[i]->Init(smgr,nWorld,&cv,rot,"assets//player.x",3.0f,100.0f);
	}
	for(i=0;i<MAX_FLYS;i++){
 		flys[i]->setNeighborCount(MAX_FLYS);
 		flys[i]->setNeighbors(flys);
	}
}

class MyEventReceiver : public  IEventReceiver{
public:
	bool mainCameraOn;
	MyEventReceiver(){
		mainCameraOn = true;
	}
	virtual bool OnEvent(SEvent event){
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown){
			switch(event.KeyInput.Key){
        case KEY_ESCAPE:
          device->closeDevice();
          return true;
          break;
        case KEY_KEY_I:
          debugOn = !debugOn;
          return true;
          break;
        case KEY_KEY_F:
          WFramesOn = !WFramesOn;
          return true;
          break;
        case KEY_KEY_V:
          if (!mainCameraOn){
            smgr->setActiveCamera(camera);
            mainCameraOn = true;
          }else{
            smgr->setActiveCamera(flys[0]->m_followCam);
            mainCameraOn = false;
          }
          return true;
          break;
        case KEY_KEY_H:
          g_wander = !g_wander;
          return true;
          break;
        case KEY_PLUS:
          g_distMod += 1.0f;
          return true;
          break;
        case KEY_MINUS:
          g_distMod += -1.0f;
          return true;
          break;
      }
			for(int k=0;k<KEY_COUNT;k++){
				if (event.KeyInput.Key == keyMap[k].code) keyMap[k].state = true;
      }
		}// pressdown
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown){
			for(int k=0;k<KEY_COUNT;k++){
				if (event.KeyInput.Key == keyMap[k].code){
					keyMap[k].state = false;
					//break;
				}
			}
		}
		return false;
	}
};

int main(int argc, char* argv[]){
  video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
  MyEventReceiver receiver;
  gui::IGUIEnvironment* guienv;
  int lastFPS = -1;
  g_wander = false;

  keyMap[0].code = KEY_UP;
  keyMap[1].code = KEY_DOWN;
  keyMap[2].code = KEY_LEFT;
  keyMap[3].code = KEY_RIGHT;
  keyMap[4].code = KEY_KEY_W;
  keyMap[5].code = KEY_KEY_S;
  keyMap[6].code = KEY_KEY_A;
  keyMap[7].code = KEY_KEY_D;
  keyMap[8].code = KEY_SPACE;
  keyMap[9].code = KEY_PRIOR;
  keyMap[10].code = KEY_NEXT;
  keyMap[11].code = KEY_KEY_C;
  keyMap[12].code = KEY_END;
  keyMap[13].code = KEY_CONTROL;
  keyMap[14].code = KEY_KEY_B;

	device = createDevice(driverType, core::dimension2d<s32>(800, 600),	32, false, false, false, &receiver);
  if (device == 0) return 1; // could not create selected driver.

  guienv = device->getGUIEnvironment();
  pText = guienv->addStaticText(L"biteme",core::rect<int>(10,5,500,55), false);
  pText->setOverrideColor(video::SColor(255,50,50,45));
  pText->setOverrideFont(guienv->getFont("assets\\midfont.bmp"));

  mText = guienv->addStaticText(L"H -- Change State\nV -- Change Camera\nI  -- Show Axis\nF -- Show Wireframes\nArrow Keys Mouse  -- Move Camera",core::rect<int>(10,500,500,620), false);
  mText->setOverrideColor(video::SColor(255,240,40,35));
  mText->setOverrideFont(guienv->getFont("assets\\midfont.bmp"));

  driver = device->getVideoDriver();
  driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
  driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
  smgr = device->getSceneManager();

  camera =	smgr->addCameraSceneNodeFPS(NULL,150.0f,150.0f);
  camera->setPosition(vector3df(0,0,-200.0f));
  camera->setTarget(vector3df(0,0,0));

//NEWTON STUFF
  nWorld = NewtonCreate(NULL, NULL);
	NewtonSetSolverModel(nWorld, 8);
	NewtonSetFrictionModel(nWorld, 1);
  groundMaterialID = NewtonMaterialGetDefaultGroupID(nWorld);
  flyMaterialID = NewtonMaterialCreateGroupID(nWorld);
  NewtonMaterialSetDefaultFriction(nWorld, groundMaterialID, groundMaterialID, 0, 0);
  NewtonMaterialSetDefaultFriction(nWorld, flyMaterialID, groundMaterialID, 0, 0);
  NewtonMaterialSetDefaultFriction(nWorld, flyMaterialID, flyMaterialID, 0, 0);
  NewtonMaterialSetDefaultElasticity(nWorld, groundMaterialID, groundMaterialID, 1);
  NewtonMaterialSetDefaultElasticity(nWorld, flyMaterialID, groundMaterialID, 1);
  NewtonMaterialSetDefaultElasticity(nWorld, flyMaterialID, flyMaterialID, 1);

  boxMesh = smgr->getMesh("assets\\box.obj");
  createNewtonHolder(boxMesh);
  initFlies();

  smgr->setActiveCamera(camera);
  device->getCursorControl()->setVisible(false);

	while(device->run()){
    if(device->isWindowActive()){
      driver->beginScene(true, true, video::SColor(0,20,0,0));
      smgr->drawAll();
      if(WFramesOn) NewtonWorldForEachBodyDo(nWorld, NewtonDebugBody);
      if(debugOn){
        for(int i=0; i<MAX_FLYS; i++){
          if(flys[i] != NULL){
            flys[i]->drawAppliedForce(driver);
            flys[i]->displayAxes(driver);
          }
        }
      }
      for(int i=0; i<MAX_FLYS; i++){
          if(flys[i] != NULL){
            flys[i]->m_target.x = g_leader.getPosition().X/IrrToNewton;
            flys[i]->m_target.y = g_leader.getPosition().Y/IrrToNewton;
            flys[i]->m_target.z = g_leader.getPosition().Z/IrrToNewton;
            if(g_distMod != 0.0f){
              flys[i]->addToSepDist(g_distMod);
              g_distMod = 0.0f;
            }
          }
      }
      wchar_t tText[124];
      if (flys[0] != NULL){
        vector3df tpos = flys[0]->getPosition();
        swprintf(tText,124,L"Position (%3.2f,%3.2f,%3.2f)", tpos.X,tpos.Y, tpos.Z);
        pText->setText(tText);
      }
      guienv->drawAll();
      driver->endScene();

      int fps = driver->getFPS();
      if(fps > 0.0f) NewtonUpdate(nWorld, 1.0f / fps);
      if(lastFPS != fps){
        core::stringw str = L"Flies Example [";
        str += driver->getName();
        str += "] FPS:";
        str += fps;
        device->setWindowCaption(str.c_str());
        lastFPS = fps;
      }
    }
	}

	NewtonDestroyAllBodies(nWorld);
	NewtonDestroy(nWorld);
//	killFlies();
	device->drop();

	return 0;
}







