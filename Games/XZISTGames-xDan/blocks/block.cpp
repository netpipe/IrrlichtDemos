
#include "block.h"

#define GRAD_PI 57.29577951f
#define GRAD_PI2 0.174532925f

Block::Block(Info *info, dWorldID dWorld, dSpaceID dSpace, int type)
{
    if (type < 0 || type > 1)
    {
        type = 0;
    }

    this->type = type;

    dBody = dBodyCreate(dWorld);
    dReal density = 0;
    dMass mass;

    if (type == 0) {
        mesh = info->smgr->getMesh("block1.obj");
        density = 0.01;
    }
    if (type == 1) {
        mesh = info->smgr->getMesh("block2.obj");
        density = 0.0001;
    }

    node = info->smgr->addAnimatedMeshSceneNode(mesh);

    // get bounding box for ODE dimensions
    aabbox3df box = node->getBoundingBox();
    vector3df extend = box.getExtent();

    // set ODE mass, body...
    dMassSetBox(&mass, density, (dReal)extend.X, (dReal)extend.Y, (dReal)extend.Z);
    dBodySetMass(dBody, &mass);
    dGeom = dCreateBox(dSpace, (dReal)extend.X, (dReal)extend.Y, (dReal)extend.Z);
    dGeomSetBody(dGeom, dBody);

    // collision structure (for magnet)
    collision.type = COLLISION_BLOCK;
    collision.data = (void *)this;
    // collision material (for ODE)
    collision.surface.mode = dContactBounce | dContactSoftCFM;
    collision.surface.mu = dInfinity;
    collision.surface.bounce = 0;
    collision.surface.bounce_vel = 0;
    collision.surface.soft_cfm = 0;
    collision.surfaceSet = true;
    // give geom the pointer
    dGeomSetData(dGeom, (void *)&collision);

    // body auto disable?
    dBodySetAutoDisableFlag(dBody, true);
    dBodySetAutoDisableLinearThreshold(dBody, 1);
    dBodySetAutoDisableAngularThreshold(dBody, 1);
    dBodySetAutoDisableSteps(dBody, 2);
    dBodySetAutoDisableTime(dBody, 0);

    // set node materials (not loaded in .x file...!)
    node->setMaterialFlag(EMF_LIGHTING, true);
    for (i = 0; i < node->getMaterialCount(); i ++)
    {
        node->getMaterial(i).AmbientColor.set(0, 10, 10, 10);
        node->getMaterial(i).DiffuseColor.set(0, 10, 200, 0);
        //node->getMaterial(i).EmissiveColor.set(0,0,0,0);
    }

    setPosition(vector3df(0,0,0));
    setRotation(vector3df(0,0,0));
}

void Block::setPosition(vector3df pos)
{
    dGeomSetPosition(dGeom, pos.X, pos.Y, pos.Z);
    node->setPosition(pos);
}

// may or may not work, I haven't tested.
void Block::setRotation(vector3df rot)
{
    //dQuaternion quaternion;
    //dGeomSetQuaternion(dGeom, quaternion);

    dMatrix3 rota;
    dRFromEulerAngles(rota, rot.X*0.017453, rot.Y*0.017453, rot.Y*0.017453);
    dGeomSetRotation(dGeom, rota);

    node->setPosition(rot);
}

void Block::Refresh()
{
    // get location
    dReal *odePos = (dReal *)dGeomGetPosition(dGeom);

    // get rotation
    dQuaternion result;
    dGeomGetQuaternion(dGeom, result);
    vector3df rot;
    QuaternionToEuler(result, rot);

    // set graphics location from physics
    vector3df irrPos = vector3df(odePos[0], odePos[1], odePos[2]);
    node->setPosition(irrPos);
    node->setRotation(rot);

    // reset collision
    collision.reset();
}

vector3df Block::getPosition()
{
    dReal *odePos = (dReal *)dGeomGetPosition(dGeom);
    vector3df irrPos = vector3df(odePos[0], odePos[1], odePos[2]);
    return irrPos;
}

vector3df Block::getRotation()
{
    dQuaternion result;
    dGeomGetQuaternion(dGeom, result);
    vector3df rot;
    QuaternionToEuler(result, rot);
    return rot;
}

void Block::QuaternionToEuler(const dQuaternion quaternion, vector3df &euler)
{
    dReal w,x,y,z;
    w=quaternion[0];
    x=quaternion[1];
    y=quaternion[2];
    z=quaternion[3];
    double sqw = w*w;
    double sqx = x*x;
    double sqy = y*y;
    double sqz = z*z;
    euler.Z = (irr::f32) (atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))*GRAD_PI);
    euler.X = (irr::f32) (atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))*GRAD_PI);
    euler.Y = (irr::f32) (asin(-2.0 * (x*z - y*w))*GRAD_PI);
}
