#ifndef RAGDOLL_H_INCLUDED
#define RAGDOLL_H_INCLUDED

#include <iostream>

#include "LinearMath/btAlignedObjectArray.h"
#include "btBulletDynamicsCommon.h"
#include <irrlicht.h>

using namespace irr;

class RagDoll
{
public:
	enum
	{
		BODYPART_PELVIS = 0,
		BODYPART_SPINE,
		BODYPART_HEAD,

		BODYPART_LEFT_UPPER_LEG,
		BODYPART_LEFT_LOWER_LEG,

		BODYPART_RIGHT_UPPER_LEG,
		BODYPART_RIGHT_LOWER_LEG,

		BODYPART_LEFT_UPPER_ARM,
		BODYPART_LEFT_LOWER_ARM,

		BODYPART_RIGHT_UPPER_ARM,
		BODYPART_RIGHT_LOWER_ARM,

		BODYPART_COUNT
	};

	enum
	{
		JOINT_PELVIS_SPINE = 0,
		JOINT_SPINE_HEAD,

		JOINT_LEFT_HIP,
		JOINT_LEFT_KNEE,

		JOINT_RIGHT_HIP,
		JOINT_RIGHT_KNEE,

		JOINT_LEFT_SHOULDER,
		JOINT_LEFT_ELBOW,

		JOINT_RIGHT_SHOULDER,
		JOINT_RIGHT_ELBOW,

		JOINT_COUNT
	};

	btDynamicsWorld* m_ownerWorld;
	btCollisionShape* m_shapes[BODYPART_COUNT];
	btRigidBody* m_bodies[BODYPART_COUNT];
	scene::IMeshSceneNode *v_Nodes[BODYPART_COUNT];
	btTypedConstraint* m_joints[JOINT_COUNT];

public:

	static btRigidBody* localCreateRigidBody (btDynamicsWorld *world, btScalar mass, const btTransform& startTransform, btCollisionShape* shape);
	static void QuaternionToEuler(const btQuaternion &TQuat, btVector3 &TEuler);

	RagDoll (btDynamicsWorld* ownerWorld, scene::ISceneManager *smgr,
				const btVector3& positionOffset,
				btScalar scale_ragdoll = btScalar(1.0));

	~RagDoll ();

	void Update();
};



#endif // RAGDOLL_H_INCLUDED
