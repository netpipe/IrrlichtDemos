#include "Vehicle.h"
#include "../TerrainFactory/Terrain.h"
#include <string.h>
#include "../Scene/Elevator.hpp"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


Vehicle::Vehicle()  :    m_carChassis(0),m_cameraHeight(4.f),m_minCameraDistance(3.f),
        m_maxCameraDistance(10.f),m_indexVertexArrays(0),m_vertices(0)
{
    int scaleT  = 2; // minimum 2 Vehicle Size test scale
    int scaleE  = 1;// engine/weight scale for friction
    printf("Vehicle constructor.\n");
    rightIndex          = 0;
    upIndex             = 1;
    forwardIndex        = 2;
    wheelDirectionCS0   = btVector3(0,-1,0);
    wheelAxleCS         = btVector3(-1,0,0);        // makes the car go forward vector
    btModelscale        = btVector3( 1.5f*scaleT, 1.5f*scaleT, 2.f*scaleT );// sides,y,FB     Model Specific
    btCarScale          = btVector3( 1.3f*scaleT, .4f*scaleT, 2*scaleT );//COMeffected -  Collision Box   default 1,2,2
    m_vehiclePosition   = btVector3(0,0,0);         // sets the position initially


    vehicleWeight       = 100           *scaleE*scaleT;      //weight of a typical vehicle 1 vehicle weight
    maxBreakingForce    = 100.0f        *scaleE*scaleT;
    maxEngineForce      = 2600.0f       *scaleE*scaleT;  //this should be engine/velocity dependent
    SpeedINC            = 1             *scaleE*scaleT; // Speed Increasement scale factor  TEST FACTOR ONLY -UNIT
    driveType           = 1;               //  0=2RWD  3=2FWD  1=4wd  2=4wd+4wsteer

    connectionHeight    = 0.03f         *scaleT;     // surface -> connection to chassy height changes COM

    suspensionStiffness = 20.0f   *scaleE*scaleT; // bigger the number the stiffer it getS
    //! there is a fucked up ratio between the suspension dampners
    suspensionDamping   = .5f        ;// bounce time       <1  bigger means less bounces
    suspensionCompression= .7f       ;// shock absorber    <1  bigger the number the more is absorbed
    suspensionRestLength= btScalar(0.6f *scaleT);   // combined height of .6 to clear the ground
    rollInfluence       = 0.001f;
    wheelFriction       = 1e30f;  //1e30f;
    wheelRadius         = 0.8f          *scaleT;
    wheelWidth          = 0.60f          *scaleT;
    steeringIncrement   = 0.01f;
    steeringClamp       = 0.45f;        // .90 max
    //Wheel SpreadScale
    CUBE_HALF_EXTENTS   = 2            *scaleT;
    wheelSpacingX       = 0.3f;       //subtracted  wheel spacing width   .3 default
    wheelSpacingZ       = 2.f;       // back wheel from center of car     2 default
    wheelSpacingZ2      = 2.f;      // Front wheels z-shift               2 default

    wheelScaleFactor    = 1.f;       //wheel model specific

    // Dont Touch \/ these
    m_numWheels         = 4;
    m_vehicle           = 0;
    maxProxies          = 32766;
    maxOverlap          = 65535;
    state               = EVEHICLE_STALLED;
    m_vehicle           = NULL;
    gEngineForce        = 0.0f;
    gBreakingForce      = 0.0f;
    gVehicleSteering    = 0.f;

        tr.setIdentity();

}


void Vehicle::loadLevel(const stringc file)
{
  if(!m_irrDevice) return;
    int tscale=20 ;
    vector3df trackScale = vector3df(tscale,tscale,tscale); //50
    vector3df trackPosition = vector3df(0,0.0f,0);

    IAnimatedMesh *mesh = m_irrDevice->getSceneManager()->getMesh(file.c_str());
    m_irrDevice->getSceneManager()->getMeshManipulator()->scaleMesh(mesh,trackScale);
    IAnimatedMeshSceneNode *node = m_irrDevice->getSceneManager()->addAnimatedMeshSceneNode(mesh);

    if(!mesh || !node) return;
    node->setAutomaticCulling(EAC_OFF);
    // node->setMaterialFlag(EMF_LIGHTING, true);
    // node->setScale(trackScale);
    // node->setRotation(vector3df(45,90.f,110));
    node->setPosition(trackPosition);

    tr.setOrigin(btVector3(trackPosition.X, trackPosition.Y, trackPosition.Z));
    btTriangleMesh *collisionMesh = new btTriangleMesh();

  //  m_cScene->setGenericMaterial(node, 0);

    int meshCount = mesh->getMeshBufferCount();
        printf("MESHBUFFER COUNT %d /n",meshCount);

        for (int i=0; i < meshCount ; i++)//!load all meshes for CD
        {
            //  meshBuffer2->append( mesh->getMeshBuffer(i) );
          //  m_cScene->setGenericMaterial(node, i); //outdoor sun lumenation
            m_cPhysics->convertIrrMeshBufferBtTriangleMesh(mesh->getMeshBuffer(i), collisionMesh, vector3df(1,1,1));
            //decalManager->addMesh(mesh->getMeshBuffer(i));
        }

    btBvhTriangleMeshShape *trackShape = new btBvhTriangleMeshShape(collisionMesh, true);
    localCreateRigidBody(0, tr, trackShape, node);


//#define IRRCD
#ifdef IRRCD
    metaSelector = m_irrDevice->getSceneManager()->createMetaTriangleSelector();
    selector = m_irrDevice->getSceneManager()->createOctTreeTriangleSelector(mesh,node,128);
    node->setTriangleSelector(selector);
    metaSelector->addTriangleSelector(selector);
    selector->drop();

    //meshbuffer converter to IMesh
    // scene::SMesh* mesh2 = new SMesh; // dont really need this
    // mesh2->addMeshBuffer (meshBuffer2);
    // mesh2->drop();

    scene::ISceneNodeAnimator* anim;
	anim = m_irrDevice->getSceneManager()->createCollisionResponseAnimator(
		metaSelector, camera, core::vector3df(30,60,30),
		core::vector3df(0,0,0),   /// MAIN irrGRAVITY
		core::vector3df(0,10,0));
    camera->addAnimator(anim);
    anim->drop();
	metaSelector->drop();
#endif
}

void Vehicle::createTerrain(vector3df pos,char *heightmap)//buggy as heck
{
    if (!m_irrDevice) return;

    // Create Irrlicht Terrain.
    int smoothFactor = 1;
    vector3df t_position = pos;
    vector3df t_rotation = vector3df(0,0,0);
    vector3df t_scale = vector3df(4,1,4);

    tr.setIdentity();
    tr.setOrigin(btVector3(pos.X, pos.Y, pos.Z));

    ITerrainSceneNode *terrainNode = m_irrDevice->getSceneManager()->addTerrainSceneNode(
                                         heightmap, 0, -1,
                                         t_position, t_rotation, t_scale,
                                         SColor(255,255,255,255), 5, ETPS_17, smoothFactor);

    terrainNode->scaleTexture(1,10);
    terrainNode->setMaterialTexture(0, m_irrDevice->getVideoDriver()->getTexture("data/textures/cells/ctown.png"));
    terrainNode->setMaterialTexture(1, m_irrDevice->getVideoDriver()->getTexture("data/textures/detailmaps/rock.png"));
    terrainNode->setMaterialType(EMT_DETAIL_MAP);
    terrainNode->setMaterialFlag(EMF_FOG_ENABLE, false);
    //terrainNode->setMaterialFlag(EMF_LIGHTING, true);
    //terrainNode->setRotation (vector3df(0, 1, 0));
    #ifdef DPHYSICS
#define Method1
#ifdef Method1 ///seems slower less controllable11
    irr::scene::CDynamicMeshBuffer* meshBuffer = 0;
    meshBuffer = new irr::scene::CDynamicMeshBuffer(irr::video::EVT_2TCOORDS, irr::video::EIT_16BIT);
    terrainNode->getMeshBufferForLOD(*meshBuffer, 0);
    btTriangleMesh *collisionMesh = new btTriangleMesh();
    m_cPhysics->convertIrrMeshBufferBtTriangleMesh(meshBuffer, collisionMesh, t_scale);
    btBvhTriangleMeshShape *groundShape = new btBvhTriangleMeshShape(collisionMesh, true);

    localCreateRigidBody(0,tr,groundShape,terrainNode);
    meshBuffer->drop();
#endif
#ifdef Method2
    scene::ITriangleSelector*
    selector = m_irrDevice->getSceneManager()->createTerrainTriangleSelector(terrainNode, 0);
	terrainNode->setTriangleSelector(selector);
    metaSelector = m_irrDevice->getSceneManager()->createMetaTriangleSelector();
    irr::scene::CDynamicMeshBuffer* meshBuffer = 0;
    meshBuffer = new irr::scene::CDynamicMeshBuffer(irr::video::EVT_2TCOORDS, irr::video::EIT_16BIT);
    terrainNode->getMeshBufferForLOD(*meshBuffer, 1);
    video::S3DVertex2TCoords* mb_vertices = (video::S3DVertex2TCoords*)meshBuffer->getVertexBuffer().getData();
    const irr::u32 vertexCount = meshBuffer->getVertexCount();
    const irr::u32 indexCount = meshBuffer->getIndexCount();
    u16* mb_indices = meshBuffer->getIndices();
    btTriangleMesh *collisionMesh = new btTriangleMesh();

    btVector3 vertices[3];
    s32 j,k;

    for(j=0;j<indexCount;j+=3)
    {
      for (k=0;k<3;k++)
      {
            s32 index = mb_indices[j+k];
            vertices[k] = btVector3(
                mb_vertices[index].Pos.X*terrainNode->getScale().X,
                mb_vertices[index].Pos.Y*terrainNode->getScale().Y,
                mb_vertices[index].Pos.Z*terrainNode->getScale().Z);
        }
        collisionMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
    }

    m_cPhysics->convertIrrMeshBufferBtTriangleMesh(meshBuffer, collisionMesh, t_scale);//(&data, collisionMesh, t_scale);
    btBvhTriangleMeshShape *groundShape = new btBvhTriangleMeshShape(collisionMesh, true);
    localCreateRigidBody(0,tr,groundShape,terrainNode);
	meshBuffer->drop();
#endif
#endif
    //for irrlicht CD
    #ifdef IRRCD
    recursiveFillMetaSelector( terrainNode, metaSelector );
  //  recursiveFillMetaSelector( m_irrDevice->getSceneManager()->getRootSceneNode() , metaSelector );

	scene::ISceneNodeAnimator* anim;
    //FOR the whole scene
//	anim = m_irrDevice->getSceneManager()->createCollisionResponseAnimator(
//		metaSelector, camera, core::vector3df(60,50,60),
//		core::vector3df(0,0,0),
//		core::vector3df(0,50,0));
//	metaSelector->drop();
//	camera->addAnimator(anim);
	//FOR the just this mesh
//	#ifdef MESHONLY
	anim = m_irrDevice->getSceneManager()->createCollisionResponseAnimator(
		selector, camera, core::vector3df(60,50,60),
		core::vector3df(0,0,0),
		core::vector3df(0,50,0));
    camera->addAnimator(anim);
 //   #endif
    selector->drop();
	anim->drop();
	#endif
}

void Vehicle::renderme()
{
    if(!m_vehicle) return;

    btScalar m[16];
    int i;
            //btCylinderShapeX wheelShape(btVector3(wheelWidth,wheelRadius,wheelRadius));
            //btVector3 wheelColor(1,0,0);
    //! Wheel tranform variables.
    btTransform wheelTransform;
    btVector3 wheelPosition;
    btQuaternion wheelRotation;
    btVector3 wheelRotationEuler;
    f32 wheelOffsetX = 0,
        wheelOffsetY = 0,
        wheelOffsetZ = 0;

// Draw wheels (cylinders)
    for (i=0;i<m_numWheels;i++)
    {

        //wheelTransform = m_vehicle->getWheelTransformWS(i);
        wheelTransform = m_vehicle->getWheelInfo(i).m_worldTransform;
        wheelPosition = wheelTransform.getOrigin();
        wheelRotation = wheelTransform.getRotation();
        m_cPhysics->quaternionToEuler(wheelRotation, wheelRotationEuler);
        wheelNode[i]->setPosition(vector3df(    (f32)wheelPosition[0] ,
                                                (f32)wheelPosition[1] ,
                                                (f32)wheelPosition[2]));
        wheelNode[i]->setRotation(vector3df(    (f32)wheelRotationEuler[0],
                                                (f32)wheelRotationEuler[1],
                                                (f32)wheelRotationEuler[2]));
                                                        // Synchronize the wheels with the (interpolated) chassis worldtransform
        m_vehicle->updateWheelTransform(i,true);
    }
    updateVehicleMovement();
}


btVector3 Vehicle::getVehiclePosition(void)
{
  if(!m_vehicle)
    return m_vehiclePosition;
  else
    return m_vehicle->getRigidBody()->getCenterOfMassPosition();
}

void Vehicle::setVehiclePosition( core::vector3df vect )
{
   // if(!m_vehicle)
   btVector3 pos = btVector3(vect.X,vect.Y,vect.Z);
    m_carChassis->setCenterOfMassTransform(tr);
   m_carChassis->translate(btVector3 (vect.X,vect.Y,vect.Z));
  // m_carChassis->s
   // m_vehicle->set;
}



btQuaternion Vehicle::getVehicleOrientation(void)
{
    return m_vehicle->getRigidBody()->getOrientation();
}




void Vehicle::setState(EVEHICLE_STATE _state)           {    state = _state;}

EVEHICLE_STATE Vehicle::getState(void)                  {    return state;}

void Vehicle::registerIrrDevice(IrrlichtDevice &device) {    m_irrDevice = &device;}

void Vehicle::registerPhysics(Physics &physics)         {    m_cPhysics = &physics;}

void Vehicle::registerScene(Scene &scene)               {   m_cScene = &scene;  }




void Vehicle::accelerate(f32 speed)
{
    setState(EVEHICLE_ACCELERATING);
    gEngineForce += speed*SpeedINC;
    if (gEngineForce > maxEngineForce) gEngineForce = maxEngineForce;

    if (gEngineForce < 0)
    {
        gEngineForce = 0;
        setState(EVEHICLE_STALLED);
    }
    gBreakingForce = 0.f;
}

void Vehicle::reverse(f32 speed)
{
    setState(EVEHICLE_REVERSING);
    gEngineForce -= speed*SpeedINC;

    if (gEngineForce < -maxEngineForce)
        gEngineForce = -maxEngineForce;

    if (gEngineForce > 0)
    {
        gEngineForce = 0;
        setState(EVEHICLE_STALLED);
    }
    gBreakingForce = 0.f;
}

void Vehicle::steer_right(void)
{
        if (	(gVehicleSteering+steeringIncrement) < steeringClamp)
    gVehicleSteering += steeringIncrement;
}

void Vehicle::steer_left(void)
{
    if (	(gVehicleSteering-steeringIncrement) > -steeringClamp)
    gVehicleSteering -= steeringIncrement;
}

void Vehicle::brake(void)
{
    gBreakingForce = maxBreakingForce;
    gEngineForce = 0.1f;
}


void Vehicle::registerCamera( ICameraSceneNode *camera2){
  camera= camera2;
}
void Vehicle::steer_reset(void)
{
    gVehicleSteering *= 0.80f;
 //   gVehicleSteering = 0;
}

void Vehicle::resetVehicle(void)
{
  gVehicleSteering = 0.0f;
  m_carChassis->setCenterOfMassTransform(btTransform::getIdentity());
	m_carChassis->setLinearVelocity(btVector3(0,0,0));
	m_carChassis->setAngularVelocity(btVector3(0,0,0));
	//m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_carChassis->getBroadphaseHandle(),getDynamicsWorld()->getDispatcher());
	if (m_vehicle)
	{
		m_vehicle->resetSuspension();
		for (int i=0;i<m_vehicle->getNumWheels();i++)
		{
			//synchronize the wheels with the (interpolated) chassis worldtransform
			m_vehicle->updateWheelTransform(i,true);
		}
	}
}

Vehicle::~Vehicle()
{
  if(!m_vehicle) return;
    for (int j=0;j<m_collisionShapes.size();j++)
    {
        btCollisionShape* shape = m_collisionShapes[j];
        delete shape;
    }
    delete m_indexVertexArrays;
    delete m_vertices;
    delete m_vehicleRayCaster;
    delete m_vehicle;
}

void Vehicle::initPhysics()
{
    printf("Init vehicle physics.\n");
    btVector3 worldMin(-1000,-1000,-1000);
    btVector3 worldMax(1000,1000,1000);

    m_overlappingPairCache = new btAxisSweep3(worldMin,worldMax);

    IAnimatedMesh *l_node;
    IAnimatedMeshSceneNode *l_node_chassi;

    const stringc carMesh = "data/models/vehicles/body.irrmesh";
 //                 carMesh = "data/models/vehicles/C.obj";

    l_node = m_irrDevice->getSceneManager()->getMesh(carMesh.c_str());
    if(!l_node) return;

    l_node_chassi = m_irrDevice->getSceneManager()->addAnimatedMeshSceneNode(l_node);
    l_node_chassi->setScale(vector3df(btModelscale[0], btModelscale[1], btModelscale[2]));
   // l_node_chassi->setPosition(vector3df(805,520,830));
    l_node_chassi->setMaterialTexture(0, m_irrDevice->getVideoDriver()->getTexture("data/body.tga"));
    m_cScene->setGenericMaterial(l_node_chassi, 0);

    if (!l_node_chassi) printf("Chassi node was not created.\n");

    btCollisionShape* chassisShape = new btBoxShape(btCarScale);  //!   << BULLET BODY SCALE
    btCompoundShape* compound = new btCompoundShape();

    btTransform localTrans;
    localTrans.setIdentity();
    localTrans.setOrigin(btVector3(0,0,0)); // moves chassi around and center of mass
        tr.setOrigin(m_vehiclePosition);
        //tr.setRotation(btQuaternion(0,0,0));
        //l_node_chassi->setRotation(vector3df(0,0,180));

    compound->addChildShape(localTrans,chassisShape);
    m_carChassis = localCreateRigidBody(vehicleWeight,tr,compound, l_node_chassi);//chassisShape);

    m_vehicleRayCaster = new btDefaultVehicleRaycaster(m_cPhysics->getDynamicsWorld());
    m_vehicle = new btRaycastVehicle(m_tuning,m_carChassis,m_vehicleRayCaster);

    m_carChassis->setDamping(suspensionDamping,suspensionDamping);
    m_carChassis->setActivationState(DISABLE_DEACTIVATION);         // never deactivate the vehicle

    m_cPhysics->getDynamicsWorld()->addVehicle(m_vehicle);
    m_vehicle->setCoordinateSystem(rightIndex,upIndex,forwardIndex);//choose coordinate system


    // Create Vehicle Wheels
    bool isFrontWheel=true;
    btVector3 connectionPointCS0(CUBE_HALF_EXTENTS-(wheelSpacingX*wheelWidth),connectionHeight,wheelSpacingZ2*CUBE_HALF_EXTENTS-wheelRadius);
    m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

    connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(wheelSpacingX*wheelWidth),connectionHeight,wheelSpacingZ2*CUBE_HALF_EXTENTS-wheelRadius);
    m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

    isFrontWheel = false;
    connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(wheelSpacingX*wheelWidth),connectionHeight,-wheelSpacingZ*CUBE_HALF_EXTENTS+wheelRadius);
    m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

    connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS-(wheelSpacingX*wheelWidth),connectionHeight,-wheelSpacingZ*CUBE_HALF_EXTENTS+wheelRadius);
    m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

    //Set Wheel Property's
    for (int i = 0; i < m_vehicle->getNumWheels(); i++)
    {
        btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
        wheel.m_suspensionStiffness = suspensionStiffness;
        wheel.m_wheelsDampingRelaxation = suspensionDamping;
        wheel.m_wheelsDampingCompression = suspensionCompression;
        wheel.m_suspensionRestLength1 = suspensionRestLength;
        wheel.m_wheelsRadius = wheelRadius;
        wheel.m_frictionSlip = wheelFriction;
        wheel.m_rollInfluence = rollInfluence;
    }

    printf("Adding wheels.\n");
    IAnimatedMeshSceneNode *tempNode;
    for (int i = 0; i < m_vehicle->getNumWheels(); i++)
    {
        tempNode = m_irrDevice->getSceneManager()->addAnimatedMeshSceneNode( m_irrDevice->getSceneManager()->getMesh("data/models/vehicles/wheel.3ds") );
   //     tempNode->setMaterialTexture(0, m_irrDevice->getVideoDriver()->getTexture("data/models/vehicles/buggy/TIRE_TEX.TGA"));
        tempNode->setScale(vector3df(   (wheelWidth*   wheelScaleFactor),
                                        (wheelRadius*  wheelScaleFactor),
                                        (wheelRadius*  wheelScaleFactor)));
        m_cScene->setGenericMaterial(tempNode, 0);
        tempNode->getMaterial(0).Shininess = 0.0f;
        wheelNode.push_back(tempNode);
    }

    m_numWheels = m_vehicle->getNumWheels();
}

void Vehicle::updateVehicleMovement(void)
{

    gBreakingForce=0;
        int wheelIndex = 0; //!RWD
                m_vehicle->setSteeringValue(gVehicleSteering,wheelIndex);
                m_vehicle->setBrake(gBreakingForce,wheelIndex);
        wheelIndex = 1;
                m_vehicle->setSteeringValue(gVehicleSteering,wheelIndex);
                m_vehicle->setBrake(gBreakingForce,wheelIndex);
        wheelIndex = 2;
                m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
                m_vehicle->setBrake(gBreakingForce,wheelIndex);
        wheelIndex = 3;
                m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
                m_vehicle->setBrake(gBreakingForce,wheelIndex);

    if (driveType==1) { //! 4x4 Wheel Drive
            wheelIndex = 0;
                m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
            wheelIndex = 1;
                m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
        }
    else if (driveType==2) { //! 4 wheel steering (gta monstertruck style)
            wheelIndex = 0;
                m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
            wheelIndex = 1;
                m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
            wheelIndex = 2;
                m_vehicle->setSteeringValue(-gVehicleSteering,wheelIndex);
            wheelIndex = 3;
                m_vehicle->setSteeringValue(-gVehicleSteering,wheelIndex);
        }
    else if (driveType==3) {
            int wheelIndex = 0; //!FWD
                m_vehicle->setSteeringValue(gVehicleSteering,wheelIndex);
                m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
            wheelIndex = 1;
                m_vehicle->setSteeringValue(gVehicleSteering,wheelIndex);
                m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
            wheelIndex = 2;
            wheelIndex = 3;
        }
}

btRigidBody*	Vehicle::localCreateRigidBody(float mass, const btTransform& startTransform,btCollisionShape* shape, ISceneNode *node)
{
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    btRigidBody* body = NULL;

    if (isDynamic)
    {
        shape->calculateLocalInertia(mass,localInertia);
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        body = new btRigidBody(mass, myMotionState, shape, localInertia);
    }
    else
    {
        body = new btRigidBody(mass,0,shape,localInertia);
    }

    body->setWorldTransform(startTransform);
    body->setUserPointer((void *)(node));

    m_cPhysics->getDynamicsWorld()->addRigidBody(body);
    m_cPhysics->push_back(body);
    return body;
}

void Vehicle::recursiveFillMetaSelector(scene::ISceneNode* node, scene::IMetaTriangleSelector* meta )
{

  //
  // the following if is basically the same as ISceneNode_assignTriangleSelector
  //
  printf ("Node name is: %s \n",node->getName());
  printf ("Node id is: %d \n",node->getID());
  printf ("Node type:");
  //  printf ("Node type: %s=",smgr->getSceneNodeTypeName());
  if (node->getType() ==   ESNT_UNKNOWN) printf("Unknown mesh type \n\n");
  if (node->getType() ==   ESNT_MESH)
  {
      io::IAttributes* attribs = m_irrDevice->getFileSystem()->createEmptyAttributes();
      if (attribs)
        {// get the mesh name out
         node->serializeAttributes(attribs);
         core::stringc name = attribs->getAttributeAsString("Mesh");
         attribs->drop();
         // get the animated mesh for the object
         scene::IAnimatedMesh* mesh = m_irrDevice->getSceneManager()->getMesh(name.c_str());
         if (mesh)
         {
            printf("\tAdding triangle selector for mesh\n");
            scene::ITriangleSelector* selector =
            m_irrDevice->getSceneManager()->createTriangleSelector(mesh->getMesh(0), node);
            node->setTriangleSelector(selector);
            metaSelector->addTriangleSelector(selector);
            selector->drop();
/*
            node->setMaterialFlag(video::EMF_LIGHTING, true);
            node->getMaterial(0).Shininess = 100.0f;
            node->getMaterial(0).DiffuseColor = SColor(255,255,255,255);
            node->getMaterial(0).EmissiveColor = SColor(255,255,255,255);
            node->getMaterial(0).AmbientColor = SColor(255,255,255,255);
            node->getMaterial(0).SpecularColor = SColor(255,255,255,255);
            node->getMaterial(0).MaterialTypeParam = 0.01;
*/
            //node->getMaterial(0).MaterialType = EMT_ONETEXTURE_BLEND;
            //node->setFlag(EMF_TRILINEAR_FILTER, true);
         }
     }
  }
         if(strcmp(node->getName(), "elevator") == 0)
  {
  	node->setMaterialType(EMT_LIGHTMAP_M4);
  	node->setMaterialFlag(EMF_LIGHTING, false);
    Elevator::Instance()->Add(node);
    Elevator::Instance()->elevatorStart = node->getAbsolutePosition();
    printf("elevator added");
    //    Elevator::Instance()->Clear();
  }
  if(strcmp(node->getName(), "obstacle") == 0)
  {
	//	Obstacle::Instance()->Add(node);
  }

  if (node->getType() ==   ESNT_ANIMATED_MESH) printf("Animated Mesh! \n\n");
  if (node->getType() ==   ESNT_SKY_BOX) printf("SkyBox! \n\n");
  if (node->getType() ==   ESNT_CAMERA_FPS) printf("Fps Camera! \n\n");
  if (node->getType() ==   ESNT_CAMERA_MAYA ) printf("Maya Camera! \n\n");
  if (node->getType() ==   ESNT_CAMERA )
  { printf("STD Camera! \n");
   printf ("The current position of this camera is: %f,%f,%f\n\n",node->getPosition().X,node->getPosition().Y,node->getPosition().Z);
   camera->setPosition(node->getPosition());
  }
  if (node->getType() ==   ESNT_PARTICLE_SYSTEM ) printf("Particles! \n\n");
  if (node->getType() ==   ESNT_LIGHT  ) printf("Light! \n\n");
  if (node->getType() ==   ESNT_OCTREE)
  {
      // Occ Trees are for land
      printf("Occtree! \n");
      io::IAttributes* attribs = m_irrDevice->getFileSystem()->createEmptyAttributes();
      if (attribs)
        {// get the mesh name out
         node->serializeAttributes(attribs);
         core::stringc name = attribs->getAttributeAsString("Mesh");
         attribs->drop();
         // get the animated mesh for the object
         scene::IAnimatedMesh* mesh = m_irrDevice->getSceneManager()->getMesh(name.c_str());
         if (mesh)
         {
            scene::ITriangleSelector* selector =
            m_irrDevice->getSceneManager()->createOctTreeTriangleSelector(mesh->getMesh(0), node, 128);
            node->setTriangleSelector(selector);
            metaSelector->addTriangleSelector(selector);
            selector->drop();
         }

     }

  }
  // now recurse on children...
  core::list<scene::ISceneNode*>::ConstIterator begin = node->getChildren().begin();
  core::list<scene::ISceneNode*>::ConstIterator end   = node->getChildren().end();

  for (; begin != end; ++begin)
    recursiveFillMetaSelector(*begin, meta);
}
