// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.
#include "DebugDraw.hpp"
#include "VehicleExample.hpp"

irr::core::vector3df pos(0, 10, 190);
btRaycastVehicle::btVehicleTuning m_tuning;

CVehicleExample::CVehicleExample() {}

void CVehicleExample::createVehicle() {

    irr::scene::IAnimatedMeshSceneNode *Node = device->getSceneManager()->addAnimatedMeshSceneNode(device->getSceneManager()->getMesh("assets/chassis/el-camino-monstertruck.obj"));
    Node->setPosition(pos);
    Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);

    IGImpactMeshShape *shape = new IGImpactMeshShape(Node, device->getSceneManager()->getMesh("assets/chassis/el-camino-monstertruck.obj"), btScalar(800));

    tank = world->addRigidBody(shape);

    tank->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION);

    tank->setDamping(0.4, 0.4);

    vehicle = world->addRaycastVehicle(tank);

    irr::core::vector3df halfExtents(1, btScalar(0.5), 2);
    //The direction of the raycast, the btRaycastVehicle uses raycasts instead of simiulating the wheels with rigid bodies
    irr::core::vector3df wheelDirectionCS0(0, -1, 0);

    //The axis which the wheel rotates arround
    irr::core::vector3df wheelAxleCS(-1, 0, 0);

    float suspensionRestLength(0.7);

    float wheelWidth(0.8);

    float wheelRadius(1.5);

    //The height where the wheels are connected to the chassis
    float connectionHeight(.2);

    //All the wheel configuration assumes the vehicle is centered at the origin and a right handed coordinate system is used
    irr::core::vector3df wheelConnectionPoint(halfExtents.X, connectionHeight, halfExtents.Z);

    SWheelInfoConstructionInfo wheel;
    wheel.chassisConnectionPointCS = wheelConnectionPoint;
    wheel.wheelDirectionCS = wheelDirectionCS0;
    wheel.wheelAxleCS = wheelAxleCS;
    wheel.suspensionRestLength = suspensionRestLength;
    wheel.wheelRadius = wheelRadius;
    wheel.isFrontWheel = true;

    //Adds the front wheels

    //right wheel
    vehicle->addWheel(wheel);

    //left wheel
    wheel.chassisConnectionPointCS = irr::core::vector3df(wheelConnectionPoint.X - 2, wheelConnectionPoint.Y, wheelConnectionPoint.Z);
    vehicle->addWheel(wheel);

    //back wheels
    wheel.isFrontWheel = false;

    //right wheel
    wheel.chassisConnectionPointCS = irr::core::vector3df(halfExtents.X, connectionHeight, halfExtents.Z - 4);
    vehicle->addWheel(wheel);

    //left wheel
    wheel.chassisConnectionPointCS = irr::core::vector3df(wheelConnectionPoint.X - 2, wheelConnectionPoint.Y, wheelConnectionPoint.Z - 4);
    vehicle->addWheel(wheel);

    for (irr::u32 i=0;i<vehicle->getNumWheels();i++) {

        SWheelInfo &info = vehicle->getWheelInfo(i);

        info.suspensionStiffness = 50;
        info.wheelDampingRelaxation = 20.0f;
        info.wheelDampingCompression = 20.0f;
        info.frictionSlip = 1000;
        info.rollInfluence = 0.1f;

        // We call updateWheel, which takes SWheelInfo as the first parameter,
        // and the ID of the wheel to apply that info to. This must
        // be called after any changes in order for the changes to actually take effect.
        vehicle->updateWheelInfo(i);
    }

}

bool CVehicleExample::OnEvent(const irr::SEvent& event) {

	if (!device)
		return false;

	switch(event.EventType) {
        case irr::EET_MOUSE_INPUT_EVENT:
	    {
		if(event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
		    shootCube(irr::core::vector3df(2,2,2), 1);
		    return true;
		}

		else
		    if(event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN) {
			shootSphere(irr::core::vector3df(0.2,0.2,0.2), 0.2);
			return true;
		    }
	    }
        break;

        case irr::EET_KEY_INPUT_EVENT:
        {
            if(event.KeyInput.Key == irr::KEY_KEY_P && event.KeyInput.PressedDown == false)
            {
                world->pauseSimulation(!world->simulationPaused());
                return true;
            }

            else
		if(event.KeyInput.Key == irr::KEY_KEY_R && event.KeyInput.PressedDown == false)
            {
                while(world->getNumCollisionObjects() > 0)
                {
                    world->removeCollisionObject(world->getCollisionObjectByIndex(0));
                }
                createTerrain();
                createBoxes();
                return true;
            }
        }
        break;
        default:
            break;
    }
    return false;

}

void CVehicleExample::createTerrain() {

    // TERRAIN
    irr::scene::IMeshSceneNode *Node = device->getSceneManager()->addMeshSceneNode(device->getSceneManager()->getMesh("assets/map/olivermath/olivermath-1.obj")->getMesh(0));
    Node->setPosition(irr::core::vector3df(0,0,0));
    Node->setMaterialFlag(irr::video::EMF_LIGHTING, false);

    // For the terrain, instead of adding a cube or sphere shape, we are going to
    // add a BvhTriangleMeshShape. This is the standard trimesh shape
    // for static objects. The first parameter is of course the node to control,
    // the second parameter is the collision mesh, incase you want a low-poly collision mesh,
    // and the third parameter is the mass.
	ICollisionShape *shape = new IBvhTriangleMeshShape(Node, device->getSceneManager()->getMesh("assets/map/olivermath/olivermath-1.obj"), 0.0);

	shape->setMargin(0.07);

    // The rigid body will be placed at the origin of the node that the collision shape is controlling,
    // so we do not need to set the position after positioning the node.
	auto terrain = world->addRigidBody(shape);


    shape->setLocalScaling(irr::core::vector3df(4,4,4), EScalingPair::ESP_BOTH);

	// When setting a rigid body to a static object, please be sure that you have
	// that object's mass set to 0.0. Otherwise, undesired results will occur.
	terrain->setCollisionFlags(ECollisionFlag::ECF_STATIC_OBJECT);
}

void CVehicleExample::runExample() {

    debugDraw = false;
    drawWireFrame = false;

    rows = 10;
    columns = 10;

    device.reset(createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(640, 480), 16, false, false, false, this));

    device->setWindowCaption(L"DMUX");
    device->getFileSystem()->addFileArchive("asset/");


    device->getSceneManager()->addLightSceneNode(0, irr::core::vector3df(20, 40, -50), irr::video::SColorf(1.0f, 1.0f, 1.0f), 4000.0f);

    ////////////////////////////
    // Create irrBullet World //
    ////////////////////////////
    world.reset(createIrrBulletWorld(device, true, debugDraw));

    world->setDebugMode(irrPhysicsDebugMode::EPDM_DrawAabb | irrPhysicsDebugMode::EPDM_DrawContactPoints);
    
    //world->setPropertiesTextPosition(EDPT_BOTTOM_RIGHT);
    world->setGravity(irr::core::vector3df(0,-10,0));


    camera = device->getSceneManager()->addCameraSceneNodeFPS();
    camera->setPosition(irr::core::vector3df(0, 3, 200));

    createGround();
    createVehicle();

    // Set our delta time and time stamp
    irr::u32 TimeStamp = device->getTimer()->getTime();
    irr::u32 DeltaTime = 0;

    DebugDraw debugDraw(device);
    debugDraw.setDebugMode(
			   btIDebugDraw::DBG_DrawWireframe |
			   btIDebugDraw::DBG_DrawAabb |
			   btIDebugDraw::DBG_DrawContactPoints |
			   //btIDebugDraw::DBG_DrawText |
			   //btIDebugDraw::DBG_DrawConstraintLimits |
			   btIDebugDraw::DBG_DrawConstraints //|
			   );
    world->getPointer()->setDebugDrawer(&debugDraw);

    irr::video::SMaterial debugMat;
    debugMat.Lighting = false;

    const bool debug_draw_bullet = true;

    while(device->run()) {

        device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255,100,101,140));

        DeltaTime = device->getTimer()->getTime() - TimeStamp;
	TimeStamp = device->getTimer()->getTime();

	// Step the simulation with our delta time
        world->stepSimulation(DeltaTime*0.001f, 120);

	vehicle->applyEngineForce(1000.0f, 2);
	vehicle->applyEngineForce(1000.0f, 3);

        device->getSceneManager()->drawAll();

	if(debug_draw_bullet) {
	    device->getVideoDriver()->setMaterial(debugMat);
	    device->getVideoDriver()->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
	    world->getPointer()->debugDrawWorld();
	}

        device->getVideoDriver()->endScene();
    }
}


void CVehicleExample::createBoxes() {

    for(irr::u32 j=0; j < columns; j++)
    {
        for(irr::u32 i=0; i < rows; i++)
        {
            addCube(irr::core::vector3df(3*j, 0+3*i+3, 0), irr::core::vector3df(3,3,3), 3);
        }
    }
}

void CVehicleExample::createGround() {

    irr::scene::ISceneNode *Node = device->getSceneManager()->addCubeSceneNode(1.0);
	Node->setScale(irr::core::vector3df(3000,3,3000)); // 400, 3, 400
	Node->setPosition(irr::core::vector3df(20,0,10));
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture("rockwall.jpg"));

    if(drawWireFrame)
        Node->setMaterialFlag(irr::video::EMF_WIREFRAME, true);

	ICollisionShape *shape = new IBoxShape(Node, 0, false);

	IRigidBody *body;
	body = world->addRigidBody(shape);
}

CVehicleExample::~CVehicleExample()
{}
