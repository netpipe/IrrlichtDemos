#include <irrlicht.h>
#include <irrbullet.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrBullet.lib")
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "GIMPACTUtils.lib")
#pragma comment(lib, "LinearMath.lib")
#pragma comment(lib, "BulletSoftBody.lib")

bool IsKeyDown = false;

static IRaycastVehicle* vehicle=0;
static IRigidBody *body;
static IAnimatedMeshSceneNode * carTirefr;
static IAnimatedMeshSceneNode * carTirefl;
static IAnimatedMeshSceneNode * carTirebl;
static IAnimatedMeshSceneNode * carTirebr;

static f32 steeringval=0.0;
void createGround();
void createVehicle(const stringw& file, const stringw& collFile,const vector3df &pos, const f32 mass);
void updateCar();
IrrlichtDevice *device=0;
irrBulletWorld *world=0;
IVideoDriver* driver = 0;
ISceneManager* smgr = 0;
IGUIEnvironment* guienv = 0;
u32 jc;
s32 eForce = 0;
class MyEventReceiver:public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{
    switch(event.EventType)
    {
		 
        case EET_KEY_INPUT_EVENT:
        {
   //         KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
            if(event.KeyInput.Key == KEY_KEY_P && event.KeyInput.PressedDown == false)
            {
                world->pauseSimulation(!world->simulationPaused());
            return true;
			}
           
			  	  if(event.KeyInput.Key == KEY_KEY_W && event.KeyInput.PressedDown == true)
            {
				eForce=50000;
//vehicle->applyEngineForce(eForce,1);
vehicle->applyEngineForce(eForce,0);
//vehicle->applyEngineForce(eForce,3);
vehicle->applyEngineForce(eForce,2);
				//body->setLinearVelocity(vector3df(1000.0f,0,0),ERBTS_LOCAL);
//body->applyForce(vector3df(0,1600,1200), vector3df(0,0,0), ERBTS_WORLD);
				return true;
            }
				    if(event.KeyInput.Key == KEY_KEY_S && event.KeyInput.PressedDown == true)
            {
				eForce=50000;
//vehicle->applyEngineForce(-eForce,1);
vehicle->applyEngineForce(-eForce,0);
//vehicle->applyEngineForce(-eForce,3);
vehicle->applyEngineForce(-eForce,2);
				return true;
            }

				   if(event.KeyInput.Key == KEY_KEY_A && event.KeyInput.PressedDown == true)
            {
steeringval = 45.0;
	vehicle->setSteeringValue(steeringval,0);
vehicle->setSteeringValue(steeringval,2);
				return true;
            }
				    if(event.KeyInput.Key == KEY_KEY_D && event.KeyInput.PressedDown == true)
            {
steeringval = 45.0;
	vehicle->setSteeringValue(-steeringval,0);
vehicle->setSteeringValue(-steeringval,2);

				return true;
            }
					
		}
	}
return false;
	}
};

int main()
{
MyEventReceiver rec;
device =createDevice( video::EDT_DIRECT3D9, dimension2d<u32>(800, 600), 32,false, true, true, &rec);
world = createIrrBulletWorld(device, true, true);
device->getSceneManager()->addLightSceneNode(0, vector3df(20, 40, -50), SColorf(1.0f, 1.0f, 1.0f), 4000.0f);
 world->setDebugMode(EPDM_DrawAabb | EPDM_DrawContactPoints);
 world->setGravity(vector3df(0,-50.0f,0));
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	createGround();

//	createVehicle(L"C:\\Users\\Mubashar Islam\\Documents\\Visual Studio 2008\\Projects\\test45\\Debug\\JagdPanther.b3d", L"C:\\Users\\Mubashar Islam\\Documents\\Visual Studio 2008\\Projects\\test45\\Debug\\JagdPantherCollision.b3d", vector3df(10.504709, 1.8, -3.333332), 200);
	ICameraSceneNode * cam=device->getSceneManager()->addCameraSceneNodeFPS();
//cam->setTarget(vehicle->getRigidBody()->getWorldTransform().getTranslation());
//cam->setTarget(vehicle->getRigidBody()->getWorldTransform().getTranslation());
cam->setFOV(32);
cam->setFarValue(20000);
    u32 TimeStamp = device->getTimer()->getTime();
    u32 DeltaTime = 0;
	 world->debugDrawProperties(true);
	       IGUIStaticText *scarproperties = device->getGUIEnvironment()->addStaticText(L"",
            rect<s32>(10,10,120,240), false);
   while(device->run())
   {


	//steeringval =0.0;
eForce=0;
		driver->beginScene(true, true, SColor(255,100,101,140));
DeltaTime = device->getTimer()->getTime() - TimeStamp;
		TimeStamp = device->getTimer()->getTime();

		//for (int i=0;i<4;i++)
		//{
		//	vehicle->updateWheelTransform(i,);
		//	
		//	vehicle->getWheelInfo(i).worldTransform.getTranslation();


		//	}*

		for(int i=0;i<1;i++)
		{
			carTirefl->setRotation(vehicle->getWheelInfo(i).worldTransform.getRotationDegrees());
			carTirebl->setRotation(vector3df(vehicle->getWheelInfo(i+1).worldTransform.getRotationDegrees().X,0.0f,0.0f));
			carTirefr->setRotation(vehicle->getWheelInfo(i+2).worldTransform.getRotationDegrees());
			carTirebr->setRotation(vector3df(vehicle->getWheelInfo(i+3).worldTransform.getRotationDegrees().X,0.0f,0.0f));
			/*carTirefl->setPosition(vehicle->getWheelInfo(i).worldTransform.getRotationDegrees());
					carTirebl->setPosition(vehicle->getWheelInfo(i+1).worldTransform.getRotationDegrees());
			carTirefr->setPosition(vehicle->getWheelInfo(i+2).worldTransform.getRotationDegrees());
			carTirebr->setPosition(vehicle->getWheelInfo(i+3).worldTransform.getRotationDegrees());*/
		/*	carTirefl->updateAbsolutePosition();
carTirebl->updateAbsolutePosition();
carTirefr->updateAbsolutePosition();
carTirebr->updateAbsolutePosition();*/
			//		carTirefl->setPosition(vehicle->getWheelInfo(i).worldTransform.getTranslation());
			//carTirebl->setPosition(vehicle->getWheelInfo(i+1).worldTransform.getTranslation());
			//carTirefr->setPosition(vehicle->getWheelInfo(i+2).worldTransform.getTranslation());
			//carTirebr->setPosition(vehicle->getWheelInfo(i+3).worldTransform.getTranslation());

			//		carTirefl->setScale(vehicle->getWheelInfo(i).worldTransform.getTranslation());
			//carTirebl->setScale(vehicle->getWheelInfo(i+1).worldTransform.getTranslation());
			//carTirefr->setScale(vehicle->getWheelInfo(i+2).worldTransform.getTranslation());
			//carTirebr->setScale(vehicle->getWheelInfo(i+3).worldTransform.getTranslation());
	/*		carTirefl->settr
			carTirebl->getRelativeTransformation(vehicle->getWheelInfo(i+1).worldTransform.getTranslation());
			carTirefr->getRelativeTransformation(vehicle->getWheelInfo(i+2).worldTransform.getTranslation());
			carTirebr->getRelativeTransformation(vehicle->getWheelInfo(i+2).worldTransform.getTranslation());*/
		}

 world->stepSimulation(DeltaTime*0.001f, 7);
//cam->setTarget(vehicle->getRigidBody()->getWorldTransform().getTranslation());
//cam->setPosition(vehicle->getRigidBody()->getWorldTransform().getTranslation());
     //   updateTank();
        world->debugDrawWorld(true);
		world->debugDrawProperties(true);
        // This call will draw the technical properties of the physics simulation
        // to the GUI environment.
       scarproperties->setOverrideColor(SColor(255,255,255,255));
	  stringw string = "scar Joint Count: ";
	  string +=jc;
        scarproperties->setText(string.c_str()); 
	  smgr->drawAll();
guienv->drawAll();
		driver->endScene();
	}

	device->drop();

	return 0;
}

void createGround()
{
	
//IMeshSceneNode * nplane = smgr->addMeshSceneNode(smgr->getMesh("media\\plane\\map.obj"));
//nplane->setMaterialFlag(EMF_LIGHTING,false);
//nplane->setMaterialTexture(0,driver->getTexture("media\\plane\\grass.JPG"));
//ICollisionShape * splane = new IBvhTriangleMeshShape(nplane,smgr->getMesh("media\\plane\\map.obj"),0.0f);
//IRigidBody* rplane = world->addRigidBody(splane);
 //irr::scene::ISceneNode *Node = smgr->addTerrainMesh(smgr->getMesh("media\\plane\\map.obj"),driver->getTexture("media\\plane\\grass.JPG"),);
	 IMeshSceneNode *Node = device->getSceneManager()->addCubeSceneNode(1.0);
		// smgr->addMeshSceneNode(smgr->getMesh("media\\plane\\map.obj"));
	//device->getSceneManager()->addCubeSceneNode(1.0);
	Node->setScale(irr::core::vector3df(10000,1,10000)); // 400, 3, 400
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, false);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture("media\\plane\\grass.JPG"));


      //  Node->setMaterialFlag(irr::video::EMF_WIREFRAME, true);

	ICollisionShape *shape = new IBoxShape(Node,0.0,false);

	//shape->setMargin(0.01);
	
	body = world->addRigidBody(shape);
IAnimatedMeshSceneNode* carNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media\\car\\f360.b3d"),0,1);
carNode->setMaterialFlag(EMF_LIGHTING,false);
	carNode->setMaterialTexture(0, device->getVideoDriver()->getTexture("media\\car\\fskin.JPG"));
//carNode->setPosition(vector3df(carNode->getPosition().X,carNode->getPosition().Y+10,carNode->getPosition().Z));
//carNode->setMaterialTexture(0,driver->getTexture("media\\car\\fskin.JPG"));s
//carNode->setMaterialFlag(irr::video::EMF_WIREFRAME, true);
//carNode->setScale(vector3df(5.0f,5.0f,5.0f));


IGImpactMeshShape * scar = new IGImpactMeshShape(carNode, carNode->getMesh(), 2000.0); // Tweak with mass a bit
IRigidBody* rcar = world->addRigidBody(scar);
rcar->includeNodeOnRemoval(false);

carTirefr = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media\\car\\tire.3ds"),carNode,2,vector3df(carNode->getJointNode("front_right_joint")->getPosition().X,carNode->getJointNode("front_right_joint")->getPosition().Y,carNode->getJointNode("front_right_joint")->getPosition().Z));
carTirefr->setMaterialFlag(EMF_LIGHTING,false);
carTirefr->setMaterialFlag(irr::video::EMF_WIREFRAME, true);

carTirefl = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media\\car\\tire.3ds"),carNode,2,vector3df(carNode->getJointNode("front_left_joint")->getPosition().X,carNode->getJointNode("front_left_joint")->getPosition().Y,carNode->getJointNode("front_left_joint")->getPosition().Z));
carTirefl->setMaterialFlag(EMF_LIGHTING,false);
carTirefl->setMaterialFlag(irr::video::EMF_WIREFRAME, true);

carTirebr = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media\\car\\tire.3ds"),carNode,2,vector3df(carNode->getJointNode("back_right_joint")->getPosition().X,carNode->getJointNode("back_right_joint")->getPosition().Y,carNode->getJointNode("back_right_joint")->getPosition().Z));
carTirebr->setMaterialFlag(EMF_LIGHTING,false);
carTirebr->setMaterialFlag(irr::video::EMF_WIREFRAME, true);

carTirebl = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media\\car\\tire.3ds"),carNode,2,vector3df(carNode->getJointNode("back_left_joint")->getPosition().X,carNode->getJointNode("back_left_joint")->getPosition().Y,carNode->getJointNode("back_left_joint")->getPosition().Z));
carTirebl->setMaterialFlag(EMF_LIGHTING,false);
carTirebl->setMaterialFlag(irr::video::EMF_WIREFRAME, true);


// 
//        // When using a raycast vehicle, we don't want this rigid body to deactivate.
rcar->setActivationState(EAS_DISABLE_DEACTIVATION);

// rcar->setDamping(0.45, 0.45);
body->setFriction(0.100f);
vehicle = world->addRaycastVehicle(rcar);
SWheelInfoConstructionInfo wheel;
wheel.wheelDirectionCS = irr::core::vector3df(0.0, -1.0, 0.0);
wheel.wheelAxleCS = irr::core::vector3df(-5.0, 0.0, 0.0);
wheel.wheelRadius = 5.0f;
//wheel.suspensionRestLength = 0.7f;
//jc = carNode->getJointCount();
wheel.chassisConnectionPointCS =  carNode->getJointNode("front_left_joint")->getPosition();
        wheel.isFrontWheel = true;
		wheel.suspensionRestLength = carNode->getJointNode("front_left_joint")->getPosition().getDistanceFrom(carNode->getJointNode("front_left_upper_joint")->getPosition());
		vehicle->addWheel(wheel); // Add left front wheel
 
        wheel.isFrontWheel = false;
        wheel.chassisConnectionPointCS =  carNode->getJointNode("back_left_joint")->getPosition();
		wheel.suspensionRestLength = carNode->getJointNode("back_left_joint")->getPosition().getDistanceFrom(carNode->getJointNode("back_left_upper_joint")->getPosition());
		vehicle->addWheel(wheel); // Add left back wheel
 
        // Right wheels
      // wheel.wheelAxleCS.X = -wheel.wheelAxleCS.X; // Invert the wheel direction for right side
        wheel.chassisConnectionPointCS = carNode->getJointNode("front_right_joint")->getPosition();
			//carTirefr->getJointNode("front_right_wheel_joint")->getPosition();
			//carNode->getJointNode("front_right_joint")->getPosition();
		wheel.suspensionRestLength = carNode->getJointNode("front_right_joint")->getPosition().getDistanceFrom(carNode->getJointNode("front_right_upper_joint")->getPosition());
        wheel.isFrontWheel = true;
        vehicle->addWheel(wheel); // Add left front wheel
 
        wheel.isFrontWheel = false;
		// irr::f32 wheelAxleCSX = -wheel.wheelAxleCS.X;
        wheel.chassisConnectionPointCS = carNode->getJointNode("back_right_joint")->getPosition();
			//carTirebr->getJointNode("front_right_wheel_joint")->getPosition();
			//carNode->getJointNode("back_right_joint")->getPosition();
		wheel.suspensionRestLength = carNode->getJointNode("back_right_joint")->getPosition().getDistanceFrom(carNode->getJointNode("back_right_upper_joint")->getPosition());
        vehicle->addWheel(wheel); // Add left back wheel
 
     
 
        for (irr::u32 i = 0; i < vehicle->getNumWheels(); i++) {
                SWheelInfo& info = vehicle->getWheelInfo(i);
                info.suspensionStiffness = 20.0f;
                info.wheelDampingRelaxation = 2.3f;
                info.wheelDampingCompression = 4.4f;
				//	btSqrt(0.2 * 2.0 * 200.0);
                info.frictionSlip = 2; 
                info.rollInfluence = 0.4f;
                vehicle->updateWheelInfo(i);
		}

        rcar->setVehicleReference(vehicle);
}