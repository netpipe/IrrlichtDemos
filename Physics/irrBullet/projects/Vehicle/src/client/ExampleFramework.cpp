// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include <irrlicht.h>
#include <irrBulletWorld.h>
#include <irrBulletRigidBody.h>
#include <irrBulletBoxShape.h>
#include <irrBulletSphereShape.h>
#include <irrBulletGImpactMeshShape.h>
#include <irrBulletRayCastVehicle.h>
#include <irrBulletCollisionObjectAffectorDelete.h>
#include <irrBulletCollisionObjectAffectorAttract.h>
#include "ExampleFramework.hpp"


CExampleFramework::CExampleFramework()
{
}

IRigidBody* const CExampleFramework::addCube(const irr::core::vector3df &pos, const irr::core::vector3df &scale, irr::f32 mass, irr::core::stringc textureFile) const
{
    auto Node = device->getSceneManager()->addCubeSceneNode(1.0f);
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture(textureFile.c_str()));

    Node->setMaterialFlag(irr::video::EMF_WIREFRAME, drawWireFrame);

	auto shape = new IBoxShape(Node, mass, false);

	auto body = world->addRigidBody(shape);

	return body;
}

IRigidBody* const CExampleFramework::shootCube(const irr::core::vector3df &scale, irr::f32 mass, irr::core::stringc textureFile) const
{
	irr::core::vector3df pos(camera->getPosition().X,camera->getPosition().Y,camera->getPosition().Z);

	auto Node = device->getSceneManager()->addCubeSceneNode(1.0f);
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture(textureFile.c_str()));

    Node->setMaterialFlag(irr::video::EMF_WIREFRAME, drawWireFrame);

	auto shape = new IBoxShape(Node, mass, true);

	auto body = world->addRigidBody(shape);

	irr::core::vector3df rot = camera->getRotation();
    irr::core::matrix4 mat;
    mat.setRotationDegrees(rot);
    irr::core::vector3df forwardDir(irr::core::vector3df(mat[8],mat[9],mat[10]) * 120);

    body->setLinearVelocity(forwardDir);

    body->setDamping(0.2,0.2);
    body->setFriction(0.4f);

    body->getAttributes()->addBool("collide", true);

    return body;
}

IRigidBody* const CExampleFramework::shootSphere(const irr::core::vector3df &scale, irr::f32 mass) const
{
	irr::core::vector3df pos(camera->getPosition().X,camera->getPosition().Y,camera->getPosition().Z);

	auto Node = device->getSceneManager()->addSphereSceneNode();
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture("rockwall.jpg"));

	if(drawWireFrame)
        Node->setMaterialFlag(irr::video::EMF_WIREFRAME, drawWireFrame);

	auto shape = new ISphereShape(Node, mass, true);

	auto body = world->addRigidBody(shape);
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
	auto Node = device->getSceneManager()->addAnimatedMeshSceneNode(
        device->getSceneManager()->getMesh(file.c_str()));
	Node->setPosition(pos);
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);


	auto shape = new IGImpactMeshShape(Node, device->getSceneManager()->getMesh(collFile.c_str()), mass);

	auto tank = world->addRigidBody(shape);
	tank->includeNodeOnRemoval(false);

    tank->setName("Tank");
	tank->getAttributes()->addBool("vehicle", true);
	tank->getAttributes()->addBool("destroyable", true);
	tank->getAttributes()->addBool("isDestroyed", false);

    // When using a raycast vehicle, we don't want this rigid body to deactivate.
	tank->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION);

    // We create our vehicle, passing our newly created rigid body as a parameter.
	auto vehicle = world->addRaycastVehicle(tank);


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
	auto ps =
    device->getSceneManager()->addParticleSystemSceneNode(false);

    auto em = ps->createBoxEmitter(
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
        auto paf = ps->createFadeOutParticleAffector();
        ps->addAffector(paf); // same goes for the affector
        paf->drop();
    }

    if(gravity==true)
    {
        auto paf = ps->createGravityAffector(irr::core::vector3df(0.0f,-0.007f, 0.0f), 1800);
        ps->addAffector(paf); // same goes for the affector
        paf->drop();
    }

    ps->setScale(irr::core::vector3df(0.4,0.4,0.4));
    ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    ps->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, device->getVideoDriver()->getTexture(textureFile.c_str()));
    ps->setMaterialType(materialType);

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
    auto Node = device->getSceneManager()->addCubeSceneNode(1.0);
	Node->setScale(irr::core::vector3df(600,3,600)); // 400, 3, 400
	Node->setPosition(irr::core::vector3df(200,0,100));
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture("rockwall.jpg"));

    if(drawWireFrame)
        Node->setMaterialFlag(irr::video::EMF_WIREFRAME, true);

	auto shape = new IBoxShape(Node, 0, false);

	auto body = world->addRigidBody(shape);

	return body;
}



CExampleFramework::~CExampleFramework()
{
}
