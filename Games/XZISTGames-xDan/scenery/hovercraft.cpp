
#include "hovercraft.h"

HoverCraft::HoverCraft(irrInfo *info) : SpaceShip(info)
{
    // create the scene node with a mesh
    irrNode = info->smgr->addAnimatedMeshSceneNode(info->smgr->getMesh("mesh/spaceship1/spaceship1.x"));

    dInitODE();  /// added this in

    // create body and geom
    dMassSetBoxTotal(&mass, 100, 30, 30, 30);
    dBodySetMass(dBody, &mass);
    dGeom = dCreateBox(dSpace, 30, 30, 30);

    dGeomSetBody(dGeom, dBody);
    dGeomSetData(dGeom, (void *)&collision);

    // set various node effects
    addShadows(irrNode);

	// set position just in case
	setPosition(0,0,0);

	// Now for some hovercraft specific code.

    HoverHeight = 200;

    // particle
    leftThrustEmitting = false;
    rightThrustEmitting = false;

    // ray
    dRayGeom = dCreateRay(dSpace, HoverHeight);
    dGeomSetData(dRayGeom, (void *)&GroundRayCollision);

    // exhaust lasers
    exhaustLeft = new CBeamNode( irrNode, info->smgr, 200, "tex/exhaust_laser.bmp" );
    exhaustLeft->SetStartPoint( core::vector3df( 7, -2, 20 ) );
    exhaustLeft->SetEndPoint(   core::vector3df( 14, -2, 100 ) );
    exhaustLeft->SetBeamColor(  video::SColor( 255, 0, 255, 0 ) );
    exhaustLeft->SetBeamScale(4.0);
    exhaustLeft->drop();

    exhaustRight = new CBeamNode( irrNode, info->smgr, 200, "tex/exhaust_laser.bmp" );
    exhaustRight->SetStartPoint( core::vector3df( -7, -2, 20 ) );
    exhaustRight->SetEndPoint(   core::vector3df( -14, -2, 100 ) );
    exhaustRight->SetBeamColor(  video::SColor( 255, 0, 255, 0 ) );
    exhaustRight->SetBeamScale(4.0);
    exhaustRight->drop();
}

HoverCraft::~HoverCraft()
{
    if (dRayGeom) dGeomDestroy(dRayGeom);

    exhaustLeft->remove();
    exhaustRight->remove();
}

void HoverCraft::Update()
{
    // get location
    dReal *odePos = (dReal *)dGeomGetPosition(dGeom);
    // get rotation
    dQuaternion result;
    dGeomGetQuaternion(dGeom, result);
    vector3df rot;
    QuaternionToEuler(result, rot);
    // get velocity
    const dReal *linVel = dBodyGetLinearVel(dBody);
    // angular
    const dReal *angVel = dBodyGetAngularVel(dBody);

    if (GroundRayCollision.getAmount() > 1 || odePos[1] < 0)
    {
        dBodyAddForce(dBody, 0, 140000, 0);
    }

    // small force to keep the ship upright
    dBodyAddForceAtRelPos(dBody, 0,10000,0, 0,50,0);

    // set ray's new position
    dGeomRaySet(dRayGeom, odePos[0], odePos[1], odePos[2], 0,-1,0);

    // left thruster
    if (goingRight || goingForwards)
    {
        exhaustLeft->setVisible(true);
    }
    else
    {
        exhaustLeft->setVisible(false);
    }
    // right thruster
    if (goingLeft || goingForwards)
    {
        exhaustRight->setVisible(true);
    }
    else
    {
        exhaustRight->setVisible(false);
    }

    // set graphics location from physics
    vector3df irrPos;
    irrPos.set((f32)odePos[0], odePos[1], odePos[2]);
    irrNode->setPosition(irrPos);
    irrNode->setRotation(rot);

    // give forces
    if (goingForwards)
    {
        dBodyAddRelForce(dBody, 0, 0, -100000);
    }
    if (goingLeft)
    {
        dBodyAddRelTorque(dBody, 0, -600000, 0);
    }

    if (goingRight)
    {
        dBodyAddRelTorque(dBody, 0, 600000, 0);
    }

    if (!crashing)
    {
        // add air resistance
        dReal airResistance = 0.99;
        dBodySetLinearVel(dBody, linVel[0]*airResistance, linVel[1]*airResistance, linVel[2]*airResistance);
        // angular dampening (to prevent spinning out of control)
        dReal angDamp = 0.95;
        dBodySetAngularVel(dBody, angVel[0]*angDamp, angVel[1]*angDamp, angVel[2]*angDamp);
    }

    GroundRayCollision.reset();
}

