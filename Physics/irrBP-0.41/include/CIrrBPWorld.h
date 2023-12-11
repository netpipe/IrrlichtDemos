#ifndef _CBULLETWORLD_H
#define _CBULLETWORLD_H

#define IrrBP_MAJOR_VERSION 0
#define IrrBP_MINOR_VERSION 3
#define IrrBP_REVISION_VERSION 0

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "CIrrBPCollisionObject.h"
#include "body/CIrrBPRigidBody.h"
#include "animator/CIrrBPAnimator.h"
#include "constraint/CIrrBPConstraint.h"
#include "softbody/CIrrBPSoftBody.h"
#include "actions/CIrrBPActionInterface.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "CIrrBPDebugDrawer.h"

#include <irrlicht.h>
#include "convert.h"
#include "types.h"
#include <iostream>



class CIrrBPWorld
{
public:

	 /*!
        Constructor.
        @param device A pointer to a Irrlicht's device
        @param Gravity World's Gravity .
		@param multithread Uses Multithreaded bullet
		@param maxtasks If multithread = true, then this value will set the nr of tasks to use bullet in multithread mode.
    */
	CIrrBPWorld(irr::IrrlichtDevice *device,const irr::core::vector3df & Gravity, bool multithread=false, int maxtasks=4);

	~CIrrBPWorld();

	/*!
		Steps the simulation.
		It must be called each frame loop to step the bullet' simulation.
	*/
    void stepSimulation();

	/*!
		Adds a rigid Body to the world.
		@param body A pointer to the body that needs to be added
	*/
	void addRigidBody(CIrrBPRigidBody * body);

	/*!
		Adds a constraint to the world.
		@param constraint A pointer to the constraint that needs to be added
	*/
	void addRigidBodyConstraint(CIrrBPConstraint * constraint);

	/*!
		Adds a constraint to the world.
		@param constraint A pointer to the constraint that needs to be removed
	*/
	void removeRigidBodyConstraint(CIrrBPConstraint * constraint);

	/*!
		Removes a rigid Body from the world.
		Please note that the body's Scene Node won't be dropped.
		@param body A pointer to the body that needs to be deleted.
		Only left for backwards compatibility. Use removeCollisionObject instead.
	*/
	void removeRigidBody(CIrrBPRigidBody *body);

	/*!
		Adds a soft Body to the world.
		@param sbody A pointer to the body that needs to be added
	*/
	void addSoftBody(CIrrBPSoftBody * sbody);

	/*!
		Removes a collision object to the world.
		@param cobj A pointer to the object that needs to be removed
	*/
	void removeCollisionObject(CIrrBPCollisionObject * cobj);

	/*!
		Adds an 'unknown' object to the world.
		You can use that instead of addRigidBody, it will know the object for you.
		@param cobj A pointer to the object that needs to be added
	*/
	void addCollisionObject(CIrrBPCollisionObject * cobj);

	/*!
		Adds an action interface to the world
		@param action A pointer to the object that needs to be added
	*/
	void addAction(CIrrBPActionInterface * action);

	/*!
		Removes an action interface to the world
		@param action A pointer to the object that needs to be removed
	*/
	void removeAction(CIrrBPActionInterface * action);

	/*!
		Gets a Body from a id.
		@param id The id to search for
		@return Pointer to the first body with this id. Returns NULL if no bodies couldn't be found.
	*/
	CIrrBPCollisionObject * getBodyFromId(irr::s32 id);

	/*!
		Gets a Body from a unique id.
		@param id The unique id to search for
		@return Pointer to the first body with this id. Returns NULL if no bodies couldn't be found.
	*/
	CIrrBPCollisionObject * getBodyFromUId(irr::u32 uid);


	/*!
		Gets a Body from a name.
		@param name The name to search for
		@return Pointer to the first body with this name. Returns NULL if no bodies couldn't be found.
	*/
	CIrrBPCollisionObject * getBodyFromName(irr::c8* name);

	/*!
		Gets a Action from a id.
		@param id The id to search for
		@return Pointer to the first action with this id. Returns NULL if no bodies couldn't be found.
	*/
	CIrrBPActionInterface * getActionFromId(irr::s32 id);
	
	/*!
		Gets a Action from a unique id.
		@param id The id to search for
		@return Pointer to the first action with this id. Returns NULL if no bodies couldn't be found.
	*/
	CIrrBPActionInterface * getActionFromUId(irr::u32 uid);

	/*!
		Gets a Action from a name.
		@param id The id to search for
		@return Pointer to the first action with this id. Returns NULL if no bodies couldn't be found.
	*/
	CIrrBPActionInterface * getActionFromName(irr::c8* name);

	/*!
		Verifies if a body is colliding or not.
		@param body body to verify
		@return body colliding status.
	*/
	bool isBodyColliding(CIrrBPCollisionObject *body);

	/*!
		Verifies if a body is colliding or not.
		This function is more deeper than isBodyColliding() because it will give also the contact point
		@param body body to verify
		@param dCP pointer to the contactPoint destination.
		@return body colliding status.
	*/
	bool getBodyCollidingPoint(CIrrBPCollisionObject *body, contactPoint * dCP);

	/*!
		Checks if a pairs of bodies is colliding or not and returns the contact point.
		@param body1 body to verify
		@param body2 second body to verify
		@param dCP pointer to the contactPoint destination (NULL if you don't need contactPoint information).
		@param returnSecondPoint if dCP is passed, setting this parameter to TRUE will return the second body's relative contact point.
		@return body colliding status.
	*/
	bool isPairColliding(CIrrBPCollisionObject *body1,CIrrBPCollisionObject *body2, contactPoint * dCP=NULL, bool returnSecondPoint=false);

	/*!
		Drop the world pointer and all his child.
		Please note that all registered rigid bodies pointers, will be destroyed.
	*/
	void drop() { delete this;}

	void setGravity(const irr::core::vector3df & newGravity) { World->setGravity(bullet::irrVectorToBulletVector(newGravity));}

	/*!
		Only for internal or expert use.
		@return a pointer to the bullet' world object
	*/
	btSoftRigidDynamicsWorld* getBulletWorldPtr(){return World;}

	/*!
		true if world is  going to close
	*/
	bool isClosing;

	/*!
		Creates a new bullet' debug drawer.
		This can be useful for debugging bounding boxes, constraints and contact points
	*/
	void createDebugDrawer();

	/*!
		Steps the debug drawer.
		Must be called between driver->beginScene() and driver->endScene() to update debug' datas.
	*/
	void stepDebugDrawer();

	/*!
		Sets the debug-drawer flags
		You can use the IBP_DEBUG_FLAGS defined in types.h
	*/
	void setDebugDrawerFlags(int flags);

	btSoftBodyWorldInfo & getSoftBodyWorldInfo();

	irr::IrrlichtDevice * getIrrDevice() {return device;}

	/*!
		Sets your own value of ERP
		@param erp new erp value
	*/
	void setERP(irr::f32 erp);

	/*!
		Sets your own value of ERP2
		@param erp2 new erp2 value
	*/
	void setERP2(irr::f32 erp2);

	/*!
		Sets your own value of CFM
		@param cfm new cfm value
	*/
	void setCFM(irr::f32 cfm);

	/*!
	   Sets your own time step. Use this function only if you know what you are doing.
	   If the timestep decreases, the simulation resolution increases.
	   Using a dynamic timestep can be useless, and your program can have an undefined behavior

	   @param step new time step
	*/
	void setTimeStep(irr::f32 step) {timestep = step;}

	/*!
		Sets your own max sub steps value. You need to fix the value of max sub steps and time step in order to meet the equation above:
		timeStep [FPS/1000] < maxSubSteps * fixedTimeStep.
		This is strictly necessary in order to make the simulation deterministic.
		Please use only positive parameters. Otherwise the simulation can have an unexpected behavior.
		If you don't know how to use this function, you can use the autoMaxSubSteps function.

		@param ssteps new max sub steps number
	*/
	void setMaxSubSteps(irr::s32 ssteps){maxSubSteps = ssteps;}

	/*!
		Auto-set the maxsubsteps in order to meet the equation timeStep < maxSubSteps * fixedTimeStep.
		You must recall this function after setting a new time step.
		In order to meet the equation above, you need to "cap" your minimun FPS to a value.

		@param minFPS FPS min cap value
	*/
	void autoMaxSubSteps(int minFPS);

	/*!
		Performs a raycast test through all bodies in the world.
		@param from line start point
		@param to line end point
		@param points contact points array (NULL if you don't need to receive the contact points)
		@return returns true if the collision occur.
	*/
	bool rayCastTest(irr::core::vector3df from,irr::core::vector3df to, irr::core::array<contactPoint> * points=NULL);

	/*!
		Performs a raycast test through all bodies in the world, and return only the closest hit.
		@param from line start point
		@param to line end point
		@param point contact point (NULL if you don't need to receive the contact points)
		@return returns true if the collision occur.
	*/
	bool rayCastClosestHitTest(irr::core::vector3df from,irr::core::vector3df to, contactPoint * point=NULL);

	const irr::core::array<CIrrBPCollisionObject *> & getCollisionObjectsList(){return collisionObj;}
	/*!
		Cleans the bullet world: removes all bodies and joints
	*/
	void clear();
private:
	CIrrBPCollisionObject * getObjectByPointer(btCollisionObject* cObj);
	inline void updateObjects();

	btSoftBodyRigidBodyCollisionConfiguration* CollisionConfiguration;
	btSoftRigidDynamicsWorld * World;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* pairCache;
    btConstraintSolver*	constraintSolver;

	irr::core::array<CIrrBPCollisionObject *> collisionObj;
	irr::core::array<CIrrBPConstraint*> rigidBodiesConst;
	irr::core::array<CIrrBPActionInterface*> actionObj;
	irr::ITimer* irrTimer;
	irr::u32 TimeStamp;
    irr::u32 DeltaTime;

	btVector3 Gravity;
	irr::IrrlichtDevice *device;
	irr::video::IVideoDriver* driver;
	irr::scene::IMeshSceneNode* worldNode;

	CIrrBPDebugDrawer * dDrawer;
	irr::video::SMaterial mat;

	btSoftBodyWorldInfo m_worldInfo;

	irr::f32 timestep;
	int maxSubSteps;

	class	btThreadSupportInterface*		m_threadSupportCollision;
	class	btThreadSupportInterface*		m_threadSupportSolver;
};

#endif
