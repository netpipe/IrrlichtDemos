/*
This is not meant to be a demo.
This application is only to test new features of irrBullet as they are being added.
*/

#include <iostream>
#include <irrlicht.h>
#include <source/irrbullet.h>



using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

ISceneManager *smgr;
IVideoDriver *driver;
IrrlichtDevice *device = 0;

irrBulletWorld *world;

IRigidBody *aircraft;
IRaycastVehicle *vehicle;

ICameraSceneNode *camera;

IRigidBody *body = 0;


IParticleSystemSceneNode *tireSmoke[8];

bool drawWireFrame = false;
bool debugDraw = true;

// Here we set up our collision masks.
// These enumerations are for the type of object that the needed rigid body will be.
enum MyGameMasks
{
    EMGM_NONE = 0,
    EMGM_GROUND = BIT(1),
    EMGM_PROPS = BIT(2)
};

// These are the types that any given rigid body can actually interact with.
// If you set the 2nd parameter in world->addRigidBody() to EMGM_PROPS,
// and set the 3rd parameter to collideWithRigidBody, it would only collide with other
// rigid bodies using that mask. If you set it to EMGM_GROUND, it would only collide
// with the rigid bodies using the EMGM_GROUND mask.
u32 collideWithGround = EMGM_GROUND;
u32 collideWithRigidBody = EMGM_PROPS;
u32 collideWithAll = EMGM_GROUND | EMGM_PROPS;


// To add a rigid body with collision masks, simply add two extra parameters to the creation code:
// world->addRigidBody(shape, EMGM_GROUND, collideWithRigidBody);
// instead of world->addRigidBody(shape)
void createGround()
{
    ISceneNode *Node = smgr->addCubeSceneNode(1.0);
	Node->setScale(vector3df(600,3,600)); // 400, 3, 400
	Node->setPosition(vector3df(200,0,100));
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, driver->getTexture("rockwall.jpg"));

    if(drawWireFrame)
        Node->setMaterialFlag(EMF_WIREFRAME, true);

	ICollisionShape *shape = new IBoxShape(Node, 0, false);

	//shape->setMargin(0.01);

	IRigidBody *body;
	body = world->addRigidBody(shape);
}


void createVehicle(const stringw file, const stringw collFile, const vector3df &pos, const f32 mass)
{
    ISceneNode *Node = smgr->addMeshSceneNode(smgr->getMesh(file.c_str()));
	Node->setPosition(pos);
	Node->setRotation(vector3df(-40,90,0));
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	//Node->setScale(vector3df(2,2,4));


	IGImpactMeshShape *shape = new IGImpactMeshShape(Node, smgr->getMesh(collFile.c_str()), mass);
	//ICollisionShape *shape = new IBoxShape(Node, mass, false);


	aircraft = world->addRigidBody(shape);

    // When using a raycast vehicle, we don't want this rigid body to deactivate.
	aircraft->setActivationState(EAS_DISABLE_DEACTIVATION);

    // We create our vehicle, passing our newly created rigid body as a parameter.
	vehicle = world->addRaycastVehicle(aircraft);


    // Set up our wheel construction info. These values can be changed for each wheel,
    // and the values that you want to keep will stay the same, that way
    // all parameters for each wheel can stay the same for what needs to remain equal,
    // such as directions and suspension rest length.
    SWheelInfoConstructionInfo wheel;
    wheel.chassisConnectionPointCS = vector3df(0.0,-0.88,4.0);
    wheel.wheelDirectionCS = vector3df(0.0,-0.1,0.0);
    wheel.wheelAxleCS = vector3df(-0.5,0.0,0.0);
    wheel.suspensionRestLength = 9.1;
    wheel.wheelRadius = 8.0;
    wheel.isFrontWheel = true;



	vehicle->addWheel(wheel); // Attach point: -3.0

    // Rear wheels will have a larger radius than the front wheel, and they won't be able to turn.
	wheel.chassisConnectionPointCS = vector3df(-3.0,-0.35,-1.0);
	wheel.isFrontWheel = false;
	wheel.wheelRadius = 10.0;

	vehicle->addWheel(wheel);

	wheel.chassisConnectionPointCS = vector3df(3.0,-0.35,-1.0);
	vehicle->addWheel(wheel);


	for (u32 i=0;i<vehicle->getNumWheels();i++)
    {
        SWheelInfo &info = vehicle->getWheelInfo(i);

        info.suspensionStiffness = 0.9f;
        info.wheelDampingRelaxation = 200.8f;
        info.wheelDampingCompression = 100.2f;
        info.frictionSlip = 1000; // Higher values keeps the aircraft from jittering when landing unevenly
        info.rollInfluence = 0.5f; // Rolls easier at higher values


        // We call updateWheel, which takes SWheelInfo as the first parameter,
        // and the ID of the wheel to apply that info to. This must
        // be called after any changes in order for the changes to actually take effect.
        vehicle->updateWheelInfo(i);

        printf("<<<< WHEEL POS: %.4f >>>>\n\n", vehicle->getWheelInfo(i).frictionSlip);
    }

}


void createTank(const stringw file, const stringw collFile, const vector3df &pos, const f32 mass)
{
    IAnimatedMeshSceneNode *Node = smgr->addAnimatedMeshSceneNode(smgr->getMesh(file.c_str()));
	Node->setPosition(pos);
	//Node->setRotation(vector3df(-40,90,0));
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	//Node->setScale(vector3df(2,2,4));
	Node->setJointMode(EJUOR_CONTROL);


	IGImpactMeshShape *shape = new IGImpactMeshShape(Node, smgr->getMesh(collFile.c_str()), mass);
	//ICollisionShape *shape = new IBoxShape(Node, mass, false);


	IRigidBody* tank = world->addRigidBody(shape);
	tank->includeNodeOnRemoval(false);

    tank->setName("Tank");
	tank->getAttributes()->addBool("vehicle", true);
	tank->getAttributes()->addBool("destroyable", true);
	tank->getAttributes()->addBool("isDestroyed", false);

    // When using a raycast vehicle, we don't want this rigid body to deactivate.
	tank->setActivationState(EAS_DISABLE_DEACTIVATION);

    // We create our vehicle, passing our newly created rigid body as a parameter.
	vehicle = world->addRaycastVehicle(tank);


    // Set up our wheel construction info. These values can be changed for each wheel,
    // and the values that you want to keep will stay the same, that way
    // all parameters for each wheel can stay the same for what needs to remain equal,
    // such as directions and suspension rest length.
    SWheelInfoConstructionInfo wheel;
    wheel.chassisConnectionPointCS = vector3df(0.0,-0.88,4.0);
    wheel.wheelDirectionCS = vector3df(0.0,-0.1,0.0);
    wheel.wheelAxleCS = vector3df(-0.5,0.0,0.0);
    wheel.suspensionRestLength = 0.6;
    wheel.wheelRadius = 8.0;
    wheel.isFrontWheel = false;


    /*for(u32 i=0; i < Node->getJointCount(); i++)
    {
        // The bones that we need in this mesh are all named "RoadWheels" with a numercial index at the end.
        // So we do a quick check to make sure that no unwanted bones get through as wheels.
        if(Node->getJointNode(i)->getName()[0] == 'R')
        {
            printf("BONE: %s\n\n", Node->getJointNode(i)->getName());
            wheel.chassisConnectionPointCS = vector3df(-4, Node->getJointNode(i)->getPosition().Y,Node->getJointNode(i)->getPosition().Z);
            vehicle->addWheel(wheel); // Attach point: -3.0
        }
    }

    for(u32 i=0; i < Node->getJointCount(); i++)
    {
        if(Node->getJointNode(i)->getName()[0] == 'R')
        {
            wheel.chassisConnectionPointCS = vector3df(4, Node->getJointNode(i)->getPosition().Y,Node->getJointNode(i)->getPosition().Z);
            vehicle->addWheel(wheel); // Attach point: -3.0
        }
    }*/

    wheel.chassisConnectionPointCS = vector3df(-4, -9.3, 12);
    vehicle->addWheel(wheel);

    wheel.chassisConnectionPointCS = vector3df(4, -9.3, 12);
    vehicle->addWheel(wheel);

    wheel.chassisConnectionPointCS = vector3df(-4, -9.3, -12);
    vehicle->addWheel(wheel);

    wheel.chassisConnectionPointCS = vector3df(4, -9.3, -12);
    vehicle->addWheel(wheel);



	for (u32 i=0;i<vehicle->getNumWheels();i++)
    {
        SWheelInfo &info = vehicle->getWheelInfo(i);

        info.suspensionStiffness = 0.15f;
        info.wheelDampingRelaxation = 2.3f;
        info.wheelDampingCompression = 4.4f;
        info.frictionSlip = 1000; // Higher values keeps the aircraft from jittering when landing unevenly
        info.rollInfluence = 0.1f; // Rolls easier at higher values


        // We call updateWheel, which takes SWheelInfo as the first parameter,
        // and the ID of the wheel to apply that info to. This must
        // be called after any changes in order for the changes to actually take effect.
        vehicle->updateWheelInfo(i);

        printf("<<<< WHEEL POS: %.4f >>>>\n\n", vehicle->getWheelInfo(i).frictionSlip);
    }

}

void addCube(const vector3df &pos, const vector3df &scale, f64 mass)
{
    ISceneNode *Node = smgr->addCubeSceneNode(1.0f);
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, driver->getTexture("crate.jpg"));

	if(drawWireFrame)
        Node->setMaterialFlag(EMF_WIREFRAME, true);

	ICollisionShape *shape = new IBoxShape(Node, mass, false);

	//shape->setMargin(0.01);

	IRigidBody *body;
	body = world->addRigidBody(shape, EMGM_PROPS, collideWithAll);
}

void shootCube(const vector3df &scale, f64 mass)
{
    if(body != 0)
    {
        world->removeCollisionObject(body);
        body->getCollisionShape()->getSceneNode()->remove();
        delete body;
    }
	vector3df pos(camera->getPosition().X,camera->getPosition().Y,camera->getPosition().Z);

	ISceneNode *Node = smgr->addCubeSceneNode(1.0f);
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, driver->getTexture("rockwall.jpg"));

	if(drawWireFrame)
        Node->setMaterialFlag(EMF_WIREFRAME, true);

	ICollisionShape *shape = new IBoxShape(Node, mass, true);

	//shape->setMargin(0.01);

	IRigidBody* body = world->addRigidBody(shape);

	core::vector3df rot = camera->getRotation();
    core::matrix4 mat;
    mat.setRotationDegrees(rot);
    vector3df forwardDir(vector3df(mat[8],mat[9],mat[10]) * 120);

    body->setLinearVelocity(forwardDir);

    body->setDamping(0.2,0.2);
    body->setFriction(0.4f);

    ICollisionObjectAffectorDelete *deleteAnim = new ICollisionObjectAffectorDelete(8000);
    body->addAffector(deleteAnim);

    body->getAttributes()->addBool("collide", true);
}

void shootSphere(const vector3df &scale, f64 mass)
{
	vector3df pos(camera->getPosition().X,camera->getPosition().Y,camera->getPosition().Z);

	ISceneNode *Node = smgr->addSphereSceneNode();
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, driver->getTexture("rockwall.jpg"));

	if(drawWireFrame)
        Node->setMaterialFlag(EMF_WIREFRAME, true);

	ICollisionShape *shape = new ISphereShape(Node, mass, true);

	//shape->setMargin(0.01);

	IRigidBody *body = world->addRigidBody(shape, EMGM_PROPS, collideWithAll);
	body->setDamping(0.2,0.2);

    // Since we use a "bumpy" triangle mesh for the terrain, we need to set the CCD radius and
    // motion threshold to ensure that we don't have any tunneling problems.
    // This doesn't work very well for box shapes.
	// ALSO: If you use setLocalScaling() on the collision shape, be sure to call this again
	// after scaling to make sure that it matches the actual object.
	body->applyCalculatedCcdValues();

	core::vector3df rot = camera->getRotation();
    core::matrix4 mat;
    mat.setRotationDegrees(rot);
    vector3df forwardDir(vector3df(mat[8],mat[9],mat[10]) * 120);

    body->setLinearVelocity(forwardDir);
}


void createFire(const vector3df& position)
{
    scene::IParticleSystemSceneNode* ps =
		smgr->addParticleSystemSceneNode(false);


	scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-7,0,-7,7,1,7), // emitter size
		core::vector3df(0.0f,0.02f,0.0f),   // initial direction
		80,100,                             // emit rate
		video::SColor(0,255,255,255),       // darkest color
		video::SColor(0,255,255,255),       // brightest color
		800,2000,0,                         // min and max age, angle
		core::dimension2df(5.f,5.f),         // min size
		core::dimension2df(10.f,10.f));        // max size

	ps->setEmitter(em); // this grabs the emitter
	em->drop(); // so we can drop it here without deleting it

	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

	ps->addAffector(paf); // same goes for the affector
	paf->drop();

	ps->setPosition(position);
	ps->setScale(core::vector3df(1,1,1));
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("fire.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
}



class EventReceiver : public IEventReceiver  {

public:

	virtual bool OnEvent(const SEvent &TEvent) {

		if(TEvent.EventType == EET_MOUSE_INPUT_EVENT && (TEvent.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN))
		{

			shootCube(vector3df(2,2,2), 1);

			return true;
		}

		else
		if(TEvent.EventType == EET_MOUSE_INPUT_EVENT && (TEvent.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN))
		{

			shootSphere(vector3df(0.2,0.2,0.2), 0.2);

			return true;
		}

		else
		if(TEvent.EventType == EET_KEY_INPUT_EVENT && (TEvent.KeyInput.Key == KEY_KEY_P) && TEvent.KeyInput.PressedDown == false)
		{
            world->pauseSimulation(!world->simulationPaused());
            return true;
		}
		return false;
	}
};



int main()
{
    EventReceiver receiver;
    device =
        createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
            false, false, false, &receiver);

    if (device == 0)
        return 1;

    device->setWindowCaption(L"irrBullet Example");


    device->getFileSystem()->addFolderFileArchive("media/");



    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    IGUIEnvironment* env = device->getGUIEnvironment();



    // Create the irrBullet world
    world = createIrrBulletWorld(device, true, true);

    world->setDebugMode(EPDM_DrawAabb |
            EPDM_DrawContactPoints);




    createGround();

    u32 rows = 0; // 10
    u32 height = 0; // 10
    for(u32 j=0; j < height; j++)
    {
        for(u32 i=0; i < rows; i++)
        {
            addCube(vector3df(4 * i, 300 + 4 * j + 4, 0), vector3df(3,3,3), 3);
        }
    }

    smgr->addLightSceneNode(0, vector3df(20, 40, -50), SColorf(1.0f, 1.0f, 1.0f), 4000.0f);



    //createVehicle(L"harrier.b3d", L"coll.obj", vector3df(20,5,10), 12); // vector3df(20,5,10)
    //createTank(L"JagdPanther.b3d", L"JagdPanther.b3d", vector3df(20,5,10), 12);

    for(int i=0; i < 8; i++)
    {
        srand ( time(NULL) );

        u32 force = rand() % 40 + 20;

        createTank(L"JagdPanther.b3d", L"JagdPanther.b3d", vector3df((i*40),5,-120), 12);
        static_cast<IRigidBody*>(world->getCollisionObject(world->getNumCollisionObjects()-1))->setLinearVelocity(vector3df(0,0,force));
    }

    /*for(int i=0; i < vehicle->getNumWheels(); i++)
    {
        tireSmoke[i] = smgr->addParticleSystemSceneNode(false);

        IParticleEmitter* em = tireSmoke[i]->createPointEmitter(
            vector3df(0.0, 0.002, 0.0), 420, 500, SColor(0, 200, 200, 200),
            SColor(0, 200, 200, 200), 500, 820, 12);

        em->setMinStartSize(dimension2d<f32>(0.5, 0.5));
        em->setMaxStartSize(dimension2d<f32>(1.0, 1.0));
        tireSmoke[i]->setEmitter(em);
        em->drop();

        // Create and set affector
        scene::IParticleAffector* paf = tireSmoke[i]->createFadeOutParticleAffector();
        tireSmoke[i]->addAffector(paf);
        paf->drop();

        // Adjust some material settings
        tireSmoke[i]->setMaterialFlag(video::EMF_LIGHTING, false);
        tireSmoke[i]->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
        tireSmoke[i]->setMaterialTexture(0, driver->getTexture("Light_Smoke.tga"));
        tireSmoke[i]->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    }*/


	camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(50,15,4));
	camera->setTarget(static_cast<IRigidBody*>(world->getCollisionObjectByName("Tank"))->getWorldTransform().getTranslation());


    // TERRAIN
	/*IMeshSceneNode *Node = smgr->addMeshSceneNode(smgr->getMesh("terrainMain.b3d")->getMesh(0));
	Node->setPosition(vector3df(0,0,0));
	Node->setMaterialFlag(video::EMF_LIGHTING, false);
	Node->getMesh()->setHardwareMappingHint(EHM_STATIC);

	//Node->setDebugDataVisible(EDS_BBOX_BUFFERS);

    // For the terrain, instead of adding a cube or sphere shape, we are going to
    // add a BvhTriangleMeshShape. This is the standard trimesh shape
    // for static objects. The first parameter is of course the node to control,
    // the second parameter is the collision mesh, incase you want a low-poly collision mesh,
    // and the third parameter is the mass.
	ICollisionShape *shape = new IBvhTriangleMeshShape(Node, smgr->getMesh("terrainMain.b3d"), 0.0);

	shape->setMargin(0.07);

    // The rigid body will be placed at the origin of the node that the collision shape is controlling,
    // so we do not need to set the position after positioning the node.
	IRigidBody *terrain = world->addRigidBody(shape);
	terrain->setGravity(vector3df(0,0,0));


    shape->setLocalScaling(vector3df(4,4,4), ESP_BOTH);




	// When setting a rigid body to a static object, please be sure that you have
	// that object's mass set to 0.0. Otherwise, undesired results will occur.
	terrain->setCollisionFlags(ECF_STATIC_OBJECT);*/

	IMesh* mesh = smgr->addHillPlaneMesh("myHill",
		core::dimension2d<f32>(10,10),
		core::dimension2d<u32>(10,10), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(10,10));

	IMeshSceneNode* softbodyNode = smgr->addMeshSceneNode(mesh);
	softbodyNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
	softbodyNode->setMaterialTexture(0, driver->getTexture("detailmap3.jpg"));
	softbodyNode->setPosition(vector3df(0,100,0));
	//IMeshSceneNode* softbodyNode = smgr->addMeshSceneNode(smgr->getMesh("dwarf.x"));
	//IMeshSceneNode* softbodyNode = smgr->addCubeSceneNode(10.0f, 0, -1, vector3df(0, 0, 0), core::vector3df(0, 0, 0), vector3df(1.0f, 1.0f, 1.0f));
	//IMeshSceneNode* softbodyNode = smgr->addSphereSceneNode(5.0f, 16, 0, -1, vector3df(0, 0, 0), vector3df(0, 0, 0), vector3df(1.0f, 1.0f, 1.0f));
	ISoftBody* softbody = world->addSoftBody(softbodyNode);





    world->pauseSimulation(true);
    debugDraw = false;


    u32 TimeStamp = device->getTimer()->getTime(), DeltaTime = 0;
	while(device->run())
    {
        driver->beginScene(true, true, SColor(255,100,101,140));

        DeltaTime = device->getTimer()->getTime() - TimeStamp;
		TimeStamp = device->getTimer()->getTime();

        // Step the simulation with our delta time
        world->stepSimulation(DeltaTime*0.001f, 120);

        /*for(u32 i=0; i < world->getNumCollisionObjects(); i++)
        {
            ICollisionObject* obj = world->getCollisionObject(i);

            if(obj->getAttributes()->existsAttribute("destroyable") && obj->hasCollidedWithAttribute("collide"))
            {
                if(obj->getAttributes()->getAttributeAsBool("isDestroyed") == false)
                {
                    obj->getAttributes()->setAttribute("isDestroyed", true);
                    //obj->setCollisionFlags(ECF_STATIC_OBJECT);
                    world->removeCollisionObject(obj);
                    createFire(static_cast<IRigidBody*>(obj)->getWorldTransform().getTranslation());
                }
            }
        }*/


        /*for(int i=0; i < world->getNumManifolds(); i++)
        {
            ICollisionCallbackInformation *info = world->getCollisionCallback(i);

            bool hasCollidedAttribute = (info->getBody0()->getAttributes()->existsAttribute("collide"));
            ICollisionObject* obj = (hasCollidedAttribute==true) ? info->getBody0() : info->getBody1();

            if(obj->getAttributes()->getAttributeAsBool("collide") == true)
            {
                obj->getAttributes()->setAttribute("collide", false);
                world->addToDeletionQueue(obj);
            }
        }*/

        /*for(u32 i=0; i < world->getNumCollisionObjects(); i++)
        {
            ICollisionObject *obj = world->getCollisionObject(i);
            if(obj->getObjectType() == ECOT_RIGID_BODY)
            {
                irr::io::IAttributes *attributes = obj->getAttributes();
                if(attributes->getAttributeAsBool("delete") == true)
                {
                    if(attributes->getAttributeAsFloat("delay") < 500)
                    {
                        attributes->setAttribute("delay", f32(attributes->getAttributeAsFloat("delay") + 0.5));
                        printf("PROP: %.2f\n", attributes->getAttributeAsFloat("delay"));
                    }

                    else
                    if(attributes->getAttributeAsFloat("delay") >= 500)
                    {
                        obj->getCollisionShape()->getSceneNode()->remove();
                        world->removeCollisionObject(obj);
                    }
                }
            }
        }*/

        /*if(body != 0)
        {
            if(body->checkCollideWith(aircraft) == true)
            {
                printf("COLLIDED");
                world->removeRigidBody(body);
                body->getCollisionShape()->getNode()->remove();
                delete body;
                body = 0;
            }
        }*/


        //printf("\n\nWHEELS: %d\n\n", vehicle->getNumWheels());
        /*for(u32 i=0; i < vehicle->getNumWheels(); i++)
        {
            //vehicle->updateWheelTransform(i,true);
            SWheelInfo& wheel = vehicle->getWheelInfo(i);

            tireSmoke[i]->setPosition(wheel.worldTransform.getTranslation());
            tireSmoke[i]->getEmitter()->setMaxParticlesPerSecond(wheel.skidInfo * 50);
            tireSmoke[i]->getEmitter()->setMinParticlesPerSecond(wheel.skidInfo * 0.4);

            if(i < 5)
            {
                IAnimatedMeshSceneNode *node = static_cast<IAnimatedMeshSceneNode*>(aircraft->getCollisionShape()->getNode());
                IBoneSceneNode* bone = node->getJointNode(i);
                matrix4 inverseMat, mat;

                mat.makeIdentity();


                matrix4 rbt = vehicle->getRigidBody()->getCollisionShape()->getNode()->getAbsoluteTransformation();
                mat.setTranslation(
                    vector3df(rbt.getTranslation().X - wheel.worldTransform.getTranslation().X,
                        rbt.getTranslation().Y - wheel.worldTransform.getTranslation().Y,
                        rbt.getTranslation().Z - wheel.worldTransform.getTranslation().Z));


                matrix4 m = bone->getParent()->getRelativeTransformation();
                m *= mat;

                bone->setPosition(m.getTranslation());
                bone->setRotation(m.getRotationDegrees());
                bone->setScale(m.getScale());

            }
        }*/

        //device->setWindowCaption(stringw(static_cast<f32>(vehicle->getWheelInfo(2).skidInfo)).c_str());

        //device->setWindowCaption(stringw(vehicle->getWheelInfo(2).skidInfo).c_str());

        world->debugDrawWorld(true);
        // This call will draw the technical properties of the physics simulation
        // to the GUI environment.
        world->debugDrawProperties(true);

        smgr->drawAll();
        env->drawAll();

        driver->endScene();
    }
    // We're done with the IrrBullet world, so we free the memory that it takes up.
    delete world;
    device->drop();

    return 0;
}

