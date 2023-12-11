/*
Bullet-FLUIDS 
Copyright (c) 2012 Jackson Lee

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#include "btFluidSoftRigidDynamicsWorld.h"

#include "LinearMath/btIDebugDraw.h"

#include "BulletFluids/Sph/btFluidSph.h"
#include "BulletFluids/Sph/btFluidSphSolver.h"

#include "FluidSoftInteraction.h"

btFluidSoftRigidDynamicsWorld::btFluidSoftRigidDynamicsWorld(btDispatcher* dispatcher, btBroadphaseInterface* pairCache, 
													btConstraintSolver* constraintSolver, btCollisionConfiguration* collisionConfiguration, 
													btFluidSphSolver* fluidSolver, btSoftBodySolver* softBodySolver) 
: 	btSoftRigidDynamicsWorld(dispatcher, pairCache, constraintSolver, collisionConfiguration, softBodySolver), 
	m_fluidSolver(fluidSolver),
	m_internalFluidPreTickCallback(0), m_internalFluidPostTickCallback(0) {}
								
int btFluidSoftRigidDynamicsWorld::stepSimulation(btScalar timeStep, int maxSubSteps, btScalar fixedTimeStep)
{
	m_tempOverrideFluids.resize(0);
	m_tempDefaultFluids.resize(0);
	for(int i = 0; i < m_fluids.size(); ++i)
	{
		if( m_fluids[i]->getOverrideSolver() || m_fluids[i]->getOverrideParameters() ) m_tempOverrideFluids.push_back(m_fluids[i]);
		else m_tempDefaultFluids.push_back(m_fluids[i]);
	}

	//
	return btSoftRigidDynamicsWorld::stepSimulation(timeStep, maxSubSteps, fixedTimeStep);
}

void btFluidSoftRigidDynamicsWorld::addFluidSph(btFluidSph* fluid, short int collisionFilterGroup, short int collisionFilterMask)
{
	btAssert(fluid);
	btAssert( m_fluids.findLinearSearch(fluid) == m_fluids.size() );
	
	m_fluids.push_back(fluid);
	btCollisionWorld::addCollisionObject(fluid, collisionFilterGroup, collisionFilterMask);
}
void btFluidSoftRigidDynamicsWorld::removeFluidSph(btFluidSph* fluid)
{
	m_fluids.remove(fluid);  //Swaps elements if fluid is not the last
	btCollisionWorld::removeCollisionObject(fluid);
}
void btFluidSoftRigidDynamicsWorld::addCollisionObject(btCollisionObject* collisionObject, 
													short int collisionFilterGroup, short int collisionFilterMask)
{
	btFluidSph* fluid = btFluidSph::upcast(collisionObject);
	if(fluid) addFluidSph(fluid, collisionFilterGroup, collisionFilterMask);
	else btSoftRigidDynamicsWorld::addCollisionObject(collisionObject, collisionFilterGroup, collisionFilterMask);
}
void btFluidSoftRigidDynamicsWorld::removeCollisionObject(btCollisionObject* collisionObject)
{
	btFluidSph* fluid = btFluidSph::upcast(collisionObject);
	if(fluid) removeFluidSph(fluid);
	else btSoftRigidDynamicsWorld::removeCollisionObject(collisionObject);
}

void btFluidSoftRigidDynamicsWorld::debugDrawWorld()
{
	btSoftRigidDynamicsWorld::debugDrawWorld();
	
	btIDebugDraw* debugDrawer = getDebugDrawer();
	if(debugDrawer)
	{
		const btVector3 FLUID_VELOCITY_COLOR(1, 1, 1);
		const btVector3 FLUID_CONTACT_COLOR(1, 1, 0);
	
		for(int i = 0; i < m_fluids.size(); i++)
		{
			btFluidSph* fluid = m_fluids[i];
			const btFluidSphParametersLocal& FL = fluid->getLocalParameters();
			
			if(debugDrawer->getDebugMode() & btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE)
			{
				for(int n = 0; n < fluid->numParticles(); ++n)
				{
					const btVector3& position = fluid->getPosition(n);
					const btVector3& velocity = fluid->getVelocity(n);
					
					btVector3 lineStart = position;
					btVector3 lineEnd = position + ( velocity*btScalar(3.0) );
					debugDrawer->drawLine(lineStart, lineEnd, FLUID_VELOCITY_COLOR);
				}
			}
			
			if(debugDrawer->getDebugMode() & btIDebugDraw::DBG_DrawContactPoints)
			{
				const btAlignedObjectArray<btFluidSphRigidContactGroup>& contactGroups = fluid->getRigidContacts();
				for(int n = 0; n < contactGroups.size(); ++n)
				{
					const btFluidSphRigidContactGroup& contactGroup = contactGroups[n];
					for(int j = 0; j < contactGroup.m_contacts.size(); ++j)
					{
						const int CONTACT_LIFETIME = 0; //btFluidSph contacts are regenerated every frame
					
						const btFluidSphRigidContact& contact = contactGroup.m_contacts[j];
						debugDrawer->drawContactPoint(contact.m_hitPointWorldOnObject, contact.m_normalOnObject, contact.m_distance,
													CONTACT_LIFETIME, FLUID_CONTACT_COLOR);
					}
				}
			}
			
		}
	}
}

void btFluidSoftRigidDynamicsWorld::internalSingleStepSimulation(btScalar timeStep) 
{
	BT_PROFILE("FluidRigidWorld - singleStepSimulation()");
	
	if(m_internalFluidPreTickCallback) m_internalFluidPreTickCallback(this, timeStep);
	
	//	Temporary - allow m_fluidSolver to be 0 so that btFluidHfRigidDynamicsWorld
	//	does not require a btFluidSolverSph to be specified in its constructor.
	if(!m_fluidSolver)
	{
		btSoftRigidDynamicsWorld::internalSingleStepSimulation(timeStep);
		return;
	}
	
	for(int i = 0; i < m_fluids.size(); ++i) m_fluids[i]->internalClearRigidContacts();
	
	//btFluidSph-btRigidBody/btCollisionObject AABB intersections are 
	//detected here(not midphase/narrowphase), so calling removeMarkedParticles() 
	//below does not invalidate the collisions.
	btSoftRigidDynamicsWorld::internalSingleStepSimulation(timeStep);
	
	for(int i = 0; i < m_fluids.size(); ++i) m_fluids[i]->removeMarkedParticles();
	
	//SPH forces
	{
		int numDefaultSolverFluids = m_tempDefaultFluids.size();
		if(numDefaultSolverFluids)
		{
			m_fluidSolver->updateGridAndCalculateSphForces(m_globalParameters, &m_tempDefaultFluids[0], numDefaultSolverFluids);
		}
		
		for(int i = 0; i < m_tempOverrideFluids.size(); ++i)
		{
			btFluidSph* fluid = m_tempOverrideFluids[i];
		
			btFluidSphSolver* overrideSolver = fluid->getOverrideSolver();
			btFluidSphParametersGlobal* overrideParameters = fluid->getOverrideParameters();
			
			btFluidSphSolver* usedSolver = (overrideSolver) ? overrideSolver : m_fluidSolver;
			btFluidSphParametersGlobal* usedGlobalParameters = (overrideParameters) ? overrideParameters : &m_globalParameters;
			
			usedSolver->updateGridAndCalculateSphForces( *usedGlobalParameters, &fluid, 1 );
		}
	}
	
	btAlignedObjectArray<btSoftBody*> softBodies = getSoftBodyArray();
	FluidSoftInteractor::performCollisionDetectionAndResponse(m_globalParameters, m_fluids, softBodies, timeStep);
	
	//Detect and resolve collisions, integrate
	{
		const bool USE_IMPULSE_BOUNDARY = 1; 	//Penalty force otherwise
	
		for(int i = 0; i < m_fluids.size(); ++i) 
		{
			btFluidSph* fluid = m_fluids[i];
			const btFluidSphParametersLocal& FL = fluid->getLocalParameters();
			
			if(!USE_IMPULSE_BOUNDARY) 
			{
				m_fluidRigidCollisionDetector.performNarrowphase(m_dispatcher1, m_dispatchInfo, m_globalParameters, m_fluids[i]);
				m_fluidRigidConstraintSolver.resolveCollisionsForce(m_globalParameters, m_fluids[i]);
			}
			btFluidSphSolver::applyForcesSingleFluid(m_globalParameters, fluid);
			
			if(USE_IMPULSE_BOUNDARY) 
			{
				m_fluidRigidCollisionDetector.performNarrowphase(m_dispatcher1, m_dispatchInfo, m_globalParameters, m_fluids[i]);
				m_fluidRigidConstraintSolver.resolveCollisionsImpulse(m_globalParameters, m_fluids[i]);
			}
			btFluidSphSolver::integratePositionsSingleFluid( m_globalParameters, fluid->internalGetParticles() );
		}
	}
	
	if(m_internalFluidPostTickCallback) m_internalFluidPostTickCallback(this, timeStep);
}
