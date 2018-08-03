#include <irrlicht.h>
#include <tokamak.h>
//#include <windows.h>
#include <time.h>

#define PI 3.1415926
#define CUBECOUNT 30
#define CUBEX 5.0
#define CUBEY 5.0
#define CUBEZ 5.0
#define CUBEMASS 50.0f
#define FLOORSIZE 300


typedef u64 LARGE_INTEGER;



irr::scene::ICameraSceneNode* camera = 0;
neSimulator *gSim = NULL;
neRigidBody *gCubes[CUBECOUNT];
neAnimatedBody *gFloor = NULL;

bool gbUseHFTimer;
int64_t gCurrentTime;
float gfTimeScale;



class MyEventReceiver : public irr::IEventReceiver
{
public:
 virtual bool OnEvent(const irr::SEvent &event)
 {
    if (camera)
       return camera->OnEvent(event);

    return false;
 }
};

class PhysicsCubeNode: public irr::scene::ISceneNode
{
  irr::core::aabbox3d<irr::f32> Box;
  irr::video::S3DVertex Vertices[8];
  irr::video::SMaterial Material;

public:

  PhysicsCubeNode(irr::scene::ISceneNode* parent,
     irr::scene::ISceneManager* mgr, irr::s32 id)
  : irr::scene::ISceneNode(parent, mgr, id)
  {
     Material.Wireframe = false;
     Material.Lighting = false;


Vertices[0] = irr::video::S3DVertex(
   -CUBEX/2,-CUBEY / 2,-CUBEZ/2, 1,1,0,
   irr::video::SColor(255,0,255,255), 0, 1);
 Vertices[1] = irr::video::S3DVertex(
   CUBEX/2,-CUBEY / 2,-CUBEZ/2, 1,0,0,
   irr::video::SColor(255,255,0,255), 1, 1);
 Vertices[2] = irr::video::S3DVertex(
   CUBEX/2,-CUBEY / 2, CUBEZ/2, 0,1,1,
   irr::video::SColor(255,255,255,0), 1, 0);
 Vertices[3] = irr::video::S3DVertex(
   -CUBEX/2,-CUBEY / 2, CUBEZ/2, 0,0,1,
   irr::video::SColor(255,0,255,0), 0, 0);
 Vertices[4] = irr::video::S3DVertex(
   -CUBEX/2, CUBEY / 2,-CUBEZ/2, 0,0,1,
   irr::video::SColor(255,0,0,255), 0, 0);
 Vertices[5] = irr::video::S3DVertex(
   CUBEX/2, CUBEY / 2,-CUBEZ/2, 0,0,1,
   irr::video::SColor(255,255,0,0), 0, 0);
 Vertices[6] = irr::video::S3DVertex(
   CUBEX/2, CUBEY / 2, CUBEZ/2, 0,0,1,
   irr::video::SColor(255,0,0,0), 0, 0);
 Vertices[7] = irr::video::S3DVertex(
   -CUBEX/2, CUBEY / 2, CUBEZ/2, 0,0,1,
   irr::video::SColor(255,255,255,255), 0, 0);

 Box.reset(Vertices[0].Pos);
 for (s32 i=1; i<8; ++i)
   Box.addInternalPoint(Vertices[i].Pos);
}


virtual void OnRegisterSceneNode()
{
  if (IsVisible)
    SceneManager->registerNodeForRendering(this);

  ISceneNode::OnRegisterSceneNode();
}


virtual void render()
 {
  irr::u16 indices[] = {0,1,2,
  0,2,3,
  1,6,2,
  1,5,6,
  0,5,1,
  0,4,5,
  0,3,7,
  0,7,4,
  2,7,3,
  2,6,7,
  4,6,5,
  4,7,6 };

  irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

  driver->setMaterial(Material);
  driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
  driver->drawIndexedTriangleList(&Vertices[0], 8, &indices[0], 12);
 }

 virtual const irr::core::aabbox3d<f32>& getBoundingBox() const
 {
   return Box;
 }

 virtual s32 getMaterialCount()
 {
  return 1;
 }

 virtual irr::video::SMaterial& getMaterial(s32 i)
 {
   return Material;
 }
};


class PhysicsFloorNode: public irr::scene::ISceneNode
 {
 private:
 irr::core::aabbox3d<irr::f32> Box;
 irr::video::S3DVertex Vertices[4];
 irr::video::SMaterial Material;

 public:

 PhysicsFloorNode(irr::scene::ISceneNode* parent,
  irr::scene::ISceneManager* mgr, irr::s32 id)
 : irr::scene::ISceneNode(parent, mgr, id)
 {
  Material.Wireframe = false;
  Material.Lighting = false;

  Vertices[0] = irr::video::S3DVertex(
    -FLOORSIZE/2,0.0,-FLOORSIZE/2, 1,1,0,
    irr::video::SColor(255,0,0,0), 0, 1);
  Vertices[1] = irr::video::S3DVertex(
    FLOORSIZE/2,0.0,-FLOORSIZE/2, 1,0,0,
    irr::video::SColor(255,0,0,0), 1, 1);
  Vertices[2] = irr::video::S3DVertex(
    FLOORSIZE/2,0.0, FLOORSIZE/2, 0,1,1,
    irr::video::SColor(255,0,0,0), 1, 0);
  Vertices[3] = irr::video::S3DVertex(
    -FLOORSIZE/2,0.0, FLOORSIZE/2, 0,0,1,
    irr::video::SColor(255,0,0,0), 0, 0);

  Box.reset(Vertices[0].Pos);
  for (s32 i=1; i<4; ++i)
    Box.addInternalPoint(Vertices[i].Pos);
 }

 virtual void OnRegisterSceneNode()
 {
  if (IsVisible)
    SceneManager->registerNodeForRendering(this);

  ISceneNode::OnRegisterSceneNode();
 }

 virtual void render()
 {
  irr::u16 indices[] = {0,1,2,
   0,2,3,
   0,2,1,
   0,3,2};

  irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

  driver->setMaterial(Material);
  driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
  driver->drawIndexedTriangleList(&Vertices[0], 4, &indices[0], 4);
 }

  virtual const irr::core::aabbox3d<f32>& getBoundingBox() const
  {
    return Box;
  }

  virtual s32 getMaterialCount()
  {
    return 1;
  }

  virtual irr::video::SMaterial& getMaterial(s32 i)
  {
    return Material;
  }
};


bool InitPhysics(void)
{
 neGeometry *geom; // Pointer to a Geometry object
                   // which we'll use to define the shape/size
                   // of each cube
 neV3 boxSize1;    // A variable to store the length, width
                   // and height of the cube
 neV3 gravity;     // A vector to store the direction and
                   //intensity of gravity
 neV3 pos;         // The position of a cube
 f32 mass;         // The mass of our cubes
 neSimulatorSizeInfo sizeInfo; // SizeInfo stores data
                  // about how manyobjects we are going to model
 int i;


sizeInfo.rigidBodiesCount = CUBECOUNT;

sizeInfo.animatedBodiesCount = 1;



s32 totalBody = sizeInfo.rigidBodiesCount +
   sizeInfo.animatedBodiesCount;
sizeInfo.geometriesCount = totalBody;


sizeInfo.overlappedPairsCount =
     totalBody * (totalBody - 1) / 2;

sizeInfo.rigidParticleCount = 0;
sizeInfo.constraintsCount = 0;
sizeInfo.terrainNodesStartCount = 0;

gravity.Set(0.0f, -10.0f, 0.0f);



gSim = neSimulator::CreateSimulator(sizeInfo, NULL, &gravity);

srand(time(0));

for (i=0; i<CUBECOUNT; i++)
{
  // Create a rigid body
  gCubes[i] = gSim->CreateRigidBody();

  // Add geometry to the body and set it to be a box
  // of dimensions 1, 1, 1
  geom = gCubes[i]->AddGeometry();
  boxSize1.Set(CUBEX, CUBEY, CUBEZ);
  geom->SetBoxSize(boxSize1[0], boxSize1[1], boxSize1[2]);

  // Update the bounding info of the object -- must always call this
  // after changing a body's geometry.
  gCubes[i]->UpdateBoundingInfo();

  // Set other properties of the object (mass, position, etc.)
  mass =CUBEMASS;
  gCubes[i]->SetInertiaTensor(
    neBoxInertiaTensor(boxSize1[0], boxSize1[1], boxSize1[2], mass));
  gCubes[i]->SetMass(mass);

  // Vary the position so the cubes don't all exactly
  // stack on top of each other
  // (makes for a more interesting simulation)

  // MC - You may need to play with the randomization
  // stuff, since your cubemay be to big to
  // be easily destabelized
  pos.Set((rand()%10) / 20.0f * CUBEX, 4.0f + i * (CUBEY + 1),
   (rand()%10)/ 20.0f * CUBEZ);
  gCubes[i]->SetPos(pos);
}

gFloor = gSim->CreateAnimatedBody();

geom = gFloor->AddGeometry();
boxSize1.Set(FLOORSIZE, 0.0f, FLOORSIZE);
geom->SetBoxSize(boxSize1[0],boxSize1[1],boxSize1[2]);
gFloor->UpdateBoundingInfo();
 pos.Set(0.0f, 0.0f, 0.0f);
 gFloor->SetPos(pos);
 // All done
 return true;
}


void KillPhysics(void)
{
 if (gSim)
 {
   // Destroy the simulator.
   // Note that this will release all related
   // resources that we've allocated.
   neSimulator::DestroySimulator(gSim);
   gSim = NULL;
 }
}


bool InitTimer(void)
{
  int64_t TimerFrequency;
/*
  // Can we use the high performance timer?
  if (QueryPerformanceFrequency( (LARGE_INTEGER*)&TimerFrequency))
  {
    // High performance timer available, let's get ready to use it
    gfTimeScale = 1.0f / TimerFrequency;
    QueryPerformanceCounter((LARGE_INTEGER *) &gCurrentTime);
    gbUseHFTimer = true;
    return true;
  }
  else
  {
    // No high precision timer to be seen,
    // let's use the Windows multimedia
    // timer instead
    gfTimeScale = 0.001f;
    gCurrentTime = timeGetTime();
    gbUseHFTimer = false;
    return true;
  }
*/
  // No timer available at all (should never happen)
  return false;
}

float GetElapsedTime()
{
    /*
  INT64 newTime;
  float fElapsed;

  if (gbUseHFTimer)
    QueryPerformanceCounter((LARGE_INTEGER *) &newTime);
  else
    newTime=timeGetTime();

  // Scale accordingly
  fElapsed = (float)((newTime - gCurrentTime) * gfTimeScale);

  // Save the new time value for the next time we're called
  gCurrentTime = newTime;

  return fElapsed;
  */
}


int main()
{
  //irr - variable
  MyEventReceiver receiver;

  //tok - timer stuff
  float fElapsed;
  static float fLastElapsed;
  int i;

  //MC - I needed a vector3df to pass to the Node movement functions
  irr::core::vector3df TempVect;

  irr::IrrlichtDevice *device =
  irr::createDevice(irr::video::EDT_OPENGL,
  irr::core::dimension2du(640, 480), 16,
     false, false, &receiver);

  irr::video::IVideoDriver* driver = device->getVideoDriver();
  irr::scene::ISceneManager* smgr = device->getSceneManager();

  camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.50f);

  //MC - I moved the camera so the initial view is more interesting
  //     then turned it toward the origin, there the cubes fall
  camera->setPosition(irr::core::vector3df(20,30,20));
  camera->setTarget(irr::core::vector3df(-20,-30,-20));

  device->getCursorControl()->setVisible(false);


 //MC - This is a major step, in case it's not obvious.
 // Initialize Tokamak
 InitPhysics();

 //MC - Here we set up our array of cubes then create them.
 // Make sure you DO NOT drop() the cubes, since we need to
 // use the pointer to refer back to the
 // cubes to position them
 PhysicsCubeNode *CubeNode[CUBECOUNT];

 for (i=0; i<CUBECOUNT;i++)
 {
   CubeNode[i] = new PhysicsCubeNode(
      smgr->getRootSceneNode(), smgr, 666);
 }

 //MC - I'm one of those programmers that will fit 2
 // or more commands on one line if I can.  Plus, I
 // think this is a really cute way of calling and
 // dropping a node without using a variable. I use
 // the magic of C++ to create a new Node and drop
 // that same node in the same look.  If I was doing
 // this the long way I would need a PhysicsFloorNode
 // pointer
 (new PhysicsFloorNode(smgr->getRootSceneNode(),
     smgr, 666))->drop();

 //tok - Initialize the timer and the variable to
 // count the last number of milliseconds.
 //      this makes sure there isn't a wierd jump in the program.
 InitTimer();
 fLastElapsed = 0;

 //irr - This will give us something to reference the last FPS count
 int lastFPS = -1;


while(device->run())
{

  //tok - Find out how much time has elapsed
  //      since we were last called
  fElapsed = device->getTimer()->getRealTime();//GetElapsedTime();

  //tok - Prevent the elapsed time from being more
  //      than 20% greater or less than the previous
  //      elapsed time.
  if (fLastElapsed != 0)
  {
    if (fElapsed > fLastElapsed * 1.2f)
      fElapsed = fLastElapsed * 1.2f;
    if (fElapsed < fLastElapsed * 0.8f)
      fElapsed = fLastElapsed * 0.8f;
  }

  // Stop the elapsed time from exceeding 1/45th of a second.
  if (fElapsed > 1.0f / 45.0f)
     fElapsed = 1.0f / 45.0f;
  // Store the elapsed time so that we can use
  //it the next time around
  fLastElapsed = fElapsed;

  gSim->Advance(fElapsed);

  //MC - here where the real magic happens.
  //     We loop through each cube;
  for (i=0; i<CUBECOUNT; i++)
  {

    //First, we get the position from the Tokamak cube

    neV3 p = gCubes[i]->GetPos();

    //Now, we set up out temporary vector
    TempVect.X = p[0];
    TempVect.Y = p[1];
    TempVect.Z = p[2];

    //And set the position of the cube that Irrlicht is going to draw
    CubeNode[i]->setPosition(TempVect);

    //Last, we get the rotation from the Tokamak cube
    // This is a the rotation quaternion, I believe
    // More information is found here:
    // http://www.gamedev.net/reference/articles/article1095.asp
    // (Thanks to unnamed forum user)
    neQ q = gCubes[i]->GetRotationQ();

    //Again, the temporary vector is set
    TempVect.Y = q.Y;
    TempVect.X = q.X;
    TempVect.Z = q.Z;

    //we convert it to degrees and set the Irrlicht cube's rotation
    TempVect *= 180 / PI;
    CubeNode[i]->setRotation(TempVect);
  }

  //irr - Now we draw it all
  driver->beginScene(true, true, irr::video::SColor(0,100,100,100));

  smgr->drawAll();

  driver->endScene();

  int fps = driver->getFPS();

  if (lastFPS != fps)
  {
   wchar_t tmp[1024];
   swprintf(tmp, 1024, L"Physics Example- Irrlicht & Tokamak"\
   L" Engines(fps:%d)", fps);

   device->setWindowCaption(tmp);
   lastFPS = fps;
  }
}

 //tok - When we're one, we kill the physics engine
 KillPhysics();

 //MC - And drop all the node pointers that we don't need anymore
 for (i=0; i<CUBECOUNT;i++)
 CubeNode[i]->drop();

 //irr - and we finish off everything by dropping the device
 device->drop();

 return 0;
}










