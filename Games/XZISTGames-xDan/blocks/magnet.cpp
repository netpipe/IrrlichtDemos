
#include "magnet.h"
#define GRAD_PI 57.29577951f
#define GRAD_PI2 0.174532925f

Magnet::Magnet(Info *info, dWorldID dWorld, dSpaceID dSpace, vector3df pos)
{
    // load base
    //nodeBase = info->smgr->addAnimatedMeshSceneNode(info->smgr->getMesh("block2.obj"));

    // constants
    const int numLinks = 15;
    vector3df linkShape = vector3df(10,46,10);

    this->pos = pos;
    this->dWorld = dWorld;

    dMass mass;

    // joint pointer
    dJointID joint;

    f32 y = pos.Y;

    for (i = 0; i < numLinks; i ++)
    {
        nodes.push_back(info->smgr->addAnimatedMeshSceneNode(info->smgr->getMesh("link.obj")));
        bodies.push_back(dBodyCreate(dWorld));
        geoms.push_back(dCreateBox(dSpace, linkShape.X,linkShape.Y,linkShape.Z));

        dMassSetBox(&mass, 1, linkShape.X,linkShape.Y,linkShape.Z);
        dBodySetMass(bodies[i], &mass);
        dGeomSetBody(geoms[i], bodies[i]);

        dBodySetPosition(bodies[i], pos.X, y, pos.Z);
        y -= linkShape.Y;
    }

    nodes.push_back(info->smgr->addAnimatedMeshSceneNode(info->smgr->getMesh("magnet.obj")));
    bodies.push_back(dBodyCreate(dWorld));
    geoms.push_back(dCreateSphere(dSpace, 25));

    // set ODE mass, body...
    dMassSetSphere(&mass, 0.001, 25);
    dBodySetMass(bodies[i], &mass);
    dGeomSetBody(geoms[i], bodies[i]);

    // set ye olde magnet datum
    colMagnet.type = COLLISION_MAGNET;
    colMagnet.data = (void *)this;
    dGeomSetData(geoms[i], (void *)&colMagnet);

    // set magnet position at end of chain
    dBodySetPosition(bodies[i], pos.X, y, pos.Z);

    // attach world to first (maybe don't do this and set position directly every frame???! or not.)
    worldJoint = dJointCreateBall(dWorld, 0);
    dJointAttach(worldJoint, 0, bodies[0]);
    dJointSetBallAnchor(worldJoint, pos.X, pos.Y, pos.Z);

    // join all links together
    y = pos.Y - (linkShape.Y/2);
    for (i = 0; i < numLinks; i ++)
    {
        joint = dJointCreateBall(dWorld, 0);
        dJointAttach(joint, bodies[i], bodies[i+1]);
        dJointSetBallAnchor(joint, pos.X, y, pos.Z);
        y -= linkShape.Y;
    }
}

Magnet::~Magnet()
{
}

vector3df Magnet::getEndPos()
{
    dReal *p = (dReal *)dBodyGetPosition(bodies[bodies.size()-1]);
    return vector3df(p[0],p[1],p[2]);
}

void Magnet::translate(vector3df t)
{
    pos += t;
}

void Magnet::attachBlock(Block *block, dContactGeom contact)
{
    vector3df touch = vector3df(contact.pos[0],contact.pos[1],contact.pos[2]);
    dJointID joint = dJointCreateBall(dWorld, 0);
    blockJoints.push_back(joint);
    dJointAttach(joint, block->getBody(), bodies[bodies.size()-1]);
    dJointSetBallAnchor(joint, touch.X, touch.Y, touch.Z);
}

void Magnet::release()
{
    for (i = 0; i < blockJoints.size(); i ++)
    {
        dJointDestroy(blockJoints[i]);
    }
    blockJoints.clear();
}

void Magnet::Refresh()
{
    // balloon effect, woo!
    //dBodyAddForce(bodies[bodies.size()-1], 0, 10000000, 0);
    //dBodyAddForce(bodies[0], 0, 30000000, 0);

    // set base position
    dBodySetPosition(bodies[0], pos.X, pos.Y, pos.Z);
    dJointSetBallAnchor(worldJoint, pos.X, pos.Y, pos.Z);

    // get graphic positions
    for (i = 0; i < geoms.size(); i ++)
    {
        /*
        // should damp using forces!
        const dReal *f = dBodyGetLinearVel(bodies[i]);
        const dReal slow = 0.95;
        dBodySetLinearVel(bodies[i], f[0]*slow, f[1]*slow, f[2]*slow);
        f = dBodyGetAngularVel(bodies[i]);
        dBodySetAngularVel(bodies[i], f[0]*slow, f[1]*slow, f[2]*slow);
        */
        dReal *odePos = (dReal *)dGeomGetPosition(geoms[i]);

        // get rotation
        dQuaternion result;
        dGeomGetQuaternion(geoms[i], result);
        vector3df rot;
        QuaternionToEuler(result, rot);

        // set graphics location from physics
        vector3df irrPos = vector3df(odePos[0], odePos[1], odePos[2]);
        nodes[i]->setPosition(irrPos);
        nodes[i]->setRotation(rot);
    }

    // reset collision struct
    colMagnet.reset();
}

void Magnet::QuaternionToEuler(const dQuaternion quaternion, vector3df &euler)
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

