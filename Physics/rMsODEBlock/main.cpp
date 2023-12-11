#include <ode/ode.h>
#include <irrlicht.h>

/* IMPLIMENT THIS
http://opende.sourceforge.net/wiki/index.php/HOWTO_fixed_vs_variable_timestep
 cache = 0
 timestep = .01

 update(time_elapsed)
 {
   cache += time_elapsed
   while(cache >= timestep)
   {
     ~normal simloop of ode using 'timestep' here ~
     cache -= timestep
   }
 }

*/



class Physics
{
   
public:
   
   ///The constructor inits ode
   Physics()
   {
      dInitODE2(0);
      dAllocateODEDataForThread(dAllocateMaskAll);

      world = dWorldCreate();
      space = dHashSpaceCreate(0);
      contactgroup = dJointGroupCreate(0);

      dWorldSetGravity(world, 0, -9.80665, 0);

      //Set and get the global ERP value, that controls how much error correction is performed in each time step.
      dWorldSetERP(world, 0.8);

      //Set and get the global CFM (constraint force mixing) value.
      dWorldSetCFM (world,1e-5);

      dWorldSetAutoDisableFlag(world, 1);
      
      dWorldSetAutoDisableAverageSamplesCount( world, 10 );

      dWorldSetLinearDamping(world, 0.00001);
      dWorldSetAngularDamping(world, 0.005);
      dWorldSetMaxAngularSpeed(world, 200);

      dWorldSetContactMaxCorrectingVel (world,0.1);
      dWorldSetContactSurfaceLayer (world,0.001);
   }

   ///The destructor shuts down ode
   ~Physics()
   {
      dJointGroupDestroy(contactgroup);
      dSpaceDestroy(space);
      dWorldDestroy(world);
      dCloseODE();
   }
   
   ///update physics
   void update(const irr::f32 stepSize = 0.01f) const
   {
      dSpaceCollide(space, 0, &nearCallback);
      dWorldStep(world, stepSize);
      dJointGroupEmpty(contactgroup); //The contact group needs to be emptied.
   }

   dWorldID& getWorld() const { return world; }

   dSpaceID& getSpace() const { return space; }
   
private:

   static dWorldID world; //the ode world.
   static dSpaceID space; //a space (used for collsions)
   static dJointGroupID contactgroup;

   //static ode callback (used for collison detection)
   static void nearCallback(void* node, dGeomID o1, dGeomID o2)
   {
      //check if they may collide
      dBodyID b1 = dGeomGetBody(o1);
      dBodyID b2 = dGeomGetBody(o2);
      if(b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact)) return;

      //generate contact points
      const unsigned int MAX_CONTACTS = 8;
      dContact contact[MAX_CONTACTS];

      const unsigned int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
      if(numc > 0)
      {
         unsigned int i = 0;
         for(i = 0; i < numc; ++i)
         {
            contact[i].surface.mode = dContactBounce | dContactSoftCFM | dContactSoftERP;
            contact[i].surface.mu = dInfinity;
            contact[i].surface.bounce = 0.01;
            contact[i].surface.bounce_vel = 0.01;
            contact[i].surface.soft_cfm = 0.01;
            contact[i].surface.soft_erp = 0.8;

            dJointID c = dJointCreateContact(world, contactgroup, &contact[i]);
            dJointAttach(c, b1, b2);
         }
      }
   }
};

dWorldID Physics::world;
dSpaceID Physics::space;
dJointGroupID Physics::contactgroup;

static const irr::core::matrix4 getTransformationMatrix(const dBodyID body)
{
   const dReal* p = dBodyGetPosition(body);
   const dReal* r = dBodyGetRotation(body);

   irr::f32 m[16];
   m[ 0] = r[ 0]; m[ 1] = r[ 4]; m[ 2] = r[ 8]; m[ 3] = 0;
   m[ 4] = r[ 1]; m[ 5] = r[ 5]; m[ 6] = r[ 9]; m[ 7] = 0;
   m[ 8] = r[ 2]; m[ 9] = r[ 6]; m[10] = r[10]; m[11] = 0;
   m[12] = p[ 0]; m[13] = p[ 1]; m[14] = p[ 2]; m[15] = 1;

   irr::core::matrix4 mat;
   mat.setM(m);

   return mat;
}

static void updateIrrlichtNode(const dBodyID body)
{
   irr::scene::ISceneNode* node = (irr::scene::ISceneNode*)dBodyGetData(body);
      if (!node) return;

      const irr::core::matrix4& mat = getTransformationMatrix(body);
      node->setRotation(mat.getRotationDegrees());
      node->setPosition(mat.getTranslation());
}


void addCube(irr::scene::ISceneManager* smgr, const Physics& physics, const irr::f32 size, const irr::core::vector3df& pos)
{
   irr::scene::ISceneNode* cube = smgr->addCubeSceneNode(size);
   cube->setPosition(pos);
   cube->updateAbsolutePosition();

//   const irr::core::vector3df& pos = cube1->getPosition();
   const irr::core::vector3df& rotationRAD = (cube->getRotation()*irr::core::DEGTORAD);
   const irr::core::quaternion irrQuat(rotationRAD);
   dQuaternion odeQuat;
   odeQuat[0] = irrQuat.W;
   odeQuat[1] = irrQuat.X;
   odeQuat[2] = irrQuat.Y;
   odeQuat[3] = irrQuat.Z;


   //create body
   dBodyID body = dBodyCreate(physics.getWorld());
   dBodySetData(body, cube);

    const irr::core::vector3df& extend = cube->getBoundingBox().getExtent();

    dMass dmass;
    dMassSetZero(&dmass);
    dMassSetBoxTotal(&dmass, 1.0, (dReal)extend.X, (dReal)extend.Y, (dReal)extend.Z);
    dBodySetMass(body, &dmass);

   dBodySetPosition(body, pos.X, pos.Y, pos.Z);
   dBodySetQuaternion(body, odeQuat);

   dBodySetGravityMode(body, true);
   dBodySetMovedCallback(body, &updateIrrlichtNode);

   //create geom
   dGeomID geom = dCreateBox(physics.getSpace(), (dReal)extend.X, (dReal)extend.Y, (dReal)extend.Z);
   dGeomSetData(geom, cube);
   dGeomSetBody(geom, body);
   dGeomSetPosition(geom, pos.X, pos.Y, pos.Z);
   dGeomSetQuaternion(geom, odeQuat);
}


int main()
{

   irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(1024, 768));
   if (device == 0) return 1;

   device->setWindowCaption(L"ODE Demo");

   irr::video::IVideoDriver* driver = device->getVideoDriver();
   irr::scene::ISceneManager* smgr = device->getSceneManager();
   
   irr::scene::ICameraSceneNode* camera = smgr->addCameraSceneNode();
   camera->setPosition(irr::core::vector3df(-60.0f, 5.0f, 80.0f));


   irr::scene::ILightSceneNode* node = smgr->addLightSceneNode(0, irr::core::vector3df(0,0,0),
      irr::video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);
   irr::scene::ISceneNodeAnimator* anim = 0;
   anim = smgr->createFlyCircleAnimator (irr::core::vector3df(0,150,0),250.0f);
   node->addAnimator(anim);
   anim->drop();


   Physics physics;

   //for the cubes to rest on
   dCreatePlane(physics.getSpace(), 0, 1, 0, 1);

   //add some cubes
   addCube(smgr, physics, 10.0f, irr::core::vector3df(0.0f, 20.0f, 80.0f));
   addCube(smgr, physics, 5.0f, irr::core::vector3df(0.0f, 50.0f, 75.0f));
   addCube(smgr, physics, 10.0f, irr::core::vector3df(0.0f, 80.0f, 80.0f));
   addCube(smgr, physics, 8.0f, irr::core::vector3df(0.0f, 100.0f, 80.0f));

   while (device->run())
   {
      if (device->isWindowActive())
      {
         physics.update();

         //update rendering
         driver->beginScene(true, true, irr::video::SColor(255, 128, 128, 255));
         smgr->drawAll();
         driver->endScene();
      }
   }

   device->drop();

   return 0;
} 
