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

#include <stdio.h>

#include "btFluidHfBuoyantShapeCollisionAlgorithm.h"
#include "btFluidHfBuoyantConvexShape.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "btFluidHf.h"

btFluidHfBuoyantShapeCollisionAlgorithm::btFluidHfBuoyantShapeCollisionAlgorithm(const btCollisionAlgorithmConstructionInfo& ci, 
										const btCollisionObjectWrapper* body0Wrap, const btCollisionObjectWrapper* body1Wrap, 
										btSimplexSolverInterface* simplexSolver, btConvexPenetrationDepthSolver* pdSolver)
: btCollisionAlgorithm(ci), m_convexConvexAlgorithm(NULL, ci, body0Wrap, body1Wrap, simplexSolver, pdSolver,0,0) 
{
#ifdef EXTEND_BT_FLUID_HF_BUOYANT_SHAPE_COLLISION
	m_algorithm = 0;
	
	//
	m_actualShape0 = body0Wrap->getCollisionShape();
	m_actualShape1 = body1Wrap->getCollisionShape();
	
	if(body0Wrap->getCollisionShape()->getShapeType() == HFFLUID_BUOYANT_CONVEX_SHAPE_PROXYTYPE)
	{
		const btFluidHfBuoyantConvexShape* tmpShape0 = static_cast<const btFluidHfBuoyantConvexShape*>( body0Wrap->getCollisionShape() );
		const btConvexShape* convexShape0 = tmpShape0->getConvexShape();
		
		m_actualShape0 = convexShape0;
	}
	if(body1Wrap->getCollisionShape()->getShapeType() == HFFLUID_BUOYANT_CONVEX_SHAPE_PROXYTYPE)
	{
		const btFluidHfBuoyantConvexShape* tmpShape1 = static_cast<const btFluidHfBuoyantConvexShape*>( body1Wrap->getCollisionShape() );
		const btConvexShape* convexShape1 = tmpShape1->getConvexShape();
		
		m_actualShape1 = convexShape1;
	}
#endif
}
btFluidHfBuoyantShapeCollisionAlgorithm::~btFluidHfBuoyantShapeCollisionAlgorithm()
{
#ifdef EXTEND_BT_FLUID_HF_BUOYANT_SHAPE_COLLISION
	if(m_algorithm)
	{
		m_algorithm->~btCollisionAlgorithm();
		m_dispatcher->freeCollisionAlgorithm(m_algorithm);
	}
#endif
}

void btFluidHfBuoyantShapeCollisionAlgorithm::processCollision(const btCollisionObjectWrapper* body0Wrap, const btCollisionObjectWrapper* body1Wrap,
															   const btDispatcherInfo& dispatchInfo, btManifoldResult* resultOut)
{
#ifndef EXTEND_BT_FLUID_HF_BUOYANT_SHAPE_COLLISION
	const btFluidHfBuoyantConvexShape* tmpShape0 = static_cast<const btFluidHfBuoyantConvexShape*>( body0Wrap->getCollisionShape() );
	const btFluidHfBuoyantConvexShape* tmpShape1 = static_cast<const btFluidHfBuoyantConvexShape*>( body1Wrap->getCollisionShape() );
	const btConvexShape* convexShape0 = tmpShape0->getConvexShape();
	const btConvexShape* convexShape1 = tmpShape1->getConvexShape();
	
	btCollisionObjectWrapper temp0Wrap( body0Wrap, convexShape0, body0Wrap->getCollisionObject(), body0Wrap->getWorldTransform() );
	btCollisionObjectWrapper temp1Wrap( body1Wrap, convexShape1, body1Wrap->getCollisionObject(), body1Wrap->getWorldTransform() );
	
	m_convexConvexAlgorithm.processCollision(&temp0Wrap, &temp1Wrap, dispatchInfo, resultOut);
	
#else
	btCollisionObjectWrapper temp0Wrap( body0Wrap, m_actualShape0, body0Wrap->getCollisionObject(), body0Wrap->getWorldTransform() );
	btCollisionObjectWrapper temp1Wrap( body1Wrap, m_actualShape1, body1Wrap->getCollisionObject(), body1Wrap->getWorldTransform() );
	
	//If btFluidHfBuoyantShape-btFluidHf collision is passed into this algorithm, findAlgorithm() will recurse endlessly
	btAssert( body0Wrap->getCollisionShape()->getShapeType() != HFFLUID_SHAPE_PROXYTYPE );
	btAssert( body1Wrap->getCollisionShape()->getShapeType() != HFFLUID_SHAPE_PROXYTYPE );
	if(!m_algorithm) m_algorithm = m_dispatcher->findAlgorithm(&temp0Wrap, &temp1Wrap);
	
	if(m_algorithm) m_algorithm->processCollision(&temp0Wrap, &temp1Wrap, dispatchInfo, resultOut);
#endif

	resultOut->setBody0Wrap(body0Wrap);
	resultOut->setBody1Wrap(body1Wrap);
}
