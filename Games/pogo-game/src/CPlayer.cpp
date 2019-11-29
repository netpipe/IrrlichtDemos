#include "../include/CPlayer.h"

CPlayer::CPlayer(f32 mmass,scene::ISceneManager* mgr,scene::ICameraSceneNode* mcam,btDiscreteDynamicsWorld* mdynamicsWorld,s32 mat,int windPower)
{
    mass=mmass;
    cam=mcam;
    dynamicsWorld=mdynamicsWorld;

    {
        //create a dynamic rigidbody
        btCollisionShape* colShape = new btSphereShape(50);


        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar	mass(1.f);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass,localInertia);

        startTransform.setOrigin(btVector3(0,200,0));

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
        body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);

    }

    //setup HUD nodes
    animCallback*animcb=new animCallback();

    jumpComboNode =mgr->addAnimatedMeshSceneNode(mgr->getMesh("text.b3d"),cam);
    jumpComboNode->setPosition(core::vector3df(-2.75,5,10));
    jumpComboNode->setScale(core::vector3df(3.5,3.5,3.5));
    jumpComboNode->setAnimationSpeed(100);
    jumpComboNode->setLoopMode(false);
    jumpComboNode->setMaterialTexture(0, mgr->getVideoDriver()->getTexture("red.tga"));


    jumpComboNode->setAnimationEndCallback(animcb);
    jumpComboNode->setMaterialFlag(video::EMF_LIGHTING, false);

    coinAirNode =mgr->addAnimatedMeshSceneNode(mgr->getMesh("cointext.b3d"),cam);
    coinAirNode->setPosition(core::vector3df(-2.75,5,10));
    coinAirNode->setScale(core::vector3df(3.5,3.5,3.5));
    coinAirNode->setAnimationSpeed(100);
    coinAirNode->setLoopMode(false);
    coinAirNode->setMaterialTexture(0, mgr->getVideoDriver()->getTexture("red.tga"));


    coinAirNode->setAnimationEndCallback(animcb);
    coinAirNode->setMaterialFlag(video::EMF_LIGHTING, false);

    specialCoinNode =mgr->addAnimatedMeshSceneNode(mgr->getMesh("specialcoin.b3d"),cam);
    specialCoinNode->setPosition(core::vector3df(-2.75,5,10));
    specialCoinNode->setScale(core::vector3df(3.5,3.5,3.5));
    specialCoinNode->setAnimationSpeed(100);
    specialCoinNode->setLoopMode(false);
    specialCoinNode->setMaterialTexture(0, mgr->getVideoDriver()->getTexture("red.tga"));

    specialCoinNode->setAnimationEndCallback(animcb);
    specialCoinNode->setMaterialFlag(video::EMF_LIGHTING, false);

    flagNode =mgr->addAnimatedMeshSceneNode(mgr->getMesh("flag.md2"),cam);
    flagNode->setRotation(core::vector3df(0,90,0));
    flagNode->setPosition(core::vector3df(10,10,20));
    flagNode->setScale(core::vector3df(4,4,4));
    flagNode->setAnimationSpeed(80);
    flagNode->setMaterialType(video::EMT_SPHERE_MAP);
    flagNode->setMaterialTexture(0,  mgr->getVideoDriver()->getTexture("coin_a.tga"));
    flagNode->setMaterialTexture(1,  mgr->getVideoDriver()->getTexture("coin_n.tga"));
    flagNode->setMaterialTexture(0,  mgr->getVideoDriver()->getTexture("flag.tga"));


    //flagNode->setMaterialTexture(0, mgr->getVideoDriver()->getTexture("red.tga"));

    flagNode->setFrameLoop (30, 5000);
    flagNode->setMaterialFlag(video::EMF_LIGHTING, false);
    flagNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    windGen=scene::createWindGenerator(windPower, 1.0f);

}

CPlayer::~CPlayer()
{
    //dtor
}

core::vector3df CPlayer::getVel()
{
    return core::vector3df(body->getLinearVelocity().getX(),body->getLinearVelocity().getY(),body->getLinearVelocity().getZ());
}
//Override cancels horizontal velocities
void CPlayer::applyForce(core::vector3df appForce,bool override)
{
    float  yvel=body->getLinearVelocity().getY();
    if (override)
        body->setLinearVelocity(btVector3(0,appForce.Y/100000,0));
    body->applyImpulse(btVector3(appForce.X,appForce.Y,appForce.Z),btVector3(0,0,0));
}

void CPlayer::update(f32 gravity,f32 time,f32 dt)
{
    core::vector2df wind2D=windGen->getWind( core::vector3df(0,0,0),time/10);
    float heightFactor=cam->getPosition().Y/1500;
    core::vector3df wind3D=core::vector3df(wind2D.X*heightFactor,0,wind2D.Y*heightFactor);
    this->applyForce(wind3D,false);


    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);

    cam->setPosition(core::vector3df(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ()));
    // wind3D.dotProduct(smgr->getActiveCamera()->getTarget() - smgr->getActiveCamera()->getAbsolutePosition());
    wind3D.normalize();
    //set flag rotation smoothly using timestep and delta change in wind direction by delta time
    core::vector3df angle=core::vector3df(0,cam->getRotation().Y-wind3D.getHorizontalAngle().Y,0);
    core::vector3df  finalAngle=((-lastAngle-angle)*dt/1000)+flagNode->getRotation();
    lastAngle=flagNode->getRotation();
    flagNode->setRotation(finalAngle);


}
