#include <irrlicht.h>
#include <bulletworld.h>
#include <rigidbody.h>
#include <boxshape.h>
#include <sphereshape.h>
#include <gimpactmeshshape.h>
#include <raycastvehicle.h>
#include <collisionobjectaffectordelete.h>
#include <collisionobjectaffectorattract.h>
//#include <collisionobjectaffectorbuoyancy.h>
#include "exampleframework.h"


CExampleFramework::CExampleFramework()
{
}

IRigidBody* const CExampleFramework::addCube(const irr::core::vector3df &pos, const irr::core::vector3df &scale, irr::f32 mass, irr::core::stringc textureFile) const
{
    irr::scene::ISceneNode *Node = device->getSceneManager()->addCubeSceneNode(1.0f);
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture(textureFile.c_str()));

    Node->setMaterialFlag(irr::video::EMF_WIREFRAME, drawWireFrame);

	ICollisionShape *shape = new IBoxShape(Node, mass, false);

	//shape->setMargin(0.01);

	IRigidBody *body;
	body = world->addRigidBody(shape);

	return body;
}

IRigidBody* const CExampleFramework::shootCube(const irr::core::vector3df &scale, irr::f32 mass, irr::core::stringc textureFile) const
{
	irr::core::vector3df pos(camera->getPosition().X,camera->getPosition().Y,camera->getPosition().Z);

	irr::scene::ISceneNode *Node = device->getSceneManager()->addCubeSceneNode(1.0f);
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture(textureFile.c_str()));

    Node->setMaterialFlag(irr::video::EMF_WIREFRAME, drawWireFrame);

	ICollisionShape *shape = new IBoxShape(Node, mass, true);

	//shape->setMargin(0.01);

	IRigidBody *body = world->addRigidBody(shape);

	irr::core::vector3df rot = camera->getRotation();
    irr::core::matrix4 mat;
    mat.setRotationDegrees(rot);
    irr::core::vector3df forwardDir(irr::core::vector3df(mat[8],mat[9],mat[10]) * 120);

    body->setLinearVelocity(forwardDir);
    //body->setActivationState(EAS_DISABLE_DEACTIVATION);

    body->setDamping(0.2,0.2);
    body->setFriction(0.4f);

    //ICollisionObjectAffectorDelete *deleteAffector = new ICollisionObjectAffectorDelete(4000);
    //body->addAffector(deleteAffector);

    body->getAttributes()->addBool("collide", true);

    //ICollisionObjectAffectorAttract* affector = new ICollisionObjectAffectorAttract(irr::core::vector3df(0,1000,0), 2500);
    //ICollisionObjectAffectorAttract* affector = new ICollisionObjectAffectorAttract(device->getSceneManager()->getActiveCamera(), 2500);
    //body->addAffector(affector);

    return body;
}

IRigidBody* const CExampleFramework::shootSphere(const irr::core::vector3df &scale, irr::f32 mass) const
{
	irr::core::vector3df pos(camera->getPosition().X,camera->getPosition().Y,camera->getPosition().Z);

	irr::scene::ISceneNode *Node = device->getSceneManager()->addSphereSceneNode();
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture("rockwall.jpg"));

	if(drawWireFrame)
        Node->setMaterialFlag(irr::video::EMF_WIREFRAME, drawWireFrame);

	ICollisionShape *shape = new ISphereShape(Node, mass, true);

	//shape->setMargin(0.01);

	IRigidBody *body = world->addRigidBody(shape);
	body->setDamping(0.2,0.2);

    // Since we use a "bumpy" triangle mesh for the terrain, we need to set the CCD radius and
    // motion threshold to ensure that we don't have any tunneling problems.
    // This doesn't work very well for box shapes.
	// ALSO: If you use setLocalScaling() on the collision shape, be sure to call this again
	// after scaling to make sure that it matches the actual object.

	irr::core::vector3df rot = camera->getRotation();
    irr::core::matrix4 mat;
    mat.setRotationDegrees(rot);
    irr::core::vector3df forwardDir(irr::core::vector3df(mat[8],mat[9],mat[10]) * 120);

    body->setLinearVelocity(forwardDir);



    //ICollisionObjectAffectorDelete *deleteAffector = new ICollisionObjectAffectorDelete(4000);
    //body->addAffector(deleteAffector);

    return body;
}

IRigidBody* const CExampleFramework::createTank(const irr::core::stringw& file, const irr::core::stringw& collFile,
    const irr::core::vector3df &pos, const irr::f32 mass) const
{
    irr::scene::IAnimatedMeshSceneNode *Node = device->getSceneManager()->addAnimatedMeshSceneNode(
        device->getSceneManager()->getMesh(file.c_str()));
	Node->setPosition(pos);
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);


	IGImpactMeshShape *shape = new IGImpactMeshShape(Node, device->getSceneManager()->getMesh(collFile.c_str()), mass);
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
	IRaycastVehicle* vehicle = world->addRaycastVehicle(tank);


    // Set up our wheel construction info. These values can be changed for each wheel,
    // and the values that you want to keep will stay the same, that way
    // all parameters for each wheel can stay the same for what needs to remain equal,
    // such as directions and suspension rest length.
    SWheelInfoConstructionInfo wheel;
    wheel.chassisConnectionPointCS = irr::core::vector3df(0.0,-0.88,4.0);
    wheel.wheelDirectionCS = irr::core::vector3df(0.0,-1,0.0);
    wheel.wheelAxleCS = irr::core::vector3df(-2,0.0,0.0);
    wheel.suspensionRestLength = 0.6;
    wheel.wheelRadius = 8.0;
    wheel.isFrontWheel = false;

    wheel.chassisConnectionPointCS = irr::core::vector3df(-4, -0.6, 12);
    vehicle->addWheel(wheel);

    wheel.chassisConnectionPointCS = irr::core::vector3df(4, -0.6, 12);
    vehicle->addWheel(wheel);

    wheel.chassisConnectionPointCS = irr::core::vector3df(-4, -0.6, -12);
    vehicle->addWheel(wheel);

    wheel.chassisConnectionPointCS = irr::core::vector3df(4, -0.6, -12);
    vehicle->addWheel(wheel);

	for(irr::u32 i=0;i<vehicle->getNumWheels();i++)
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
    }

    // Set a reference to the raycast vehicle so that irrBullet will automatically remove it when the object is removed
    tank->setVehicleReference(vehicle);

    return tank;
}

irr::scene::IParticleSystemSceneNode* const CExampleFramework::createParticleSystem(const irr::core::vector3df& dir, irr::f32 min, irr::f32 max,
    const irr::video::SColor& darkest, const irr::video::SColor& brightest, irr::f32 minAge, irr::f32 maxAge, irr::f32 angle,
    const irr::core::dimension2df& minSize, const irr::core::dimension2df maxSize, const irr::core::stringc& textureFile,
    irr::f32 lifeTime, bool gravity, irr::video::E_MATERIAL_TYPE materialType, bool fadeOut) const
{
    irr::scene::IParticleSystemSceneNode* ps =
    device->getSceneManager()->addParticleSystemSceneNode(false);

    irr::scene::IParticleEmitter* em = ps->createBoxEmitter(
        irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // emitter size
        dir,   // initial direction core::vector3df(0.0f,0.01f,0.0f)
        min,max,                             // emit rate
        darkest,       // darkest color
        brightest,       // brightest color
        minAge,maxAge,angle,                         // min and max age, angle
        minSize,         // min size
        maxSize);        // max size

    ps->setEmitter(em); // this grabs the emitter
    em->drop(); // so we can drop it here without deleting it

    if(fadeOut)
    {
        irr::scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
        ps->addAffector(paf); // same goes for the affector
        paf->drop();
    }

    if(gravity==true)
    {
        irr::scene::IParticleAffector* paf = ps->createGravityAffector(irr::core::vector3df(0.0f,-0.007f, 0.0f), 1800);
        ps->addAffector(paf); // same goes for the affector
        paf->drop();
    }

    ps->setScale(irr::core::vector3df(0.4,0.4,0.4));
    ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    ps->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, device->getVideoDriver()->getTexture(textureFile.c_str()));
    ps->setMaterialType(materialType);

    //ps->setParent(info->getBody0()->getCollisionShape()->getSceneNode());
    //ps->setPosition(vector3df(0,2,0));

    if(lifeTime > 0.0f)
    {
        irr::scene::ISceneNodeAnimator* anim = device->getSceneManager()->createDeleteAnimator(lifeTime);
        ps->addAnimator(anim);
        anim->drop();
    }

    return ps;
}


IRigidBody* const CExampleFramework::createGround() const
{
    irr::scene::ISceneNode *Node = device->getSceneManager()->addCubeSceneNode(1.0);
	Node->setScale(irr::core::vector3df(600,3,600)); // 400, 3, 400
	Node->setPosition(irr::core::vector3df(200,0,100));
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture("rockwall.jpg"));

    if(drawWireFrame)
        Node->setMaterialFlag(irr::video::EMF_WIREFRAME, true);

	ICollisionShape *shape = new IBoxShape(Node, 0, false);

	//shape->setMargin(0.01);

	IRigidBody *body;
	body = world->addRigidBody(shape);

	return body;
}



CExampleFramework::~CExampleFramework()
{
}
