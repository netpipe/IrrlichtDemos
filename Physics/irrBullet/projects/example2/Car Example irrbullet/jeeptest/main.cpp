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

const int RX=800;
const int RY=600;

static IAnimatedMeshSceneNode* carNode1;


vector3df Irr2BT(const vector3df& in);
f32 distance;
void UpdateVehicles();
vector3df localInertia(0,0,0);
static IAnimatedMeshSceneNode* carNode;
static IRaycastVehicle* vehicle=0;
static IRigidBody *body;
static ICollisionShape * scar;
static IAnimatedMeshSceneNode * carTirefr;
static IAnimatedMeshSceneNode * carTirefl;
static IAnimatedMeshSceneNode * carTirebl;
static IAnimatedMeshSceneNode * carTirebr;
static IRigidBody* rsphere;
static f32 steeringval=0.0;
static f32 steeringval1=0.0;
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
			IsKeyDown = true;
           if(event.KeyInput.Key == KEY_KEY_P && event.KeyInput.PressedDown == false)
            {
                world->pauseSimulation(!world->simulationPaused());
            return true;
			}

			  	  if(event.KeyInput.Key == KEY_KEY_W && event.KeyInput.PressedDown == true)
            {
				if (steeringval!=0)
				{
				if(steeringval>0.0)
{
	steeringval -=15.0;
}
else if(steeringval<0.0)
{
steeringval +=15.0;
}
				}
				vehicle->setSteeringValue(-steeringval,0);
vehicle->setSteeringValue(-steeringval,2);
if(eForce>-500)
{
eForce-=25;
vehicle->applyEngineForce(eForce,1);
vehicle->applyEngineForce(eForce,0);
vehicle->applyEngineForce(eForce,3);
vehicle->applyEngineForce(eForce,2);
}
				return true;
            }
				    if(event.KeyInput.Key == KEY_KEY_S && event.KeyInput.PressedDown == true)
            {
				if(steeringval!=0)
				{
				if(steeringval>0.0)
{
	steeringval -=15.0;
}
else if(steeringval<0.0)
{
steeringval +=15.0;
}

				}
				vehicle->setSteeringValue(-steeringval,0);
vehicle->setSteeringValue(-steeringval,2);
if(eForce<500)
{
eForce+=25.0f;
vehicle->applyEngineForce(eForce,1);
vehicle->applyEngineForce(eForce,0);
vehicle->applyEngineForce(eForce,3);
vehicle->applyEngineForce(eForce,2);
}
return true;
            }

	   if(event.KeyInput.Key == KEY_SPACE && event.KeyInput.PressedDown == true)
            {
if(eForce>0)
	{
	eForce-=50;
	vehicle->applyEngineForce(eForce,1);
vehicle->applyEngineForce(eForce,0);
vehicle->applyEngineForce(eForce,3);
vehicle->applyEngineForce(eForce,2);
	}
	else if(eForce<0)
	{
eForce+=50;
vehicle->applyEngineForce(eForce,1);
vehicle->applyEngineForce(eForce,0);
vehicle->applyEngineForce(eForce,3);
vehicle->applyEngineForce(eForce,2);
	}
				return true;
            }
				   if(event.KeyInput.Key == KEY_KEY_A && event.KeyInput.PressedDown == true)
            {
				if (steeringval!=-45.0f)
				{
steeringval -= 15.0f;
if(steeringval!=-45.0f)
{
vehicle->setSteeringValue(-steeringval,0);
vehicle->setSteeringValue(-steeringval,2);
}
				else
{
vehicle->setSteeringValue(steeringval,0);
vehicle->setSteeringValue(steeringval,2);
}
				}
				vehicle->applyEngineForce(eForce,1);
vehicle->applyEngineForce(eForce,0);
vehicle->applyEngineForce(eForce,3);
vehicle->applyEngineForce(eForce,2);

				return true;
            }
				    if(event.KeyInput.Key == KEY_KEY_D && event.KeyInput.PressedDown == true)
            {
					if (steeringval!=45.0f)
				{
steeringval += 15.0f;
if(steeringval!=45.0f)
{
vehicle->setSteeringValue(-steeringval,0);
vehicle->setSteeringValue(-steeringval,2);
}

else
{
vehicle->setSteeringValue(steeringval,0);
vehicle->setSteeringValue(steeringval,2);
}

					}
vehicle->applyEngineForce(eForce,1);
vehicle->applyEngineForce(eForce,0);
vehicle->applyEngineForce(eForce,3);
vehicle->applyEngineForce(eForce,2);
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
device =createDevice( video::EDT_OPENGL, dimension2d<u32>(RX, RY), 32,false, true, true, &rec);
world = createIrrBulletWorld(device, true, true);
device->getSceneManager()->addLightSceneNode(0, vector3df(0, 25000, 0), SColorf(1.0f, 1.0f, 1.0f), 250000.0f);
 world->setDebugMode(EPDM_DrawAabb | EPDM_DrawContactPoints);
 world->setGravity(vector3df(0,-9.8f,0));
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	createGround();


	ICameraSceneNode * cam=device->getSceneManager()->addCameraSceneNode(0,vector3df(0,50,200));
	//ICameraSceneNode * cam=smgr->addCameraSceneNodeFPS();
	//ICameraSceneNode * cam1=device->getSceneManager()->addCameraSceneNode(0,vector3df(0,50,200));
//ICameraSceneNode * topcam=device->getSceneManager()->addCameraSceneNode(0,vector3df(0,5000,0));
cam->setTarget(vehicle->getRigidBody()->getWorldTransform().getTranslation());
//topcam->setTarget(vehicle->getRigidBody()->getWorldTransform().getTranslation());
//topcam->setPosition(vector3df(vehicle->getRigidBody()->getWorldTransform().getTranslation().X,vehicle->getRigidBody()->getWorldTransform().getTranslation().Y+2000.0f,vehicle->getRigidBody()->getWorldTransform().getTranslation().Z));


cam->setFOV(32);
cam->setFarValue(20000);
//cam1->setFOV(32);
//cam1->setFarValue(20000);
//topcam->setFOV(32);
//topcam->setFarValue(20000);
    u32 TimeStamp = device->getTimer()->getTime();
    u32 DeltaTime = 0;

		   IGUIStaticText *txtdistance = device->getGUIEnvironment()->addStaticText(L"",rect<s32>(0,0,RX/2,RY/2), false);

   while(device->run())
   {

	//   driver->setViewPort(rect<s32>(0,0,RX,RY));
		driver->beginScene(true, true, SColor(255,100,101,140));
		smgr->setActiveCamera(cam);
cam->setTarget(vehicle->getRigidBody()->getWorldTransform().getTranslation());
cam->setPosition(vector3df(vehicle->getRigidBody()->getWorldTransform().getTranslation().X,vehicle->getRigidBody()->getWorldTransform().getTranslation().Y+50.0f,vehicle->getRigidBody()->getWorldTransform().getTranslation().Z+200));
//cam->setRotation(vehicle->getRigidBody()->internalTransform.getRotation());
cam->updateAbsolutePosition();
//	topcam->setTarget(vehicle->getRigidBody()->getWorldTransform().getTranslation());
//topcam->setPosition(vector3df(vehicle->getRigidBody()->getWorldTransform().getTranslation().X,vehicle->getRigidBody()->getWorldTransform().getTranslation().Y+2000.0f,vehicle->getRigidBody()->getWorldTransform().getTranslation().Z));
//topcam->updateAbsolutePosition();
DeltaTime = device->getTimer()->getTime() - TimeStamp;
		TimeStamp = device->getTimer()->getTime();

UpdateVehicles();


//topcam->setTarget(rsphere->getWorldTransform().getTranslation());
//topcam->setPosition(vector3df(rsphere->getWorldTransform().getTranslation().X,rsphere->getWorldTransform().getTranslation().Y+5000.0f,rsphere->getWorldTransform().getTranslation().Z));


 world->stepSimulation(DeltaTime*0.001f, 10);
     world->debugDrawWorld(true);
	//	world->debugDrawProperties(true);
	  stringw string = "Speed: ";
	  string +=vehicle->getCurrentSpeedKmHour();
	  txtdistance->setText(string.c_str());
		smgr->drawAll();
guienv->drawAll();
		driver->endScene();
	}

	device->drop();

	return 0;
}

void createGround()
{

IMeshSceneNode * nplane = smgr->addMeshSceneNode(smgr->getMesh("media\\plane\\scaled track.obj"),0,-1,vector3df(0,-100,0));
nplane->setMaterialFlag(EMF_LIGHTING,false);
//IMeshSceneNode * nplane1 = smgr->addMeshSceneNode(smgr->getMesh("media\\plane\\Road-2-Lane-Straight.obj"));
//IMeshSceneNode * nplane1 = smgr->addMeshSceneNode(smgr->getMesh("media\\plane\\plane.obj"));

nplane->setMaterialTexture(0,driver->getTexture("media\\plane\\grass.jpg"));
//nplane1->setMaterialTexture(0,driver->getTexture("media\\plane\\grass.jpg"));
nplane->getMaterial(0).AntiAliasing = EAAM_LINE_SMOOTH | EAAM_QUALITY;
nplane->getMaterial(0).Shininess = 150.0;
nplane->getMaterial(0).SpecularColor = SColor(255,255,255,255);
nplane->setPosition(vector3df(0,0,0));
ICollisionShape * splane = new IGImpactMeshShape(nplane,nplane->getMesh(),0.0f);
//ICollisionShape * sroadst = new IGImpactMeshShape(nplane1,nplane1->getMesh(),8000.0f);

IRigidBody* rplane = world->addRigidBody(splane);
//world->addRigidBody(sroadst);
IMeshSceneNode * nsphere = smgr->addSphereSceneNode(15.0,200.0);
nsphere->setMaterialFlag(EMF_LIGHTING,false);
nsphere->setPosition(vector3df(nsphere->getPosition().X+250.0f,nsphere->getPosition().Y+150,nsphere->getPosition().Z+500.0f));
nsphere->setMaterialTexture(0,driver->getTexture("media\\plane\\conc_base1_grooved.jpg"));
ICollisionShape * nssphere = new ISphereShape(nsphere,0.1f,false);
rsphere = world->addRigidBody(nssphere);


carNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media\\car\\f360.b3d"),0,1);
carNode->setMaterialFlag(EMF_LIGHTING,false);
carNode->setPosition(vector3df(nplane->getPosition().X,nplane->getPosition().Y-3800,nplane->getPosition().Z));
//carNode->setPosition(vector3df(nplane1->getPosition().X-200,nplane->getPosition().Y+200,nplane->getPosition().Z-500));
carNode->addShadowVolumeSceneNode();
	carNode->setMaterialTexture(0, device->getVideoDriver()->getTexture("media\\car\\fskin.JPG"));
carNode->getMaterial(0).AmbientColor = SColor(255,255,255,255);
carNode->getMaterial(0).Shininess = 150.0;
carNode->getMaterial(0).SpecularColor = SColor(255,255,255,255);

carNode->getMaterial(0).AntiAliasing = EAAM_LINE_SMOOTH | EAAM_QUALITY;

scar = new IGImpactMeshShape(carNode,smgr->getMesh("media\\car\\f360.b3d"),1.0); // Tweak with mass a bit
IRigidBody* rcar = world->addRigidBody(scar);
rcar->includeNodeOnRemoval(false);

carTirefr = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media\\car\\tright.3ds"),carNode,2,carNode->getJointNode("front_right_joint")->getPosition());
carTirefr->setMaterialFlag(EMF_LIGHTING,false);
	carTirefr->setMaterialTexture(0, device->getVideoDriver()->getTexture("media\\car\\fskin.JPG"));
	carTirefr->addShadowVolumeSceneNode();

carTirefl = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media\\car\\tleft.3ds"),carNode,2,carNode->getJointNode("front_left_joint")->getPosition());
carTirefl->setMaterialFlag(EMF_LIGHTING,false);
	carTirefl->setMaterialTexture(0, device->getVideoDriver()->getTexture("media\\car\\fskin.JPG"));
		carTirefl->addShadowVolumeSceneNode();

carTirebr = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media\\car\\tright.3ds"),carNode,2,carNode->getJointNode("back_right_joint")->getPosition());
carTirebr->setMaterialFlag(EMF_LIGHTING,false);
	carTirebr->setMaterialTexture(0, device->getVideoDriver()->getTexture("media\\car\\fskin.JPG"));
		carTirebr->addShadowVolumeSceneNode();

carTirebl = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media\\car\\tleft.3ds"),carNode,2,carNode->getJointNode("back_left_joint")->getPosition());
carTirebl->setMaterialFlag(EMF_LIGHTING,false);
	carTirebl->setMaterialTexture(0, device->getVideoDriver()->getTexture("media\\car\\fskin.JPG"));
		carTirebl->addShadowVolumeSceneNode();

		rcar->setActivationState(EAS_DISABLE_DEACTIVATION);

rcar->setDamping(0.6f, 0.6f);
rcar->setFriction(0.01f);
//rcar->setGravity(vector3df(0,-9.8,0));
//rplane->setFriction(1.0f);
vehicle = world->addRaycastVehicle(rcar);
SWheelInfoConstructionInfo wheel;
wheel.wheelDirectionCS = irr::core::vector3df(0.0, -1.0, 0.0);
wheel.wheelAxleCS = irr::core::vector3df(-5.0, 0.0, 0.0);
wheel.suspensionRestLength = 0.100f;
wheel.wheelRadius = 7.564f;
//wheel.wheelRadius = 7.564f;


wheel.chassisConnectionPointCS =  carNode->getJointNode("front_left_joint")->getPosition();
//vector3df(carNode->getJointNode("front_left_joint")->getPosition().X,carNode->getJointNode("front_left_joint")->getPosition().Y+1,carNode->getJointNode("front_left_joint")->getPosition().Z);
        wheel.isFrontWheel = true;
		vehicle->addWheel(wheel); // Add left front wheel

        wheel.isFrontWheel = false;
        wheel.chassisConnectionPointCS =  carNode->getJointNode("back_left_joint")->getPosition();
			//vector3df(carNode->getJointNode("back_left_joint")->getPosition().X,carNode->getJointNode("back_left_joint")->getPosition().Y+1,carNode->getJointNode("back_left_joint")->getPosition().Z);
				vehicle->addWheel(wheel); // Add left back wheel

        // Right wheels
        wheel.chassisConnectionPointCS = carNode->getJointNode("front_right_joint")->getPosition();
			//vector3df(carNode->getJointNode("front_right_joint")->getPosition().X,carNode->getJointNode("front_right_joint")->getPosition().Y+1,carNode->getJointNode("front_right_joint")->getPosition().Z);

	 wheel.isFrontWheel = true;
        vehicle->addWheel(wheel); // Add left front wheel

        wheel.isFrontWheel = false;
        wheel.chassisConnectionPointCS = carNode->getJointNode("back_right_joint")->getPosition();
			//vector3df(carNode->getJointNode("back_right_joint")->getPosition().X,carNode->getJointNode("back_right_joint")->getPosition().Y+1,carNode->getJointNode("back_right_joint")->getPosition().Z);
        vehicle->addWheel(wheel); // Add left back wheel



        for (irr::u32 i = 0; i < vehicle->getNumWheels(); i++) {
                SWheelInfo& info = vehicle->getWheelInfo(i);
                info.suspensionStiffness = 200.0f;
                info.wheelDampingRelaxation = 2.3f;
                info.wheelDampingCompression = btSqrt(0.2f * 2.3f * 200.0f);
                info.frictionSlip = 2.0f;
                info.rollInfluence = 0.2f;
				info.maxSuspensionTravelCm = 20.0f;
				info.wheelSuspensionForce = 6000.0f;
                vehicle->updateWheelInfo(i);
		}

        rcar->setVehicleReference(vehicle);
}

void UpdateVehicles()
{

for(int i=0;i<1;i++)
		{
if(steeringval==-45.0f || steeringval==45.0f)
{
	carTirefl->setRotation(vector3df(-vehicle->getWheelTransformWS(i).getRotationDegrees().X,vehicle->getSteeringValue(i),0.0));
				carTirefr->setRotation(vector3df(-vehicle->getWheelTransformWS(i+2).getRotationDegrees().X,vehicle->getSteeringValue(i+2),0.0));
}
else
{
	carTirefl->setRotation(vector3df(-vehicle->getWheelTransformWS(i).getRotationDegrees().X,-vehicle->getSteeringValue(i),0.0));
				carTirefr->setRotation(vector3df(-vehicle->getWheelTransformWS(i+2).getRotationDegrees().X,-vehicle->getSteeringValue(i+2),0.0));
}


			carTirebl->setRotation(-vector3df(vehicle->getWheelTransformWS(i+1).getRotationDegrees().X,0.0f,0.0f));

			carTirebr->setRotation(-vector3df(vehicle->getWheelTransformWS(i+3).getRotationDegrees().X,0.0f,0.0f));

//carNode->setPosition(vector3df(vehicle->getRigidBody()->getWorldTransform().getTranslation().X,vehicle->getRigidBody()->getWorldTransform().getTranslation().Y,vehicle->getRigidBody()->getWorldTransform().getTranslation().Z));
carNode->setPosition(vector3df(vehicle->getRigidBody()->getWorldTransform().getTranslation()));
carNode->updateAbsolutePosition();


//carTirefl->setPosition(vector3df(carTirefl->getPosition().X,vehicle->getWheelTransformWS(i).getTranslation().Y,carTirefl->getPosition().Z));
//carTirefl->updateAbsolutePosition();
//				carTirefr->setPosition(vector3df(carTirefr->getPosition().X,vehicle->getWheelTransformWS(i+2).getTranslation().Y,carTirefr->getPosition().Z));
//carTirefr->updateAbsolutePosition();
//
//		carTirebl->setPosition(vector3df(carTirebl->getPosition().X,vehicle->getWheelTransformWS(i+1).getTranslation().Y,carTirebl->getPosition().Z));
//		carTirebl->updateAbsolutePosition();
//			carTirebr->setPosition(vector3df(carTirebr->getPosition().X,vehicle->getWheelTransformWS(i+3).getTranslation().Y,carTirebr->getPosition().Z));
//carTirebr->updateAbsolutePosition();
vehicle->resetSuspension();
}


if(!IsKeyDown)
{
if (steeringval!=0)
				{
				if(steeringval>0.0)
{
	steeringval -=15.0;
}
else if(steeringval<0.0)
{
steeringval +=15.0;
}
}
else
{
	if(eForce>0)
	{
	eForce-=25;
	vehicle->applyEngineForce(eForce,1);
vehicle->applyEngineForce(eForce,0);
vehicle->applyEngineForce(eForce,3);
vehicle->applyEngineForce(eForce,2);
	}
	else if(eForce<0)
	{
eForce+=25;
vehicle->applyEngineForce(eForce,1);
vehicle->applyEngineForce(eForce,0);
vehicle->applyEngineForce(eForce,3);
vehicle->applyEngineForce(eForce,2);
	}
	}
				vehicle->setSteeringValue(-steeringval,0);
vehicle->setSteeringValue(-steeringval,2);
}
IsKeyDown = false;
}
