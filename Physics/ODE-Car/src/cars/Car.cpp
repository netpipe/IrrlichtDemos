#include "Car.h"
#include "World.h"

#include "PhysicAnimator.h"

using namespace irr;
using namespace irr::io;
using namespace irr::core;
using namespace scene;

/**
*	Perform computing physics
*/
class CCarAnimator :
	public ISceneNodeAnimator
{
private:
	CCar *m_car;

public:
	CCarAnimator(CCar *car) : m_car(car) {}

	//! Animates a scene node.
	/** \param node Node to animate.
	\param timeMs Current time in milli seconds. */
	virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs) {
		m_car->animate();
	}

	//! Creates a clone of this animator.
	/** Please note that you will have to drop
	(IReferenceCounted::drop()) the returned pointer after calling this. */
	virtual ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node,
			irr::scene::ISceneManager* newManager=0) {
		return (new CCarAnimator(m_car));
	}
};

CCar::CCar(CWorld *world)
	: m_world(world), m_chassis(0), m_glass(0), m_damaged(false), m_speed(0.0f), m_steer(0.0f), m_acceleration(0.0), m_steer_acceleration(0.0), m_max_speed(3.0)
{
	memset(m_wheel, 0, sizeof(irr::scene::IMeshSceneNode*)*4);

	p.car_space = dSimpleSpaceCreate(world->getPhysicSpace());

	// to auto remove all geom
	dSpaceSetCleanup(p.car_space, 1);

	m_root = world->getSceneManager()->addEmptySceneNode(world->getCarsRoot());
}

CCar::~CCar(void)
{
	dSpaceDestroy(p.car_space);
	for (int i = 0; i < 4; i++)
		dJointDestroy(p.joint[i]);
	dBodyDestroy(p.chassis_body);
	for (int i = 0; i < 4; i++)
		dBodyDestroy(p.wheel_body[i]);

	m_root->remove();
}

void CCar::setChassis(irr::scene::IMesh * mesh, irr::video::ITexture * map, const irr::core::vector3df &position, float mass)
{
	scene::ISceneManager * smgr = m_world->getSceneManager();

	// save clean mesh
	g.clean_model = mesh;
	g.clean_chassis_texture = map;

	// create scene node
	m_chassis = smgr->addMeshSceneNode(mesh, m_root,-1, position);

	m_chassis->setName(m_name + "_chassis");
	m_chassis->setMaterialTexture(0, map);

	// create physic model
	dMass m;

	// bounding box sizes
	const irr::core::aabbox3df &aabb = mesh->getBoundingBox();
	float car_width = aabb.MaxEdge.X - aabb.MinEdge.X,
     	  car_height = aabb.MaxEdge.Y - aabb.MinEdge.Y,
		  car_lenght = aabb.MaxEdge.Z - aabb.MinEdge.Z;

	// create ODE stuff
	p.chassis_body = dBodyCreate(m_world->getPhysicWorld());
	dBodySetData(p.chassis_body, (void*)m_chassis);
	dBodySetPosition(p.chassis_body, position.X, position.Y, position.Z);
	dMassSetBox(&m, 1, car_width, car_height, car_lenght);
	dMassAdjust(&m, mass);
	dBodySetMass(p.chassis_body, &m);

	p.chassis_geom = dCreateBox(0, car_width, car_height, car_lenght);
	dGeomSetBody(p.chassis_geom, p.chassis_body);

	dGeomSetData(p.chassis_geom, this);

	// add geom into space
	dSpaceAdd(p.car_space, p.chassis_geom);

	// attach animator
	ISceneNodeAnimator * animator = new CPhysicAnimator(p.chassis_geom);
	m_chassis->addAnimator(animator);
	animator->drop();

	// create car animator
	animator = new CCarAnimator(this);
	m_chassis->addAnimator(animator);
	animator->drop();
}

void CCar::setGlass(irr::scene::IMesh * mesh, irr::video::ITexture * map)
{
	scene::ISceneManager * smgr = m_world->getSceneManager();

	// save clean model
	g.clean_glass = mesh;
	g.clean_glass_texture = map;

	// create scene node
	m_glass = smgr->addMeshSceneNode(mesh, m_chassis);
	m_glass->setName(m_name + "_glass");
	m_glass->setMaterialTexture(0, g.getGlassTexture());
	m_glass->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
}

void CCar::addWheel(EWheel id, const irr::core::vector3df &position, float mass)
{
	scene::ISceneManager * smgr = m_world->getSceneManager();

	// create scene node
	m_wheel[id] = smgr->addMeshSceneNode(g.wheel, m_root, -1, position);
	m_wheel[id]->setMaterialTexture(0, g.wheel_texture);
	m_wheel[id]->setName(m_name + "_wheel");

	// create physic stuff
	dMass m;

	// compute radius from mesh
	float wheel_radius = abs(g.wheel->getBoundingBox().MaxEdge.Y);

	// create body
	dBodyID wheel_body = p.wheel_body[id] = dBodyCreate(m_world->getPhysicWorld());
	dBodySetData(wheel_body, (void*)m_wheel[id]);
	dQuaternion q;
	dQFromAxisAndAngle(q, 1, 0, 0, M_PI*0.5);
	dBodySetQuaternion(wheel_body, q);

	dMassSetSphere(&m, 1, wheel_radius);
	dMassAdjust(&m, mass);

	dBodySetMass(wheel_body, &m);

	// create sphere geom
	dGeomID wheel_geom = p.wheel_geom[id] = dCreateSphere(0, wheel_radius);
	dGeomSetBody(wheel_geom, wheel_body);

	// TODO: set body global position
	dBodySetPosition(wheel_body, position.X, position.Y, position.Z);

	// create joint
	dJointID joint = p.joint[id] = dJointCreateHinge2(m_world->getPhysicWorld(), 0);
	dJointAttach(joint, p.chassis_body, wheel_body);
	const dReal *a = dBodyGetPosition(wheel_body);
	dJointSetHinge2Anchor(joint, a[0], a[1], a[2]);
	dJointSetHinge2Axis1(joint, 0,1,0);
	dJointSetHinge2Axis2(joint, 1,0,0);

	// set joint suspension
	dJointSetHinge2Param(joint, dParamSuspensionERP, 0.4f);
	dJointSetHinge2Param(joint, dParamSuspensionCFM, 0.8f);

	dJointSetHinge2Param(joint, dParamLoStop, 0.0f);
	dJointSetHinge2Param(joint, dParamHiStop, 0.0f);

	// add geom into space
	dSpaceAdd(p.car_space, wheel_geom);

	// attach animator
	CPhysicAnimator * animator = new CPhysicAnimator(wheel_geom);
	m_wheel[id]->addAnimator(animator);
	animator->drop();
}

void CCar::animate()
{
	if (m_steer_acceleration == 0.0f)
	{
		float val = 0.01f;

		if (abs(m_steer) < 0.01f)
			val = m_steer;

		if (m_steer > 0.0f)
			m_steer_acceleration = -val;
		else if (m_steer < 0.0f)
			m_steer_acceleration = val;
	}

	m_acceleration += 0.005f * -m_speed * 0.1f;

	m_speed += m_acceleration;
	m_steer += m_steer_acceleration;

	if (m_steer > 30 * core::DEGTORAD)
		m_steer = 30 * core::DEGTORAD;
	else
	if (m_steer < -30 * core::DEGTORAD)
		m_steer = -30 * core::DEGTORAD;

	m_acceleration = 0.0f;
	m_steer_acceleration = 0.0f;

	for (int i = 0; i < 2; i++)
	{
		// motor
		dJointSetHinge2Param (p.joint[i],dParamVel2, m_speed);
		dJointSetHinge2Param (p.joint[i],dParamFMax2, 0.1f);
	}
	for (int i = 0; i < 2; i++)
	{
		// steering
		dReal v = m_steer - dJointGetHinge2Angle1 (p.joint[i]);
		if (v > 0.1f) v = 0.1f;
		if (v < -0.1f) v = -0.1f;
		v *= 10.0;
		dJointSetHinge2Param (p.joint[i],dParamVel,v);
		dJointSetHinge2Param (p.joint[i],dParamFMax,0.2f);
		dJointSetHinge2Param (p.joint[i],dParamLoStop,-0.75f);
		dJointSetHinge2Param (p.joint[i],dParamHiStop,0.75f);
		dJointSetHinge2Param (p.joint[i],dParamFudgeFactor,0.1f);
	}
}

void CCar::setPosition (const irr::core::vector3df &newpos) {
	assert(m_chassis != NULL);

	// get wheels relative positions
	vector3df parentPosition = m_chassis->getAbsolutePosition();
	vector3df wheelRelativePositions[4];
	vector3df wheelPosition;

	for (int i = 0; i < 4; i++)
		wheelRelativePositions[i] = m_wheel[i]->getAbsolutePosition() - parentPosition;

	// set chassis position
	m_chassis->setPosition(newpos);

	// update body positions
	dBodySetPosition(p.chassis_body, newpos.X, newpos.Y, newpos.Z);

	for (int i = 0; i < 4; i++)
	{
		wheelPosition = newpos + wheelRelativePositions[i];

		// set wheel position
		m_wheel[i]->setPosition(wheelPosition);

		// update body position
		dBodySetPosition(p.wheel_body[i], wheelPosition.X, wheelPosition.Y, wheelPosition.Z);
	}
}

void CCar::setRotation(const irr::core::vector3df &rotation)
{
	// update body transformation
	core::quaternion q;
	q.set(rotation);
	dQuaternion q_arr = {q.W, q.X, q.Y, q.Z};
	dBodySetQuaternion(p.chassis_body, q_arr);
}

void CCar::turnRight(irr::s32 angle) {
	m_steer_acceleration = -0.01f;
}
void CCar::turnLeft(irr::s32 angle) {
	m_steer_acceleration = 0.01f;
}

void CCar::accelerate(irr::s32 force) {
	m_acceleration = 0.01f * ((m_max_speed - m_speed) / m_max_speed);
}
void CCar::slowdown(irr::s32 force) {
	m_acceleration = -0.003f;
}

//! set car looking as damaged
void CCar::damage()
{
	if (g.damaged_model)
		m_chassis->setMesh(g.damaged_model);

	m_chassis->setMaterialTexture(0, g.getDamagedChassisTexture());
	m_glass->setMaterialTexture(0, g.getDamagedGlassTexture());

	m_damaged = true;

	m_speed = 0.0f;
}

void CCar::reset()
{
	m_chassis->setMesh(g.clean_model);

	m_chassis->setMaterialTexture(0, g.getChassisTexture());
	m_glass->setMaterialTexture(0, g.getGlassTexture());

	m_damaged = false;

	m_speed = 0.0;
}
