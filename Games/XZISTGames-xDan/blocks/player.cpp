
#include "player.h"
#define GRAD_PI 57.29577951f
#define GRAD_PI2 0.174532925f
Player::Player(Info *info, dWorldID dWorld, dSpaceID dSpace)
{

    mesh = info->smgr->getMesh("sphere.obj");
    node = info->smgr->addAnimatedMeshSceneNode(mesh);

    dBody = dBodyCreate(dWorld);
    dMass mass;

    // need a certain width to prevent falling through floor
    f32 playerHeight = 20;//50;

    // set ODE mass, body...
    dMassSetSphere(&mass, 0.0001, playerHeight);
    dBodySetMass(dBody, &mass);

    //maybe set geom as cylinder?
    dGeom = dCreateSphere(dSpace, playerHeight);
    //dCreateCCylinder(dSpace, 20, playerHeight);
    dGeomSetBody(dGeom, dBody);

    // collision
    collision.type = COLLISION_PLAYER;
    collision.data = (void *)this;
    // collision material
    collision.surface.mode = dContactBounce | dContactSoftCFM;
    collision.surface.mu = 10000000;
    collision.surface.bounce = 0;
    collision.surface.bounce_vel = 0;
    collision.surface.soft_cfm = 0;
    collision.surfaceSet = true;
    // pointer...
    dGeomSetData(dGeom, (void *)&collision);

    // ground ray
    groundRayCollision.type = COLLISION_UNKNOWN;
    groundRayCollision.data = (void *)this;
    dGroundRayGeom = dCreateRay(dSpace, playerHeight*2);
    dGeomSetData(dGroundRayGeom, (void *)&groundRayCollision);

    vParallel = 0;
    vPerpendicular = 0;

    moveVel = 4000000.0;
    //moveVel = 500000.0;

    setPosition(vector3df(0,0,0));

    // body auto disable?
    dBodySetAutoDisableFlag(dBody, true);
    dBodySetAutoDisableLinearThreshold(dBody, 1);
    dBodySetAutoDisableAngularThreshold(dBody, 1);
    dBodySetAutoDisableSteps(dBody, 2);
    dBodySetAutoDisableTime(dBody, 0);
}

Player::~Player()
{
}

void Player::Refresh()
{
    f32 yaw = yawAngle.Y*0.017453;

    // friction - because we have zero friction for object sliding...
    if (!onGround())
    {
        const dReal *f = dBodyGetLinearVel(dBody);
        const dReal slow = 0.80;
        dBodySetLinearVel(dBody, f[0]*slow, f[1], f[2]*slow);
    }

    // no rotation
    dBodySetAngularVel(dBody, 0, 0, 0);

    if (vParallel != 0 && vPerpendicular != 0) {
        vParallel /= sqrt(2);
        vPerpendicular /= sqrt(2);
    }

    float vx = vParallel * sin(yaw) + vPerpendicular * cos(yaw);
    float vy = vParallel * cos(yaw) - vPerpendicular * sin(yaw);

    //dBodyAddForce(dBody, vx, 0, vy);
    const dReal *la = dBodyGetLinearVel(dBody);
    dReal vert = la[1];
    if (vUp) vert = moveVel/8000.0;
    if (vDown) vert = -moveVel/8000.0;
    // should change so "/8000" not needed, use correct moveVel?

    dBodySetLinearVel(dBody, vx/8000.0, vert, vy/8000.0);

    // reset movement components.
    vParallel = 0;
    vPerpendicular = 0;
    vUp = false;
    vDown = false;

    // get location
    dReal *odePos = (dReal *)dGeomGetPosition(dGeom);

    // get rotation
    dQuaternion result;
    dGeomGetQuaternion(dGeom, result);
    vector3df rot;
    QuaternionToEuler(result, rot);

    // set graphics location from physics
    vector3df irrPos = vector3df(odePos[0]+40, odePos[1], odePos[2]);
    node->setPosition(irrPos);
    node->setRotation(rot);

    // set ray's new position
    dGeomRaySet(dGroundRayGeom, odePos[0], odePos[1], odePos[2], 0,-1,0);

    // reset collisions
    groundRayCollision.reset();
}

void Player::setPosition(vector3df pos)
{
    dGeomSetPosition(dGeom, pos.X, pos.Y, pos.Z);
    node->setPosition(pos);
}

vector3df Player::getPosition()
{
    dReal *odePos = (dReal *)dGeomGetPosition(dGeom);
    vector3df irrPos = vector3df(odePos[0], odePos[1], odePos[2]);
    return irrPos;
}

void Player::setAngle(vector3df angle)
{
    yawAngle = angle;
}

void Player::goForward()
{
    vParallel += moveVel;
    dBodyEnable(dBody);
}

void Player::goBackward()
{
    vParallel -= moveVel;
    dBodyEnable(dBody);
}

void Player::goLeft()
{
    vPerpendicular -= moveVel;
    dBodyEnable(dBody);
}

void Player::goRight()
{
    vPerpendicular += moveVel;
    dBodyEnable(dBody);
}

void Player::goUp()
{
    vUp = true;
    dBodyEnable(dBody);
}

void Player::goDown()
{
    vDown = true;
    dBodyEnable(dBody);
}

void Player::jump()
{
    // possibly also check for main body collision so only jumps when exactly on ground.
    dBodySetLinearVel(dBody, 0, 400, 0);
    //dBodyAddForce(dBody, 0, 1000000, 0);
    dBodyEnable(dBody);
}

bool Player::onGround()
{
    if (groundRayCollision.getAmount() > 1) return true;
    return false;
}

void Player::QuaternionToEuler(const dQuaternion quaternion, vector3df &euler)
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
