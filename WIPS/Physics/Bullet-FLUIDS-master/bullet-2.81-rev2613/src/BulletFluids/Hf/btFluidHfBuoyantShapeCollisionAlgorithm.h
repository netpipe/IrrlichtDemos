/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2009 Erwin Coumans  http://bulletphysics.com

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Experimental Buoyancy fluid demo written by John McCutchan
*/
//This is an altered source version based on the HeightFieldFluidDemo included with Bullet Physics 2.80(bullet-2.80-rev2531).

#ifndef BT_FLUID_HF_BUOYANT_SHAPE_COLLISION_ALGORITHM_H
#define BT_FLUID_HF_BUOYANT_SHAPE_COLLISION_ALGORITHM_H

#include "BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h"
#include "BulletCollision/CollisionDispatch/btCollisionCreateFunc.h"
#include "BulletCollision/NarrowPhaseCollision/btPersistentManifold.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/CollisionShapes/btTriangleCallback.h"
#include "BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.h"

#include "LinearMath/btVector3.h"
class btFluidHf;

class btConvexConvexAlgorithm;
class btConvexPenetrationDepthSolver;
class btSimplexSolverInterface;

///btFluidHfBuoyantShapeCollisionAlgorithm provides collision detection for btFluidHfBuoyantConvexShape
class btFluidHfBuoyantShapeCollisionAlgorithm : public btCollisionAlgorithm
{
//EXTEND_BT_FLUID_HF_BUOYANT_SHAPE_COLLISION extends this algorithm to handle all
//collisions involving btFluidHfBuoyantConvexShape, except for btFluidHfBuoyantConvexShape-btFluidHf pairs.
//
//If EXTEND_BT_FLUID_HF_BUOYANT_SHAPE_COLLISION is defined, avoid input of btFluidHf into this algorithm as
//btDispatcher::findAlgorithm() will recurse endlessly in btFluidHfBuoyantShapeCollisionAlgorithm::processCollision.
#define EXTEND_BT_FLUID_HF_BUOYANT_SHAPE_COLLISION
#ifdef EXTEND_BT_FLUID_HF_BUOYANT_SHAPE_COLLISION
	btCollisionAlgorithm* m_algorithm;
	
	const btCollisionShape* m_actualShape0;
	const btCollisionShape* m_actualShape1;
#endif

	btConvexConvexAlgorithm m_convexConvexAlgorithm;
	
public:
	btFluidHfBuoyantShapeCollisionAlgorithm(const btCollisionAlgorithmConstructionInfo& ci, 
											const btCollisionObjectWrapper* body0Wrap, const btCollisionObjectWrapper* body1Wrap, 
											btSimplexSolverInterface* simplexSolver, btConvexPenetrationDepthSolver* pdSolver);

	virtual ~btFluidHfBuoyantShapeCollisionAlgorithm();

	virtual void processCollision(const btCollisionObjectWrapper* body0Wrap, const btCollisionObjectWrapper* body1Wrap,
								  const btDispatcherInfo& dispatchInfo, btManifoldResult* resultOut);
								  
	virtual btScalar calculateTimeOfImpact(btCollisionObject* body0, btCollisionObject* body1, 
											const btDispatcherInfo& dispatchInfo, btManifoldResult* resultOut) 
	{ 
		btAssert(0);	//Not implemented
		return btScalar(1.0); 
	}

	virtual	void getAllContactManifolds(btManifoldArray& manifoldArray)
	{
	#ifndef EXTEND_BT_FLUID_HF_BUOYANT_SHAPE_COLLISION
		m_convexConvexAlgorithm.getAllContactManifolds(manifoldArray);
	#else
		if(m_algorithm) m_algorithm->getAllContactManifolds(manifoldArray);
	#endif
	}


	struct CreateFunc : public btCollisionAlgorithmCreateFunc
	{
		btConvexPenetrationDepthSolver* m_pdSolver;
		btSimplexSolverInterface* m_simplexSolver;
		
		CreateFunc(btSimplexSolverInterface* simplexSolver, btConvexPenetrationDepthSolver* pdSolver)
		: m_simplexSolver(simplexSolver), m_pdSolver(pdSolver) {}
		
		virtual ~CreateFunc() {}
		virtual	btCollisionAlgorithm* CreateCollisionAlgorithm(btCollisionAlgorithmConstructionInfo& ci,
																const btCollisionObjectWrapper* body0Wrap, 
																const btCollisionObjectWrapper* body1Wrap)
		{
			void* mem = ci.m_dispatcher1->allocateCollisionAlgorithm( sizeof(btFluidHfBuoyantShapeCollisionAlgorithm) );
			return new(mem) btFluidHfBuoyantShapeCollisionAlgorithm(ci, body0Wrap, body1Wrap, m_simplexSolver, m_pdSolver);
		}
	};
};

#endif //BT_FLUID_HF_BUOYANT_SHAPE_COLLISION_ALGORITHM_H
