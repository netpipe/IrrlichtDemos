#include <irrlicht.h>
#include <ode/ode.h>
#include <vector>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif
static dWorldID world;
static dSpaceID space;
static dJointGroupID contactgroup;
dBodyID body;
dBodyID body2;		// the body
dGeomID geom;
dGeomID geom2;
dGeomID geom3;
irr::core::vector3df rot;
int fps = 0;
std::vector<dGeomID> boxes;
std::vector<ISceneNode*>nodes;
std::vector<dBodyID>bodies;

//functions


//This is the near collision callback. When two geoms are close to each other,
//ODE calls this function.  Special code can be added by getting the user data(scene node)
// from the geom with ISceneNode *node =  (ISceneNode*)dGeomGetData(geom);
//you can change the surface properties for general collisions.
void nearCallback(void* node, dGeomID o1, dGeomID o2)
{
  int i=0;
  dBodyID b1=dGeomGetBody(o1);
  dBodyID b2=dGeomGetBody(o2);
    int MAX_CONTACTS = 8;
  if(b1 && b2 && dAreConnectedExcluding(b1,b2,dJointTypeContact))return;
  dContact contact[MAX_CONTACTS];
    for(i=0;i<MAX_CONTACTS;++i)
  {
    contact[i].surface.mode=dContactBounce | dContactSoftCFM;
    contact[i].surface.mu=10000000000000000000000.0;
    contact[i].surface.mu2=.00001;
    contact[i].surface.bounce=.2;
    contact[i].surface.bounce_vel=.1;
    contact[i].surface.soft_cfm=.0000001;
  }
    int numc=dCollide(o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact));
  if(numc>0)
  {
    for(i=0;i<numc;i++)
    {
      dJointID c=dJointCreateContact(world,contactgroup,&contact[i]);
      dJointAttach(c,b1,b2);
    }
  }
}
//This function gets the size of the node's bounding box and assigns a box geom of that
//size to the space.  It positions it according to the node position. It gives
//it a mass of 2.  It assigns the node to the body data and the geom
//data so they can be called up during collision.
void setBody(dWorldID &world,dSpaceID &space,ISceneNode* node,dBodyID &body,dGeomID &geom)
{
    vector3df extend = (node->getBoundingBox()).getExtent();
    vector3df pos=node->getPosition();
    geom=dCreateBox(space,(dReal)extend.X,(dReal)extend.Y,(dReal)extend.Z);
    dGeomSetPosition(geom,pos.X,pos.Y,pos.Z);
    dGeomSetData(geom,node);
    body=dBodyCreate(world);
    dMass mass;
    dMassSetBox(&mass,1.0,(dReal)extend.X,(dReal)extend.Y,(dReal)extend.Z);
    dBodySetMass(body,&mass);
    dGeomSetBody(geom,body);
    dBodySetPosition(body,pos.X,pos.Y,pos.Z);
    dBodySetData(body,node);
}
//This function gets the scene node from the geom and positions it for render.
void setPosition(dGeomID geom)
{
    dReal * ode_pos =(dReal*)dGeomGetPosition(geom);
    ISceneNode *node =  (ISceneNode*)dGeomGetData(geom);
    vector3df pos((irr::f32)ode_pos[0],(irr::f32)ode_pos[1],(irr::f32)ode_pos[2]);
    node->setPosition(pos);
    //ODE uses quaternions, so we get it and convert to euler
    dQuaternion result;
    dGeomGetQuaternion(geom, result);
    irr::core::quaternion quat(result[1],result[2],result[3],result[0]);
    quat.toEuler(rot);
    vector3df degrees(rot.X*180/PI,rot.Y*180/PI,rot.Z*180/PI);
    node->setRotation(degrees);

}



int main()
{


//  dInitODE();
  world = dWorldCreate();
  space = dHashSpaceCreate (0);
  contactgroup = dJointGroupCreate (0);
  dWorldSetGravity (world,0,-10.0,0);


  int lastFPS = 180;


	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2d<u32>(800, 600), 32,
			false, false, false, 0);


	device->setWindowCaption(L"Hello World! - ODE Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Primitive Collision on a Plane",
		rect<s32>(10,10,260,22), true);


	IAnimatedMesh* mesh = smgr->getMesh("box.x");
	IAnimatedMeshSceneNode* node;
	f32 hgt = 8;
	f32 step = -25;
    for(int i = 0;i<100;++i)
    {
    hgt+=3.0;
    step+=.07;
    node = smgr->addAnimatedMeshSceneNode( mesh );
	node->setPosition(vector3df(step,hgt,-10.0));
	node->setMaterialFlag(EMF_LIGHTING, false);
	nodes.push_back(node);
    boxes.push_back(geom);
    bodies.push_back(body);
    setBody(world,space,nodes[i],bodies[i],boxes[i]);
    }






//We create a plane geom at level zero so the boxes won't drop off the screen.

    geom3=dCreatePlane(space,0,1,0,0);


	smgr->addCameraSceneNode(0, vector3df(0,10,-40), vector3df(0,5,0));

	while(device->run())
	{
        fps = driver->getFPS();

		if (lastFPS != fps)
		{
		  core::stringw str;
		  str += driver->getName();
		  str += " FPS:";
		  str += fps;

		  device->setWindowCaption(str.c_str());
		  lastFPS = fps;
		}

       for(int i = 0; i<boxes.size();++i)
       {
          setPosition(boxes[i]);
       }
        dJointGroupEmpty (contactgroup);
		driver->beginScene(true, true, SColor(255,100,101,140));
		smgr->drawAll();
		guienv->drawAll();
        dSpaceCollide (space,0,&nearCallback);
		dWorldQuickStep(world,0.025);
		//The contact group needs to be emptied.


		driver->endScene();
		device->sleep(10);
	}

	void dWorldDestroy (dWorldID);
	device->drop();


	return 0;
}

