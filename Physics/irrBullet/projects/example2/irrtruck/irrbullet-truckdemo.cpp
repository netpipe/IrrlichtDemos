// MISSING CODE HERE (headers, variables, irrlicht stuff.......)
// Irrlicht + IrrBullet = Vehicle + Wheels + Trailer (joints) etc... - By Jorge Rosa - http://sites.google.com/site/jorgerosaportfolio


#include <irrlicht.h>
#include <irrBullet.h>
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


// Irrbullet stuff:
enum ECollisionGroupTypes{
COL_NOTHING = 0, // <-- Collide with nothing
COL_TERRAIN = BIT(1),
COL_WORLD_OBJECT = BIT(2),
COL_VEHICLE = BIT(3)
};

int objectCollidesWith = COL_VEHICLE | COL_TERRAIN | COL_WORLD_OBJECT;
int vehicleRaycastCollidesWith = COL_TERRAIN | COL_WORLD_OBJECT;
int terrainCollidesWith = COL_VEHICLE | COL_WORLD_OBJECT;

// Define Some Irrlicht Global Variables:
IrrlichtDevice *device = 0; // Create Device
video::IVideoDriver* driver  = 0; // Create Driver
scene::ISceneManager* smgr   = 0; // Create Scene Manager







int main(int argc, char** argv){

    // MISSING CODE HERE (normal irrlicht stuff.......)

    // INITIALIZE PHYSICS WORLD:
    irrBulletWorld* world = createIrrBulletWorld(device, true, true); //  irr::IrrlichtDevice *const Device, bool useGImpact, bool useDebugDrawer
    world->setDebugMode(EPDM_DrawAabb | EPDM_DrawContactPoints);
    world->setGravity(vector3df(0, -10, 0));
    world->registerGImpactAlgorithm();

    f32 sm  = 0.02;    // setMargin road only (base)
    f32 smo = 0.02;    // setMargin to all other objects
    f32 sr  = 0.01;    // setRestitution
    vector3df globalScaling = vector3df(1.0,1.0,1.0);

    // NODE -> TERRAIN:
    // add terrain scene node
    scene::ITerrainSceneNode* terrainNode = smgr->addTerrainSceneNode(
        "terrain_heightmap.png",                  // data: heightmap (256x256)
        0,                                        // parent node
        -1,                                       // node id
        core::vector3df(-233.f, -17.0f, -8.0f),   // position
        core::vector3df(0.0f, 0.0f, 0.0f),        // rotation
        core::vector3df(1.0f, 0.1f, 2.0f),        // scale
        video::SColor(255, 255, 255, 255 ),       // vertexColor
        10,                                       // maxLOD (simplify distant meshes)
        scene::ETPS_17,                           // patchSize (9,17,33,65)
        0                                         // smoothFactor
        );

    // MISSING CODE HERE (.......)

    // Add to physics world:
    scene::CDynamicMeshBuffer meshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
    terrainNode->getMeshBufferForLOD(meshBuffer, 0);
    scene::SMesh terraMesh;
    terraMesh.addMeshBuffer(&meshBuffer);
    ICollisionShape* terraShape = new IBvhTriangleMeshShape(terrainNode, &terraMesh, 0.0); // Create terrain shape
    world->addRigidBody(terraShape);



    // NODES -> VEHICLE:
    irr::scene::IAnimatedMeshSceneNode* vehicleNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("vehicle_truck.b3d"));
    vehicleNode->setPosition(vector3df(0,0,0));


    // Create impacts mesh shapes:
    IGImpactMeshShape* vshape = new IGImpactMeshShape(vehicleNode, device->getSceneManager()->getMesh("vehicle_truck_raycast.b3d"), 1000.0);

    vshape->setMargin(smo);

    IRigidBody* rigidBody = world->addRigidBody(vshape, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
    vshape->setLocalScaling(globalScaling, ESP_BOTH);
    rigidBody->setLinearFactor(vector3df(0,1,1));  // Allowed positions axis (y,z)
    rigidBody->setAngularFactor(vector3df(1,0,0)); // Allowed rotations axis (x)

    rigidBody->getAttributes()->addBool("vehicle", true);

    rigidBody->includeNodeOnRemoval(false);
    // When using a raycast vehicle, we don't want this rigid body to deactivate.
    rigidBody->setActivationState(EAS_DISABLE_DEACTIVATION);
    // rigidBody->setDamping(0.000001,0.000001);
    rigidBody->setFriction(20);  // rigidBody->setFriction(7.0f);
    rigidBody->setRestitution(0); // void bouncy
    // rigidBody->setMassProps(200, vector3df(1,0,0));
    rigidBody->setDamping(0.1,0.1);
    // rigidBody->setGravity(vector3df(0, -30, 0));


    // NODES -> WHEELS:
    irr::scene::IAnimatedMeshSceneNode* rodaNode[10];
    rodaNode[1] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("wheel_big.b3d"));  // right_front_wheel
    rodaNode[2] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("wheel_big.b3d"));  // left_front_wheel
    rodaNode[3] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("wheel_big.b3d"));  // right_back_wheel01
    rodaNode[4] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("wheel_big.b3d"));  // left_back_wheel01



    f32 wsm   = 0.07;   // setMargin
    f32 wsf   = 10.0;   // setFriction
    f32 dap   = 0.3;    // setDamping -> Set some damping on the rigid body because the raycast vehicles tend to bounce a lot without a lot of tweaking.
    f32 wmass = 800.0;  // mass


    // IGImpactMeshShape* wShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); // IConvexHullShape* wShape1 = new IConvexHullShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0); // IBoxShape* wShape1 = new IBoxShape(rodaNode1, 10.0, true);
    ISphereShape* wShape1 = new ISphereShape(rodaNode[1], wmass, true);
    // wShape1->calculateLocalInertia(wmass, vector3df(1000,1000,1000));
    IRigidBody *wbody1 = world->addRigidBody(wShape1, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
    wShape1->setMargin(wsm);
    wbody1->setFriction(wsf);
    wbody1->setDamping(dap,dap);
    wbody1->getAttributes()->addBool("wheel", true);
    // wbody1->setActivationState(EAS_DISABLE_DEACTIVATION);

    // IGImpactMeshShape* wShape2 = new IGImpactMeshShape(rodaNode2, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); // IConvexHullShape* wShape2 = new IConvexHullShape(rodaNode2, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0);
    ISphereShape* wShape2 = new ISphereShape(rodaNode[2], wmass, true);
    IRigidBody *wbody2 = world->addRigidBody(wShape2, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
    wShape2->setMargin(wsm);
    wbody2->setFriction(wsf);
    wbody2->setDamping(dap,dap);
    wbody2->getAttributes()->addBool("wheel", true);
    // wbody2->setActivationState(EAS_DISABLE_DEACTIVATION);

    // IGImpactMeshShape* wShape3 = new IGImpactMeshShape(rodaNode3, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); // IConvexHullShape* wShape2 = new IConvexHullShape(rodaNode2, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0);
    ISphereShape* wShape3 = new ISphereShape(rodaNode[3], wmass, true);
    IRigidBody *wbody3 = world->addRigidBody(wShape3, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
    wShape3->setMargin(wsm);
    wbody3->setFriction(wsf);
    wbody3->setDamping(dap,dap);
    wbody3->getAttributes()->addBool("wheel", true);
    // wbody3->setActivationState(EAS_DISABLE_DEACTIVATION);

    // IGImpactMeshShape* wShape4 = new IGImpactMeshShape(rodaNode4, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); // IConvexHullShape* wShape2 = new IConvexHullShape(rodaNode2, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0);
    ISphereShape* wShape4 = new ISphereShape(rodaNode[4], wmass, true);
    IRigidBody *wbody4 = world->addRigidBody(wShape4, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
    wShape4->setMargin(wsm);
    wbody4->setFriction(wsf);
    wbody4->setDamping(dap,dap);
    wbody4->getAttributes()->addBool("wheel", true);
    // wbody4->setActivationState(EAS_DISABLE_DEACTIVATION);




    // FINISHING: Create vehicle
    IRaycastVehicle* vehicle = world->addRaycastVehicle(rigidBody); // Assuming world is pointer to your irrBullet World
    // world->addRaycastVehicle(wbody1); // Assuming world is pointer to your irrBullet World

    vehicle->getVehicleRaycaster()->setUseFilter(true);
    vehicle->getVehicleRaycaster()->setCollisionFilterGroup(COL_VEHICLE);
    vehicle->getVehicleRaycaster()->setCollisionFilterMask(vehicleRaycastCollidesWith);






    // WHEELS "attached" to CHASSI, the "secret" is to use "constrains":

    btTransform frameInA[10]; // Chassi
    btTransform frameInB[10]; // Tyre

    frameInA[0] = btTransform::getIdentity();
    frameInA[0].setOrigin(irrlichtToBulletVector(vehicleNode->getJointNode("01")->getPosition())); // position
    frameInB[0] = btTransform::getIdentity();
    frameInB[0].setOrigin(irrlichtToBulletVector(vector3df(0,0,0))); // rotation

    frameInA[1] = btTransform::getIdentity();
    frameInA[1].setOrigin(irrlichtToBulletVector(vehicleNode->getJointNode("02")->getPosition())); // position
    frameInB[1] = btTransform::getIdentity();
    frameInB[1].setOrigin(irrlichtToBulletVector(vector3df(0,0,0))); // rotation

    frameInA[2] = btTransform::getIdentity();
    frameInA[2].setOrigin(irrlichtToBulletVector(vehicleNode->getJointNode("03")->getPosition())); // position
    frameInB[2] = btTransform::getIdentity();
    frameInB[2].setOrigin(irrlichtToBulletVector(vector3df(0,0,0))); // rotation

    frameInA[3] = btTransform::getIdentity();
    frameInA[3].setOrigin(irrlichtToBulletVector(vehicleNode->getJointNode("04")->getPosition())); // position
    frameInB[3] = btTransform::getIdentity();
    frameInB[3].setOrigin(irrlichtToBulletVector(vector3df(0,0,0))); // rotation

    btRigidBody* bodyA = rigidBody->getPointer(); // get chassi --->  parentObject->getRigidBody()->getPointer();

    btRigidBody* bodyB[10];
    bodyB[0] = wbody1->getPointer(); // get wheel  --->  obj->getRigidBody()->getPointer();
    bodyB[1] = wbody2->getPointer(); // get wheel  --->  obj->getRigidBody()->getPointer();
    bodyB[2] = wbody3->getPointer(); // get wheel  --->  obj->getRigidBody()->getPointer();
    bodyB[3] = wbody4->getPointer(); // get wheel  --->  obj->getRigidBody()->getPointer();

    btGeneric6DofSpringConstraint* C1 = new btGeneric6DofSpringConstraint(*bodyA, *bodyB[0], frameInA[0], frameInB[0], true);
    btGeneric6DofSpringConstraint* C2 = new btGeneric6DofSpringConstraint(*bodyA, *bodyB[1], frameInA[1], frameInB[1], true);
    btGeneric6DofSpringConstraint* C3 = new btGeneric6DofSpringConstraint(*bodyA, *bodyB[2], frameInA[2], frameInB[2], true);
    btGeneric6DofSpringConstraint* C4 = new btGeneric6DofSpringConstraint(*bodyA, *bodyB[3], frameInA[3], frameInB[3], true);

    C1->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    C1->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit

    C2->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    C2->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit

    C3->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    C3->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit

    C4->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    C4->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit

    world->getPointer()->addConstraint(C1, true);
    world->getPointer()->addConstraint(C2, true);
    world->getPointer()->addConstraint(C3, true);
    world->getPointer()->addConstraint(C4, true);




    // Create wheel 1 - info:
    SWheelInfoConstructionInfo wheel1;
    wheel1.chassisConnectionPointCS = vehicleNode->getJointNode("01")->getPosition();
    wheel1.wheelDirectionCS = irr::core::vector3df(0.0, -0.1, 0.0);
    wheel1.wheelAxleCS = irr::core::vector3df(0.2, 0.0, 0.0);
    wheel1.suspensionRestLength = 0.3;  // 0.30;
    wheel1.wheelRadius = 8;            // wheel (node) radius (10)
    wheel1.isFrontWheel = true;
    vehicle->addWheel(wheel1);          // Add left front wheel

    // Create wheel 2 - info:
    SWheelInfoConstructionInfo wheel2;
    wheel2.chassisConnectionPointCS = vehicleNode->getJointNode("02")->getPosition();
    wheel2.wheelDirectionCS = irr::core::vector3df(0.0, -0.1, 0.0);
    wheel2.wheelAxleCS = irr::core::vector3df(-0.2, 0.0, 0.0);
    wheel2.suspensionRestLength = 0.3;  // 0.30;
    wheel2.wheelRadius = 8;            // wheel (node) radius (10)
    wheel2.isFrontWheel = true;
    vehicle->addWheel(wheel2);          // Add left front wheel

    // Create wheel 3 - info:
    SWheelInfoConstructionInfo wheel3;
    wheel3.chassisConnectionPointCS = vehicleNode->getJointNode("03")->getPosition();
    wheel3.wheelDirectionCS = irr::core::vector3df(0.0, -0.1, 0.0); // irr::core::vector3df(0.0, -1.0, 0.0);
    wheel3.wheelAxleCS = irr::core::vector3df(0.2, 0.0, 0.0);
    wheel3.suspensionRestLength = 0.3;  // 0.30;
    wheel3.wheelRadius = 8;            // wheel (node) radius (15)
    wheel3.isFrontWheel = false;
    vehicle->addWheel(wheel3);          // Add right back wheel

    // Create wheel 4 - info:
    SWheelInfoConstructionInfo wheel4;
    wheel4.chassisConnectionPointCS = vehicleNode->getJointNode("04")->getPosition();
    wheel4.wheelDirectionCS = irr::core::vector3df(0.0, -0.1, 0.0); // irr::core::vector3df(0.0, -1.0, 0.0);
    wheel4.wheelAxleCS = irr::core::vector3df(-0.2, 0.0, 0.0);
    wheel4.suspensionRestLength = 0.3;  // 0.30;
    wheel4.wheelRadius = 8;            // wheel (node) radius (15)
    wheel4.isFrontWheel = false;
    vehicle->addWheel(wheel4);          // Add left back wheel





    // Tweak wheel info a bit, tweak here more:
    // SRaycastInfo& info = vehicle->getWheelInfo(i);
    // info.contactNormalWS = irr::core::vector3df(-0.1, 0.0, 0.0);

    for(irr::u32 i=0; i<vehicle->getNumWheels(); i++){
    SWheelInfo& info = vehicle->getWheelInfo(i);
    info.suspensionStiffness     = 0.06f; // Vibração: tipo mola frente/trás (maior número => mais vibração)
    info.wheelDampingRelaxation  = 0.20f;
    info.wheelDampingCompression = 0.20f;
    info.frictionSlip  = 1000;
    info.rollInfluence = 0.1f; // Fluidez no rolar (colisao entre rigid bodies)
    // info.wheelSuspensionForce = 0.1f;
    // info.engineForce = 100;
    vehicle->updateWheelInfo(i);
    // vehicle->updateWheelTransform(i,true);
    };


    rigidBody->setVehicleReference(vehicle);

    // Set all wheels (nodes) in the right positions:
    rodaNode[1]->setPosition(wheel1.chassisConnectionPointCS);
    rodaNode[2]->setPosition(wheel2.chassisConnectionPointCS);
    rodaNode[3]->setPosition(wheel3.chassisConnectionPointCS);
    rodaNode[4]->setPosition(wheel4.chassisConnectionPointCS);









    // NODES -> TRAILER:
    irr::scene::IAnimatedMeshSceneNode* trailerNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("vehicle_truck_trailer.b3d")); // vehicle_traktor.b3d
    trailerNode->setPosition(vector3df(0,0,0));


    // Create impacts mesh shapes:
    // IGImpactMeshShape* vshape = new IGImpactMeshShape(emptyNode, trailerNode->getMesh(), 1000.0); // Mass
    IGImpactMeshShape* trshape = new IGImpactMeshShape(trailerNode, device->getSceneManager()->getMesh("vehicle_truck_trailer_raycast.b3d"), 500.0); // vehicle_traktor_raycast.b3d
    trshape->setMargin(smo);
    // trshape->calculateLocalInertia(800, 0);
    IRigidBody* rigidTBody = world->addRigidBody(trshape, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
    trshape->setLocalScaling(globalScaling, ESP_BOTH);
    rigidTBody->setLinearFactor(vector3df(0,1,1));  // Allowed positions axis (y,z)
    rigidTBody->setAngularFactor(vector3df(1,0,0)); // Allowed rotations axis (x)

    rigidTBody->getAttributes()->addBool("trailer", true);
    rigidTBody->includeNodeOnRemoval(false);
    rigidTBody->setFriction(20);  // rigidBody->setFriction(7.0f);
    rigidTBody->setRestitution(0); // void bouncy
    rigidTBody->setDamping(0.1,0.1);




    // NODES -> WHEELS:
    irr::scene::IAnimatedMeshSceneNode* rodaTNode[10];
    rodaTNode[1] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("wheel_big.b3d"));  // right_front_wheel
    rodaTNode[2] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("wheel_big.b3d"));  // left_front_wheel



    // IGImpactMeshShape* wTShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); // IConvexHullShape* wShape1 = new IConvexHullShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0); // IBoxShape* wShape1 = new IBoxShape(rodaNode1, 10.0, true);
    ISphereShape* wTShape1 = new ISphereShape(rodaTNode[1], wmass, true);
    IRigidBody *wtbody1 = world->addRigidBody(wTShape1, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
    wTShape1->setMargin(wsm);
    wtbody1->setFriction(wsf);
    wtbody1->setDamping(dap,dap);
    wtbody1->getAttributes()->addBool("wheel", true);
    // wbody1->setActivationState(EAS_DISABLE_DEACTIVATION);

    // IGImpactMeshShape* wTShape2 = new IGImpactMeshShape(rodaNode2, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); // IConvexHullShape* wShape2 = new IConvexHullShape(rodaNode2, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0);
    ISphereShape* wTShape2 = new ISphereShape(rodaTNode[2], wmass, true);
    IRigidBody *wtbody2 = world->addRigidBody(wTShape2, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
    wTShape2->setMargin(wsm);
    wtbody2->setFriction(wsf);
    wtbody2->setDamping(dap,dap);
    wtbody2->getAttributes()->addBool("wheel", true);
    // wbody2->setActivationState(EAS_DISABLE_DEACTIVATION);



    // FINISHING: Create vehicle2
    IRaycastVehicle* vehicle2 = world->addRaycastVehicle(rigidTBody); // Assuming world is pointer to your irrBullet World
    // world->addRaycastVehicle(wbody1); // Assuming world is pointer to your irrBullet World

    vehicle2->getVehicleRaycaster()->setUseFilter(true);
    vehicle2->getVehicleRaycaster()->setCollisionFilterGroup(COL_VEHICLE);
    vehicle2->getVehicleRaycaster()->setCollisionFilterMask(vehicleRaycastCollidesWith);






    // WHEELS "attached" to TRAILER, the secret is to use "constrains":
    btTransform frameInAA[10];
    btTransform frameInBB[10];

    frameInAA[0] = btTransform::getIdentity();
    frameInAA[0].setOrigin(irrlichtToBulletVector(trailerNode->getJointNode("05")->getPosition())); // position
    frameInBB[0] = btTransform::getIdentity();
    frameInBB[0].setOrigin(irrlichtToBulletVector(vector3df(0,0,0))); // rotation

    frameInAA[1] = btTransform::getIdentity();
    frameInAA[1].setOrigin(irrlichtToBulletVector(trailerNode->getJointNode("06")->getPosition())); // position
    frameInBB[1] = btTransform::getIdentity();
    frameInBB[1].setOrigin(irrlichtToBulletVector(vector3df(0,0,0))); // rotation

    btRigidBody* bodyAA = rigidTBody->getPointer(); // get chassi --->  parentObject->getRigidBody()->getPointer();

    btRigidBody* bodyBB[10];
    bodyBB[0] = wtbody1->getPointer(); // get wheel  --->  obj->getRigidBody()->getPointer();
    bodyBB[1] = wtbody2->getPointer(); // get wheel  --->  obj->getRigidBody()->getPointer();

    btGeneric6DofSpringConstraint* C11 = new btGeneric6DofSpringConstraint(*bodyAA, *bodyBB[0], frameInAA[0], frameInBB[0], true);
    btGeneric6DofSpringConstraint* C22 = new btGeneric6DofSpringConstraint(*bodyAA, *bodyBB[1], frameInAA[1], frameInBB[1], true);

    C11->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    C11->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit

    C22->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    C22->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit

    world->getPointer()->addConstraint(C11, true);
    world->getPointer()->addConstraint(C22, true);




    // Create wheel 1 - info:
    SWheelInfoConstructionInfo wheel11;
    wheel11.chassisConnectionPointCS = trailerNode->getJointNode("05")->getPosition();
    wheel11.wheelDirectionCS = irr::core::vector3df(0.0, -0.1, 0.0);
    wheel11.wheelAxleCS = irr::core::vector3df(-0.2, 0.0, 0.0);
    wheel11.suspensionRestLength = 0.3;  // 0.30;
    wheel11.wheelRadius = 8;            // wheel (node) radius (10)
    wheel11.isFrontWheel = false;
    vehicle2->addWheel(wheel11);          // Add left front wheel

    // Create wheel 2 - info:
    SWheelInfoConstructionInfo wheel22;
    wheel22.chassisConnectionPointCS = trailerNode->getJointNode("06")->getPosition();
    wheel22.wheelDirectionCS = irr::core::vector3df(0.0, -0.1, 0.0);
    wheel22.wheelAxleCS = irr::core::vector3df(0.2, 0.0, 0.0);
    wheel22.suspensionRestLength = 0.3;  // 0.30;
    wheel22.wheelRadius = 8;            // wheel (node) radius (10)
    wheel22.isFrontWheel = false;
    vehicle2->addWheel(wheel22);          // Add left front wheel






    // Tweak wheel info a bit, tweak here more:
    for(irr::u32 i=0; i<vehicle2->getNumWheels(); i++){
    SWheelInfo& info = vehicle2->getWheelInfo(i);
    info.suspensionStiffness     = 0.06f;
    info.wheelDampingRelaxation  = 0.20f;
    info.wheelDampingCompression = 0.20f;
    info.frictionSlip  = 1000;
    info.rollInfluence = 0.1f; // Fluidez no rolar (colisao entre rigid bodies)
    // info.wheelSuspensionForce = 0.1f;
    // info.engineForce = 100;
    vehicle2->updateWheelInfo(i);
    // vehicle2->updateWheelTransform(i,true);
    };


    rigidTBody->setVehicleReference(vehicle2);


    // Set all wheels (nodes) in the right positions:
    rodaTNode[1]->setPosition(wheel11.chassisConnectionPointCS);
    rodaTNode[2]->setPosition(wheel22.chassisConnectionPointCS);






    // TRAILER "attached" to VEHICLE, the secret is to use "constrains":
    btTransform frameInAAA[10];
    btTransform frameInBBB[10];

    frameInAAA[0] = btTransform::getIdentity();
    frameInAAA[0].setOrigin(irrlichtToBulletVector(vehicleNode->getJointNode("conn1_trailer")->getPosition()));  // VEHICLE
    frameInBBB[0] = btTransform::getIdentity();
    frameInBBB[0].setOrigin(irrlichtToBulletVector(trailerNode->getJointNode("conn2_trailer")->getPosition()));  // TRAILER
    // frameInBBB[0].setOrigin(irrlichtToBulletVector(trailerNode->getJointNode("conn2_trailer")->getPosition()));  // TRAILER

    btRigidBody* bodyAAA = rigidBody->getPointer();  // get chassi --->  parentObject->getRigidBody()->getPointer();
    btRigidBody* bodyBBB = rigidTBody->getPointer(); // get wheel  --->  obj->getRigidBody()->getPointer();

    btGeneric6DofSpringConstraint* C111 = new btGeneric6DofSpringConstraint(*bodyAAA, *bodyBBB, frameInAAA[0], frameInBBB[0], true);
    C111->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));       // linearUpperLimit
    C111->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,0,0)));       // linearLowerLimit
    C111->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0.5,0,0)));    // AngularUpperLimit ok: 0.5
    C111->setAngularLowerLimit(irrlichtToBulletVector(vector3df(-340,0,0)));   // AngularlowerLimit ok: -340

    world->getPointer()->addConstraint(C111, true);








    // Run all:
    int lastFPS = -1;
    while(device->run()){



            driver->beginScene(true, true, video::SColor(0,40,40,40));

            frameDeltaTime = device->getTimer()->getTime() - TimeStamp;
            TimeStamp = device->getTimer()->getTime();

            int fps = driver->getFPS();
            if (lastFPS != fps){
            device->setWindowCaption(L"TRUCK - By Jorge Rosa - jorge.bigarte@gmail.com");
            };




            // UPDATING PHYSICS:
            world->stepSimulation(frameDeltaTime*0.0017f, 10);


            // MISSING CODE HERE (irrlicht normal stuff .......)


            // reset FORCES:
            for(irr::u32 i=0; i<vehicle->getNumWheels(); i++){ vehicle->applyEngineForce(0,i); }; // force, wheelindex

            if(receiver.IsKeyDown(irr::KEY_KEY_A)){
                force = 10000;
                vehicle->applyEngineForce( force, 0);
                vehicle->applyEngineForce(-force, 1);
                vehicle->applyEngineForce( force, 2);
                vehicle->applyEngineForce(-force, 3);
            };

            if(receiver.IsKeyDown(irr::KEY_KEY_D)){
                force = 10000;
                vehicle->applyEngineForce(-force, 0);
                vehicle->applyEngineForce( force, 1);
                vehicle->applyEngineForce(-force, 2);
                vehicle->applyEngineForce( force, 3);
            };




            // WHEELS ROTATING: (Updates nodes according VELOCITY)
            // Rotate wheels (nodes) according vehicle´s (rigidbody) velocity:
            f32 velocity = vehicle->getRigidBody()->getLinearVelocity().Z;
            // ROTATING vehicle wheels:
            irr::core::matrix4 mat;
            mat.setRotationDegrees(vector3df(rotat+=(velocity * 0.12 * frameDeltaTime),0,0));
            rodaNode[1]->setRotation(mat.getRotationDegrees());   // Apply to nodes
            rodaNode[2]->setRotation(mat.getRotationDegrees());   // Apply to nodes
            rodaNode[3]->setRotation(mat.getRotationDegrees());   // Apply to nodes
            rodaNode[4]->setRotation(mat.getRotationDegrees());   // Apply to nodes


            f32 tvelocity = wtbody1->getLinearVelocity().Z;
            // ROTATING trailer wheels:
            irr::core::matrix4 mattr;
            mattr.setRotationDegrees(vector3df(trotat+=(tvelocity * 0.12 * frameDeltaTime),0,0));
            rodaTNode[1]->setRotation(mattr.getRotationDegrees());  // Apply to nodes
            rodaTNode[2]->setRotation(mattr.getRotationDegrees());  // Apply to nodes


            // WHEELS ROTATING: (Updates nodes according ENGINE FORCE)
            // Rotate wheels (nodes) according vehicle´s (rigidbody) velocity:
            f32 engine_force = wbody1->getTotalForce().Z  + wbody2->getTotalForce().Z  + wbody3->getTotalForce().Z  + wbody4->getTotalForce().Z;
            f32 totalTorque  = wbody1->getTotalTorque().Z + wbody2->getTotalTorque().Z + wbody3->getTotalTorque().Z + wbody4->getTotalTorque().Z;







            for(u32 i=0; i<world->getNumManifolds(); ++i){
            ICollisionCallbackInformation* info = world->getCollisionCallback(i);

            ICollisionObject* obj1 = info->getBody0();
            ICollisionObject* obj2 = info->getBody1();

            int numContacts = info->getPointer()->getNumContacts();
            SManifoldPoint point = info->getContactPoint(0);


            for(u32 i=0; i<numContacts; ++i){
            if(info->getContactPoint(0).getDistance()<0.1f && numContacts>0){

            // COLLISION:
            if(obj1->getAttributes()->existsAttribute("box") && obj2->getAttributes()->existsAttribute("road")){
            // Do stuff...
            };

            };

            // Clear stuff:
            info->getPointer()->clearManifold();  //<- is this the right way to clear & delete the info?
            delete info;
            };






            smgr->drawAll();
            guienv->drawAll();
            driver->endScene();
    };


    device->drop();
    return 0;
};
