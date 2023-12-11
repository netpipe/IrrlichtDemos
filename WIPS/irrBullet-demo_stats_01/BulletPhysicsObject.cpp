//CBulletPhysicsObject
//The physical representation of an object

#include "BulletPhysicsWorld.h"
#include "BulletPhysicsObject.h"
#include "CollisionObjectMgr.h"
#include "utils.h"

//we are using irrlicht vectors everywhere, so need to include irrlicht.h here
// so we can return data as irr::core::vector3df
#include "irrlicht.h"
using namespace irr;
using namespace core;

CBulletPhysicsWorld* CBulletPhysicsObject::s_pPhysicsWorld=0;

CBulletPhysicsObject::CBulletPhysicsObject()
{
	m_pCollisionObject=0;
	m_pRigidBody=0;
	m_pMotionState=0;
}

CBulletPhysicsObject::~CBulletPhysicsObject()
{
	EndPhysics();

}
void CBulletPhysicsObject::EndPhysics(void)
{
	SAFE_DELETE(m_pMotionState);
	if (s_pPhysicsWorld) s_pPhysicsWorld->m_pDynamicsWorld->removeRigidBody(m_pRigidBody);
	SAFE_DELETE(m_pRigidBody);

	g_pCollisionObjectMgr->Remove(m_pCollisionObject);
}

//initialise physics for this object
void CBulletPhysicsObject::InitPhysics(f32 ccdThreshold, f32 linearDamping, f32 angularDamping, f32 friction, f32 restitution)
{
	if (!m_pCollisionObject || !m_pCollisionObject->m_pShape) return;

	if (!s_pPhysicsWorld) return;
	btDynamicsWorld* pDynamicsWorld = s_pPhysicsWorld->m_pDynamicsWorld;

	btTransform trans;
	trans.setIdentity();

	btVector3 localInertia(0,0,0);
	if (m_Mass != 0)
		m_pCollisionObject->m_pShape->calculateLocalInertia(m_Mass,localInertia);

	m_pMotionState = new MyMotionState(trans);

	//m_pRigidBody = new btRigidBody(m_Mass,m_pMotionState,m_pCollisionObject->m_pShape,localInertia,linearDamping,angularDamping,friction,restitution);
	m_pRigidBody = new btRigidBody(m_Mass,m_pMotionState,m_pCollisionObject->m_pShape,localInertia);
	pDynamicsWorld->addRigidBody(m_pRigidBody);

	//can set unique gravity value for the body here, if necessary

	SetPosition(vector3df(0,0,0));
	SetRotation(vector3df(0,0,0));

	m_pRigidBody->setUserPointer((void*)this); //very convenient

	// Only do CCD if  motion in one timestep (1.f/60.f) exceeds half dims
	//m_pRigidBody->setCcdSquareMotionThreshold( ccdThreshold );

	//Experimental: better estimation of CCD Time of Impact:
	m_pRigidBody->setCcdSweptSphereRadius( 0.2f*ccdThreshold );
}


void CBulletPhysicsObject::SetPosition(const vector3df &v)
{
	if (!m_pRigidBody) return;
	btTransform t=m_pRigidBody->getWorldTransform();
	btVector3 btv(v.X, v.Y, v.Z);
	t.setOrigin(btv);
	m_pRigidBody->setWorldTransform(t);

	if (m_pRigidBody->getActivationState()==ISLAND_SLEEPING) {
//!		OutputDebugString("deactivated ");
	}
}
vector3df CBulletPhysicsObject::GetPosition()
{
	if (!m_pRigidBody) return vector3df(0,0,0);
	btVector3 p = m_pRigidBody->getCenterOfMassPosition();
	return vector3df(p.getX(), p.getY(), p.getZ());
}
void CBulletPhysicsObject::SetRotation(const vector3df &v)
{
	if (!m_pRigidBody) return;
	btTransform t=m_pRigidBody->getWorldTransform();
	btQuaternion btq;
//!	EulerXYZToQuaternion(btVector3(v.X * GRAD_PI2, v.Y * GRAD_PI2, v.Z * GRAD_PI2), btq);
	t.setRotation(btq);
	m_pRigidBody->setWorldTransform(t);
}
vector3df CBulletPhysicsObject::GetRotation()
{
	if (!m_pRigidBody) return vector3df(0,0,0);
	btVector3 btv;
	btQuaternion btq=m_pRigidBody->getOrientation();
	QuaternionToEulerXYZ(btq, btv);
 	vector3df v(btv.getX(), btv.getY(), btv.getZ());
	v *= GRAD_PI;
	return v;
}
vector3df CBulletPhysicsObject::GetLinearVelocity() const
{
	if (!m_pRigidBody || m_Mass==0) return vector3df(0,0,0);
	btVector3 btV = m_pRigidBody->getLinearVelocity();
	return vector3df(btV.getX(), btV.getY(), btV.getZ());
}
void CBulletPhysicsObject::SetLinearVelocity(const vector3df & vel) const
{
	if (!m_pRigidBody || m_Mass==0) return; //bullet hangs if attempt to set lin. or ang. velocity on static object
	m_pRigidBody->setLinearVelocity(btVector3(vel.X, vel.Y, vel.Z));
	//note - apparently, need to use motion state on kinematic objects, and rigid body functions
	// on dynamic rigid bodies to change pos / rot etc.
}
vector3df CBulletPhysicsObject::GetAngularVelocity() const
{
	if (!m_pRigidBody || m_Mass==0) return vector3df(0,0,0);
	btVector3 btV = m_pRigidBody->getAngularVelocity();
	return vector3df(btV.getX(), btV.getY(), btV.getZ());
}
void CBulletPhysicsObject::SetAngularVelocity(const vector3df & vel) const
{
	if (!m_pRigidBody || m_Mass==0) return;
	m_pRigidBody->setAngularVelocity(btVector3(vel.X, vel.Y, vel.Z));
}
void CBulletPhysicsObject::ApplyForce(const vector3df &v)
{
	if (!m_pRigidBody) return;
	m_pRigidBody->applyForce(btVector3(v.X, v.Y, v.Z), btVector3(0,0,0));
}

void CBulletPhysicsObject::ZeroForces()
{
	if (!m_pRigidBody) return;
	m_pRigidBody->setLinearVelocity(btVector3(0,0,0));
	m_pRigidBody->setAngularVelocity(btVector3(0,0,0));
}
void CBulletPhysicsObject::QuaternionToEulerXYZ(const btQuaternion &quat,btVector3 &euler)
{
	f32 w=quat.getW();	f32 x=quat.getX();	f32 y=quat.getY();	f32 z=quat.getZ();
	double sqw = w*w; double sqx = x*x; double sqy = y*y; double sqz = z*z;
	euler.setZ((atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))));
	euler.setX((atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))));
	euler.setY((asin(-2.0 * (x*z - y*w))));
}
void CBulletPhysicsObject::EulerXYZToQuaternion(btVector3 &euler, btQuaternion &quat)
{

	btMatrix3x3 mat;
	mat.setIdentity();
	mat.setEulerZYX(euler.getX(), euler.getY(), euler.getZ());
	mat.getRotation(quat);

	//equivalent?
	//btQuaternion q(euler.getX(), euler.getY(), euler.getZ());
	//btQuaternion q1(q.getY(), q.getX(), q.getZ(), q.getW());
	//quat = q1;
}

void CBulletPhysicsObject::SetActivationState(bool active)
{
	s32 _active = (active) ? 1 : 0;
	if (m_pRigidBody) m_pRigidBody->setActivationState(_active);
}


//get mesh data. Bullet can use the meshbuffer data directly in btTriangleIndexVertexArray,
// but Irrlicht indices are 16 bits wide which is not compatible with Bullet :(
//So we are just using a btTriangleMesh instead, although it would probably
// be faster to store the indices in int arrays and use these in a btTriangleIndexVertexArray
btTriangleMesh* CBulletPhysicsObject::GetTriangleMesh(scene::IMesh* pMesh)
{
	btVector3 vertices[3];
	s32 i,j,k,index,numVertices;
	u16* mb_indices;
	i = 0;
	j = 0;
	btTriangleMesh *pTriMesh = new btTriangleMesh();

	for(u32 i=0;i<pMesh->getMeshBufferCount();i++){
		irr::scene::IMeshBuffer* mb=pMesh->getMeshBuffer(i);

		//extract vertex data
		//because the vertices are stored as structs with no common base class,
		// we need to handle each type separately
		if(mb->getVertexType()==irr::video::EVT_STANDARD){
			irr::video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)mb->getVertices();
			mb_indices = mb->getIndices();
			numVertices = mb->getVertexCount();
			for(u32 j=0;j<mb->getIndexCount();j+=3){ //get index into vertex list
				for (k=0;k<3;k++) { //three verts per triangle
					index = mb_indices[j+k];
					if (index > numVertices) goto error;
					//convert to btVector3
					vertices[k] = btVector3(mb_vertices[index].Pos.X,mb_vertices[index].Pos.Y,mb_vertices[index].Pos.Z);
				}
				pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
			}

		}
		else if(mb->getVertexType()==irr::video::EVT_2TCOORDS){
			//same but for S3DVertex2TCoords data
			irr::video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
			u16* mb_indices = mb->getIndices();
			s32 numVertices = mb->getVertexCount();
			for(u32 j=0;j<mb->getIndexCount();j+=3){ //index into irrlicht data
				for (k=0;k<3;k++) {
					s32 index = mb_indices[j+k];
					if (index > numVertices) goto error;
					vertices[k] = btVector3(mb_vertices[index].Pos.X,mb_vertices[index].Pos.Y,mb_vertices[index].Pos.Z);
				}
				pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
			}
		}
		//not bothering with EVT_TANGENTS vertex type
	}

	return pTriMesh;
error:
	//should have better error handling than this
//!	OutputDebugString("Error in Irrlicht mesh\n");
	delete pTriMesh;
	return 0;
}
