/* Copyright (c) <2003-2011> <Julio Jerez, Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 
* 3. This notice may not be removed or altered from any source distribution.
*/

// cuda.cpp : Defines the exported functions for the DLL application.
//

#include <cudaStdafx.h>
#include <dgCudaSolver.h>
#include <cuda.h>
#include "dgWorld.h"

#include "dgBody.h"
#include "dgWorld.h"
#include "dgConstraint.h"
#include "dgCudaSolverKernels.h"
#include "driver_types.h"
#include "cuda_runtime_api.h"


#define	DG_SOLVER_MAX_ERROR					(DG_FREEZE_MAG * dgFloat32 (0.5f))
#define	DG_FREEZZING_VELOCITY_DRAG			dgFloat32 (0.9f)

#ifdef DG_BUILD_SIMD_CODE
#define DG_SIMD_WORD_SIZE					dgInt32 (sizeof (simd_type) / sizeof (dgFloat32))
#else
#define DG_SIMD_WORD_SIZE					dgInt32 (sizeof (dgVector) / sizeof (dgFloat32))
#endif


#define REST_RELATIVE_VELOCITY				dgFloat32 (1.0e-3f)
#define MAX_DYNAMIC_FRICTION_SPEED			dgFloat32 (0.3f)
#define DG_CUDA_BASE_ITERATION_COUNT		3


#define DG_CUDA_THREAD_SYNCRONIZE				\
{												\
	cudaError_t error;							\
	error = cudaThreadSynchronize();			\
	_ASSERTE (error == cudaSuccess);			\
}



static inline dgInt32 CompareJoints (const dgInt32* const jointA, const dgInt32* const jointB, void* jointArray)
{
	dgInt32 countA;
	dgInt32 countB;
	dgCudaJointGraph::dgJointInfo* const infoArray = (dgCudaJointGraph::dgJointInfo*) jointArray;

	countA = infoArray[jointA[0]].m_joint->GetMaxDOF();
	countB = infoArray[jointB[0]].m_joint->GetMaxDOF();

	if (countA < countB) {
		return 1;
	}
	if (countA > countB) {
		return -1;
	}
	return 0;
}



dgCudaSolver::dgCudaSolver ()
{
//	InitCudaDevice (device, m_name);

	m_bodyArray.Init (512);
	m_jointGraph.Init (1024);
	m_jointSlacks.Init (1024);
	m_activeIslands.Init (512);
	m_contactMemory.Init (2048);
	m_jacobianMemory.Init (4096);
}



dgCudaSolver::~dgCudaSolver ()
{
	m_bodyArray.Free();
	m_jointGraph.Free();
	m_jointSlacks.Free();
	m_contactMemory.Free();
	m_jacobianMemory.Free();
	m_activeIslands.Free();
}


void dgCudaSolver::UpdateDynamics (dgWorld* world, dgFloat32 timestep)
{
world->m_dynamicSolver.UpdateDynamics (world, 0, timestep);
return;

	dgUnsigned32 lru;
	dgUnsigned32 ticks;
	dgUnsigned32 updateTime;
	dgUnsigned32 dynamicsTime;
	

	updateTime = world->m_getPerformanceCount();

	m_world = world;

	m_world->m_dynamicsLru = m_world->m_dynamicsLru + 2;
	m_markLru = m_world->m_dynamicsLru;
	lru = m_markLru - 1;

	dgBodyMasterList& me = *m_world;
	_ASSERTE (me.GetFirst()->GetInfo().GetBody() == m_world->m_sentionelBody);

	m_activeIslands.Clear();
	m_bodyArray.m_count = 0;
	m_jointGraph.m_count = 0;
	m_jacobianMemory.m_count = 0;
	for (dgBodyMasterList::dgListNode* node = me.GetLast(); node; node = node->GetPrev()) {
		dgBody *body;	

		const dgBodyMasterListRow& graphNode = node->GetInfo();
		body = graphNode.GetBody();	
		if (body->m_invMass.m_w == dgFloat32(0.0f)) {
			#ifdef _DEBUG
			for (; node; node = node->GetPrev()) {
				_ASSERTE (node->GetInfo().GetBody()->m_invMass.m_w == dgFloat32(0.0f));
			}
			#endif
			break;
		}

		if (body->m_dynamicsLru < lru) {
			if (!(body->m_freeze | body->m_spawnnedFromCallback | body->m_sleeping | !body->m_isInWorld)) {
				SpanningTree (body);
			}
		}
		body->m_spawnnedFromCallback = false;
	}

	ScanBodyGraph ();


	dynamicsTime = m_world->m_getPerformanceCount();
	m_world->m_perfomanceCounters[0][m_dynamicsBuildSpanningTreeTicks] = dynamicsTime - updateTime;
	
	CalculateInertiaMatricesAndBodyKineticForces (m_bodyArray);	
	BuildJacobianMatrix (timestep);
	CalculateReactionsForces (timestep, DG_SOLVER_MAX_ERROR);
	IntegrateArray (timestep, DG_SOLVER_MAX_ERROR);

	ticks = m_world->m_getPerformanceCount();
	m_world->m_perfomanceCounters[0][m_dynamicsSolveSpanningTreeTicks] = ticks - dynamicsTime;
	m_world->m_perfomanceCounters[0][m_dynamicsTicks] = ticks - updateTime;
}


void dgCudaSolver::BuildJacobianMatrix (dgFloat32 timestep)
{
	dgInt32 rowCount;
	dgInt32 maxContactCount;

	rowCount = 0;
	maxContactCount = 0;
	dgCudaJointGraph::dgJointInfo* const constraintArray = &m_jointGraph.m_sysConstraintArray[0];

	for (dgInt32 i = 0; i < m_jointGraph.m_count; i ++) {
		dgConstraint* constraint;
		
		dgCudaJointGraph::dgJointInfo& info = constraintArray[i];
		constraint = info.m_joint;

		info.m_contactStart = -1;
		if (constraint->GetId() == dgContactConstraintId) {
			dgInt32 count;
			const dgContact* const contactPoints = (dgContact*) constraint;

			count = contactPoints->GetCount();
			if ((maxContactCount + count) > m_contactMemory.m_maxCount) {
				m_contactMemory.Realloc(maxContactCount);
			}

			info.m_autoPairstart = rowCount;
			info.m_contactStart = maxContactCount;
			info.m_autoPairActiveCount = count; 
			info.m_autoPaircount = count;

			//info.m_m0 = (contactPoints->m_body0->m_invMass.m_w != dgFloat32(0.0f)) ? contactPoints->m_body0->m_index: 0;
			//info.m_m1 = (contactPoints->m_body1->m_invMass.m_w != dgFloat32(0.0f)) ? contactPoints->m_body1->m_index: 0;

			info.m_m0 = contactPoints->m_body0->m_index;
			info.m_m1 = contactPoints->m_body1->m_index;

			rowCount += count * 3;
			rowCount = (rowCount & (DG_SIMD_WORD_SIZE - 1)) ? ((rowCount & (-DG_SIMD_WORD_SIZE)) + DG_SIMD_WORD_SIZE) : rowCount;
			_ASSERTE ((rowCount & (DG_SIMD_WORD_SIZE - 1)) == 0);

			_ASSERTE ((sizeof (dgCudaContactMemory::dgContact) % (16 * sizeof (dgFloat32))) == 0);
			for (dgList<dgContactMaterial>::dgListNode* node = contactPoints->GetFirst(); node; node = node->GetNext()) {
				dgContactMaterial& contact = node->GetInfo(); 
				dgCudaContactMemory::dgContact& contactInfo = m_contactMemory.m_sysContacts[maxContactCount];
				(simd_type&)contactInfo.m_point = (simd_type&)contact.m_point;
				(simd_type&)contactInfo.m_normal = (simd_type&)contact.m_normal;
				(simd_type&)contactInfo.m_dir0 = (simd_type&)contact.m_dir0;
				(simd_type&)contactInfo.m_dir1 = (simd_type&)contact.m_dir1;

				contactInfo.m_normal_Force = contact.m_normal_Force;
				contactInfo.m_dir0_Force = contact.m_dir0_Force;
				contactInfo.m_dir1_Force = contact.m_dir1_Force;
				contactInfo.m_softness = contact.m_softness;
				contactInfo.m_penetration = contact.m_penetration;
				contactInfo.m_restitution = contact.m_restitution;
				contactInfo.m_staticFriction0 = contact.m_staticFriction0;
				contactInfo.m_staticFriction1 = contact.m_staticFriction1;
				contactInfo.m_dynamicFriction0 = contact.m_dynamicFriction0;
				contactInfo.m_dynamicFriction1 = contact.m_dynamicFriction1;
				contactInfo.m_flags = contact.m_flags;

				contactInfo.m_normal_ForcePtr = &contact.m_normal_Force;
				contactInfo.m_dir0_ForcePtr = &contact.m_dir0_Force;
				contactInfo.m_dir1_ForcePtr = &contact.m_dir1_Force;
				
				maxContactCount ++;
			}
		}
	}
	m_contactMemory.m_count = maxContactCount;

	CalculateContactDerivatives (m_jointGraph, m_contactMemory, m_jacobianMemory, m_bodyArray, timestep);	

//	_ASSERTE (0);
/*
	for (dgInt32 i = 0; i < m_islands; i ++) {
		const dgCudaIsland* const island = &m_islandArray[i];
		if (island->m_hasUnilateralJoints) {
			rowCount = GetJacobialDerivatives (*island, false, rowCount, timestep);
		}
		rowCount = GetJacobialDerivatives (*island, true, rowCount, timestep);
	}
*/

	CalculateJacobianMatrix (m_jointGraph, m_bodyArray, m_jacobianMemory);
}







void dgCudaSolver::IntegrateArray (dgFloat32 timestep, dgFloat32 accelTolerance) const
{
	dgVector zero (dgFloat32 (0.0f), dgFloat32 (0.0f), dgFloat32 (0.0f), dgFloat32 (0.0f));

	dgBody* const* bodyArray = &m_activeIslands.m_bodyList.m_pool[0];
	for (dgInt32 j = 0; j < m_activeIslands.m_count; j ++) {
		dgInt32 base;
		dgInt32 count;
		dgInt32 isAutoSleep;
		dgInt32 sleepCounter;
		dgInt32 stackSleeping;
		dgFloat32 maxAccel;
		dgFloat32 maxAlpha;
		dgFloat32 maxSpeed;
		dgFloat32 maxOmega;
		dgFloat32 speedFreeze;
		dgFloat32 accelFreeze;

		isAutoSleep = true;
		stackSleeping = true;
		sleepCounter = 10000;
		maxAccel = dgFloat32 (0.0f);
		maxAlpha = dgFloat32 (0.0f);
		maxSpeed = dgFloat32 (0.0f);
		maxOmega = dgFloat32 (0.0f);
		speedFreeze = m_world->m_freezeSpeed2;
		accelFreeze = m_world->m_freezeAccel2;

		base = m_activeIslands.m_sysIsland[j].m_start;
		count = m_activeIslands.m_sysIsland[j].m_count;
		for (dgInt32 i = 0; i < count; i ++) {
			dgBody *body;	
	
			body = bodyArray[base + i];
			if (body->m_invMass.m_w && body->m_isInWorld) {
				dgInt32 equilibrium;
				dgFloat32 accel2;
				dgFloat32 alpha2;
				dgFloat32 speed2;
				dgFloat32 omega2;

				body->IntegrateVelocity(timestep);

				accel2 = body->m_accel % body->m_accel;
				alpha2 = body->m_alpha % body->m_alpha;
				speed2 = body->m_veloc % body->m_veloc;
				omega2 = body->m_omega % body->m_omega;

				maxAccel = GetMax (maxAccel, accel2);
				maxAlpha = GetMax (maxAlpha, alpha2);
				maxSpeed = GetMax (maxSpeed, speed2);
				maxOmega = GetMax (maxOmega, omega2);

				equilibrium = (accel2 < accelFreeze) && (alpha2 < accelFreeze) && (speed2 < speedFreeze) && (omega2 < speedFreeze);
				if (equilibrium) {
					body->m_veloc = body->m_veloc.Scale (DG_FREEZZING_VELOCITY_DRAG);
					body->m_omega = body->m_omega.Scale (DG_FREEZZING_VELOCITY_DRAG);
				}

				body->m_equilibrium = equilibrium;
				stackSleeping &= equilibrium;
				isAutoSleep &= body->m_autoSleep;

				sleepCounter = GetMin (sleepCounter, body->m_sleepingCounter);
			}
		}

		for (dgInt32 i = 0; i < count; i ++) {
			dgBody *body;	
			body = bodyArray[base + i];
			if (body->m_invMass.m_w && body->m_isInWorld) {
				body->UpdateMatrix (timestep, 0);
			}
		}

		if (isAutoSleep) {
			if (stackSleeping) {
				for (dgInt32 i = 0; i < count; i ++) {
					dgBody *body;	
//					body = bodyArray[i].m_body;
					body = bodyArray[base + i];
					body->m_netForce = zero;
					body->m_netTorque = zero;
					body->m_veloc = zero;
					body->m_omega = zero;
				}
			} else {
				dgInt32 sleepEntriesCount;
				sleepEntriesCount = (count > 1) ? DG_SLEEP_ENTRIES : DG_SLEEP_ENTRIES - 1;

				if ((maxAccel > m_world->m_sleepTable[sleepEntriesCount -1].m_maxAccel) ||
					(maxAlpha > m_world->m_sleepTable[sleepEntriesCount -1].m_maxAlpha) ||
					(maxSpeed > m_world->m_sleepTable[sleepEntriesCount -1].m_maxVeloc) ||
					(maxOmega > m_world->m_sleepTable[sleepEntriesCount -1].m_maxOmega)) {
						for (dgInt32 i = 0; i < count; i ++) {
							dgBody *body;	
							body = bodyArray[base + i];
							body->m_sleepingCounter = 0;
						}
				} else {
					dgInt32 timeScaleSleepCount;
					dgInt32 index = 0;

					for (dgInt32 i = 0; i < sleepEntriesCount; i ++) {
						if ((maxAccel <= m_world->m_sleepTable[i].m_maxAccel) &&
							(maxAlpha <= m_world->m_sleepTable[i].m_maxAlpha) &&
							(maxSpeed <= m_world->m_sleepTable[i].m_maxVeloc) &&
							(maxOmega <= m_world->m_sleepTable[i].m_maxOmega)) {
								index = i;
								break;
						}
					}

					timeScaleSleepCount = int (dgFloat32 (60.0) * sleepCounter * timestep);

					if (timeScaleSleepCount > m_world->m_sleepTable[index].m_steps) {
						for (dgInt32 i = 0; i < count; i ++) {
							dgBody *body;	
							body = bodyArray[base + i];
							body->m_netForce = zero;
							body->m_netTorque = zero;
							body->m_veloc = zero;
							body->m_omega = zero;
							body->m_equilibrium = true;
						}
					} else {
						sleepCounter ++;
						for (dgInt32 i = 0; i < count; i ++) {
							dgBody *body;	
//							body = bodyArray[i].m_body;
							body = bodyArray[base + i];
							body->m_sleepingCounter = sleepCounter;
						}
					}
				}
			}
		}
	}
}







void dgCudaSolver::CalculateReactionsForces (dgFloat32 dt, dgFloat32 tolerance)
{
	dgInt32 iterations;
	dgInt32 maxPasses;
	dgFloat32 firstPassCoef;
	dgFloat32 invStep;
	dgFloat32 timeStep;
	dgFloat32 invTimeStep;

	invStep = (dgFloat32 (1.0f) / dgFloat32 (LINEAR_SOLVER_SUB_STEPS));
	timeStep = dt * invStep;
	invTimeStep = dgFloat32 (LINEAR_SOLVER_SUB_STEPS) / dt;

	InitInternalForces (m_version, m_jointSlacks, m_jointGraph, m_bodyArray, m_jacobianMemory);


	firstPassCoef = dgFloat32 (0.0f);
	iterations = (m_world->m_solverMode >= 1) ? m_world->m_solverMode : 13;

	maxPasses = iterations + DG_CUDA_BASE_ITERATION_COUNT;
	for (dgInt32 step = 0; step < LINEAR_SOLVER_SUB_STEPS; step ++) {
		CalculateRowsAcceleration (m_jointGraph, m_bodyArray, m_jacobianMemory, timeStep, invTimeStep, firstPassCoef);
		firstPassCoef = dgFloat32 (1.0f);

		for (dgInt32 passes = 0; passes < maxPasses; passes ++) {
			CalculateJointForces (m_version, m_jointSlacks, m_jointGraph, m_bodyArray, m_jacobianMemory);
		}
		UpdateBodiesVelocities (m_bodyArray, timeStep); 
	}

	cudaError_t error;
	error =	cudaMemcpy (&m_jointGraph.m_sysConstraintArray[0], &m_jointGraph.m_devConstraintArray[0], m_jointGraph.m_count * sizeof (dgCudaJointGraph::dgJointInfo), cudaMemcpyDeviceToHost);
	_ASSERTE (error == cudaSuccess);

	error =	cudaMemcpy (&m_jacobianMemory.m_sysJacobian[0], &m_jacobianMemory.m_devJacobian[0], m_jacobianMemory.m_count * sizeof (dgCudaJacobianMemory::dgJacobianInfo), cudaMemcpyDeviceToHost);
	_ASSERTE (error == cudaSuccess);


	const dgCudaJacobianMemory::dgJacobianInfo* const jacobianRows = &m_jacobianMemory.m_sysJacobian[0];
	for (dgInt32 i = 0; i < m_jointGraph.m_count; i ++) {
		int count;
		int first;
		
		dgCudaJointGraph::dgJointInfo& jointInfo = m_jointGraph.m_sysConstraintArray[i];	
		count = jointInfo.m_autoPaircount;
		first = jointInfo.m_autoPairstart;
		for (dgInt32 j = 0; j < count; j ++) { 
			dgFloat32 val;
			dgFloat32* ptr;
			dgInt32 index;

			index = j + first;
			val = jacobianRows[index].m_force; 
			_ASSERTE (dgCheckFloat(val));
			ptr = (dgFloat32*) jacobianRows[index].m_jointFeebackForcePtr;
			ptr[0] = val;
		}


		if (jointInfo.m_joint->m_updaFeedbackCallback) {
			jointInfo.m_joint->m_updaFeedbackCallback (*jointInfo.m_joint, dt, 0);
		}
	}

	dgFloat32 maxAccNorm2;
	maxAccNorm2 = tolerance * tolerance;

	error = cudaMemcpy (&m_bodyArray.m_sysBody[0], &m_bodyArray.m_devBody[0], m_bodyArray.m_count * sizeof (dgCudaBody::dgBodyInfo), cudaMemcpyDeviceToHost);
	_ASSERTE (error == cudaSuccess);

	error = cudaMemcpy (&m_bodyArray.m_sysBodyWorkData[0], &m_bodyArray.m_devBodyWorkData[0], m_bodyArray.m_count * sizeof (dgCudaBody::dgBodyWorkData), cudaMemcpyDeviceToHost);
	_ASSERTE (error == cudaSuccess);


	dgCudaBody::dgBodyInfo* const bodyArray = &m_bodyArray.m_sysBody[0];
	dgCudaBody::dgBodyWorkData* const velocForce = &m_bodyArray.m_sysBodyWorkData[0];
	dgVector zero (dgFloat32 (0.0f), dgFloat32 (0.0f), dgFloat32 (0.0f), dgFloat32 (0.0f));
	for (int i = 0; i < m_bodyArray.m_count; i ++) {
		dgBody* body;
		body = bodyArray[i].m_body;

		if (body->m_invMass[3] > dgFloat32 (0.0f)) {
			dgVector veloc (&velocForce[i].m_internalVeloc.m_linear[0]);
			dgVector omega (&velocForce[i].m_internalVeloc.m_angular[0]);

			veloc = veloc.Scale (invStep);
			omega = omega.Scale (invStep);

			dgVector& prevVeloc = (dgVector&)bodyArray[i].m_netForce;
			dgVector& prevOmega = (dgVector&)bodyArray[i].m_netForce;

			dgVector accel ((veloc - prevVeloc).Scale (invTimeStep));
			dgVector alpha ((omega - prevOmega).Scale (invTimeStep));

			if ((accel % accel) < maxAccNorm2) {
				accel = zero;
			}

			if ((alpha % alpha) < maxAccNorm2) {
				alpha = zero;
			}



			body->m_veloc = veloc;
			body->m_omega = omega;
			body->m_accel = accel;
			body->m_alpha = alpha;

			body->m_netForce = accel.Scale (body->m_mass[3]);
			alpha = body->m_matrix.UnrotateVector(alpha);
			body->m_netTorque = body->m_matrix.RotateVector (alpha.CompProduct(body->m_mass));
		}
	}
}




void dgCudaSolver::ScanBodyGraph ()
{
	while (m_jacobianMemory.m_count >= m_jacobianMemory.m_maxCount) {
		m_jacobianMemory.Realloc(0);
	}

	for (dgInt32 i = 0; i < sizeof (m_jointSlacks.m_slackArray) / sizeof (m_jointSlacks.m_slackArray[0]); i ++) {
		m_jointSlacks.m_slackArray[i].m_count = 0;
	}

	for (dgInt32 i = 0; i < m_jointGraph.m_count; i ++) {
		dgBody* body;
		dgConstraint* constraint;
		dgCudaJointGraph::dgJointInfo& jointInfo = m_jointGraph.m_sysConstraintArray[i];
		
		dgInt32 index = 0; 
		dgInt32 code = jointInfo.m_code;
		for (dgInt32 n = 1; n & code; n <<= 1) {
			index ++;
			_ASSERTE (index < 32);
		}

		dgInt32 entry = m_jointSlacks.m_slackArray[index].m_count;
		if (entry >= m_jointSlacks.m_slackArray[index].m_maxCount) {
			m_jointSlacks.m_slackArray[index].ReAlloc (entry);
		}

		m_jointSlacks.m_slackArray[index].m_sysJointSlack[entry] = i;
		m_jointSlacks.m_slackArray[index].m_count = entry + 1;

		code = 1 << index;
		constraint = jointInfo.m_joint;
		body = constraint->m_body0;
		if (body->m_invMass.m_w > dgFloat32 (0.0f)) {
			for (dgBodyMasterListRow::dgListNode* jointNode = body->m_masterNode->GetInfo().GetFirst(); jointNode; jointNode = jointNode->GetNext()) {
				dgConstraint* neiborgLink;
				dgBodyMasterListCell& cell = jointNode->GetInfo();

				neiborgLink = cell.m_joint;
				if (neiborgLink != constraint) {
					dgCudaJointGraph::dgJointInfo& info = m_jointGraph.m_sysConstraintArray[neiborgLink->m_index];
					info.m_code |= code;
				}
			}
		}

		body = constraint->m_body1;
		if (body->m_invMass.m_w > dgFloat32 (0.0f)) {
			for (dgBodyMasterListRow::dgListNode* jointNode = body->m_masterNode->GetInfo().GetFirst(); jointNode; jointNode = jointNode->GetNext()) {
				dgConstraint* neiborgLink;
				dgBodyMasterListCell& cell = jointNode->GetInfo();

				neiborgLink = cell.m_joint;
				if (neiborgLink != constraint) {
					dgCudaJointGraph::dgJointInfo& info = m_jointGraph.m_sysConstraintArray[neiborgLink->m_index];
					info.m_code |= code;
				}
			}
		}
	}

	for (dgInt32 i = 0; i < sizeof (m_jointSlacks.m_slackArray) / sizeof (m_jointSlacks.m_slackArray[0]); i ++) {
		if (m_jointSlacks.m_slackArray[i].m_count) {
			dgSort (&m_jointSlacks.m_slackArray[i].m_sysJointSlack[0], m_jointSlacks.m_slackArray[i].m_count, CompareJoints, &m_jointGraph.m_sysConstraintArray[0]); 
		}
	}


/*
	if (m_version >= 101) {
		dgInt32 count;
		count = m_jointSlacks.m_slackArray[0].m_count;
		for (dgInt32 i = 1; i < sizeof (m_jointSlacks.m_slackArray) / sizeof (m_jointSlacks.m_slackArray[0]); i ++) {
			if (m_jointSlacks.m_slackArray[i].m_count) {
				while ((count + m_jointSlacks.m_slackArray[i].m_count) >= m_jointSlacks.m_slackArray[0].m_maxCount) {
					m_jointSlacks.m_slackArray[0].ReAlloc (count);
				}
				memcpy (&m_jointSlacks.m_slackArray[0].m_sysJointSlack[count], &m_jointSlacks.m_slackArray[i].m_sysJointSlack[0], sizeof (int) * m_jointSlacks.m_slackArray[i].m_count);
				count += m_jointSlacks.m_slackArray[i].m_count;
			}
		}
		_ASSERTE (count == m_jointGraph.m_count);
	}


	dgInt32 roudnCount = (m_bodyArray.m_count + ((1<<DG_CLUSTER_TREAD_SIZE_BITS) - 1)) & (-(1<<DG_CLUSTER_TREAD_SIZE_BITS));
	for (dgInt32 i = m_bodyArray.m_count; i < roudnCount; i ++) {
		if (i >= m_bodyArray.m_maxCount) {
			m_bodyArray.ReAlloc (i);
		}

		dgCudaBody::dgBodyInfo& bodyInfo = m_bodyArray.m_sysBody[i];

		bodyInfo.m_body = sentinel;
		(simd_type&)bodyInfo.m_matrix[ 0] = (simd_type&)sentinel->m_matrix[0];
		(simd_type&)bodyInfo.m_matrix[ 4] = (simd_type&)sentinel->m_matrix[1];
		(simd_type&)bodyInfo.m_matrix[ 8] = (simd_type&)sentinel->m_matrix[2];
		(simd_type&)bodyInfo.m_matrix[12] = (simd_type&)sentinel->m_matrix[3];
		(simd_type&)bodyInfo.m_invInertia = (simd_type&)sentinel->m_invMass;
		(simd_type&)bodyInfo.m_veloc = (simd_type&)sentinel->m_veloc;
		(simd_type&)bodyInfo.m_omega = (simd_type&)sentinel->m_omega;
		(simd_type&)bodyInfo.m_accel = (simd_type&)sentinel->m_accel;
		(simd_type&)bodyInfo.m_alpha = (simd_type&)sentinel->m_alpha;
		(simd_type&)bodyInfo.m_dampCoef = (simd_type&)sentinel->m_dampCoef;
		(simd_type&)bodyInfo.m_globalCentreOfMass = (simd_type&)sentinel->m_globalCentreOfMass;
	}				


	roudnCount = (m_jointGraph.m_count + ((1<<DG_CLUSTER_TREAD_SIZE_BITS) - 1)) & (-(1<<DG_CLUSTER_TREAD_SIZE_BITS));
	for (dgInt32 i = m_jointGraph.m_count; i < roudnCount; i ++) {
		if (m_jointGraph.m_count >= m_jointGraph.m_maxCount) {
				m_jointGraph.ReAlloc (i);
		}
		dgCudaJointGraph::dgJointInfo& info = m_jointGraph.m_sysConstraintArray[i];
	
		info.m_code = 0;
		info.m_autoPairstart = 0;
		info.m_autoPaircount = 0;
		info.m_autoPairActiveCount= 0;
		info.m_m0 = 0;
		info.m_m1 = 0;
		info.m_code = 0;
		info.m_contactStart = 0;
		info.m_joint = NULL;
	}
*/
}



void dgCudaSolver::SpanningTree (dgBody* rootBody)
{
	dgInt32 isInWorld;
	dgInt32 bodyCount;
	dgInt32 baseJointCount;
	dgInt32 bodyIslandStart;
	dgInt32 isInEquilibrium;
	dgInt32 hasUnilateralJoints;
	dgInt32 isContinueCollisionIsland;
	dgFloat32 haviestMass;
	dgUnsigned32 lruMark;
	dgBody* heaviestBody;

	bodyCount = 0;
	isInWorld = 0;
	isInEquilibrium = 1;
	lruMark = m_markLru - 1;
	hasUnilateralJoints = 0;
	isContinueCollisionIsland = 0;

	heaviestBody = NULL;
	haviestMass = dgFloat32 (0.0f);

	dgQueue<dgBody*> queue ((dgBody**) m_world->m_pairMemoryBuffer, (m_world->m_pairMemoryBufferSizeInBytes>>1)/sizeof (void*));
	dgQueue<dgBody*> activeQueue (&queue.m_pool[queue.m_mod], queue.m_mod);
	
	bodyIslandStart = m_activeIslands.m_bodyList.m_count;

	baseJointCount = m_jointGraph.m_count;

	queue.Insert (rootBody);
	rootBody->m_dynamicsLru = lruMark;
	while (!queue.IsEmpty()) {
		dgInt32 count;
		dgInt32 index;
		count = queue.m_firstIndex - queue.m_lastIndex;
		if (count < 0) {
			_ASSERTE (0);
			count += queue.m_mod;
		}

		index = queue.m_lastIndex;
		queue.Reset ();
		for (dgInt32 j = 0; j < count; j ++) {
			dgBody* srcBody;
			srcBody = queue.m_pool[index];
			_ASSERTE (srcBody);
			_ASSERTE (srcBody->m_invMass.m_w > dgFloat32 (0.0f));
			_ASSERTE (srcBody->m_dynamicsLru == lruMark);
			_ASSERTE (srcBody->m_masterNode);

			if (srcBody->m_mass.m_w > haviestMass) {
				haviestMass = srcBody->m_mass.m_w;
				heaviestBody = srcBody;
			}

			isInWorld |= srcBody->m_isInWorld;
			isInEquilibrium &= (srcBody->m_equilibrium & srcBody->m_autoSleep);
			srcBody->m_sleeping = false;
			isContinueCollisionIsland |= srcBody->m_solverInContinueCollision;

			if ((bodyIslandStart + bodyCount) > m_activeIslands.m_bodyList.m_maxCount) {
				m_activeIslands.m_bodyList.Realloc (bodyIslandStart + bodyCount);
			}
			m_activeIslands.m_bodyList.m_pool[bodyIslandStart + bodyCount] = srcBody;
			bodyCount ++;
			_ASSERTE (bodyCount < queue.m_mod);

			for (dgBodyMasterListRow::dgListNode* jointNode = srcBody->m_masterNode->GetInfo().GetFirst(); jointNode; jointNode = jointNode->GetNext()) {
				dgBody* body;
				dgBodyMasterListCell& cell = jointNode->GetInfo();

				body = cell.m_bodyNode;
				if (body->m_dynamicsLru != lruMark) {
					if (body->m_invMass.m_w > dgFloat32 (0.0f)) {
						queue.Insert (body);
						body->m_dynamicsLru = lruMark;
					} else {
						dgConstraint* constraint;
						activeQueue.Insert (srcBody);
						_ASSERTE (activeQueue.m_lastIndex < activeQueue.m_firstIndex);
					
						constraint = cell.m_joint;
						if (dgInt32 (constraint->m_dynamicsLru) != m_markLru) {
							dgInt32 dof;
						
							constraint->m_dynamicsLru = m_markLru;
							if (m_jointGraph.m_count >= m_jointGraph.m_maxCount) {
								m_jointGraph.ReAlloc (m_jointGraph.m_count);
							}

							if (constraint->m_isUnilateral)	{
								hasUnilateralJoints = 1;
								_ASSERTE ((constraint->m_body0 == m_world->m_sentionelBody) || (constraint->m_body1 == m_world->m_sentionelBody));
							}			

							constraint->m_index = m_jointGraph.m_count;
							m_jointGraph.m_sysConstraintArray[m_jointGraph.m_count].m_code = 0;
							m_jointGraph.m_sysConstraintArray[m_jointGraph.m_count].m_joint = constraint;
							m_jointGraph.m_count ++;

							_ASSERTE (constraint->m_body0);
							_ASSERTE (constraint->m_body1);

							dof = (constraint->m_maxDOF & (DG_SIMD_WORD_SIZE - 1)) ? ((constraint->m_maxDOF & (-DG_SIMD_WORD_SIZE)) + DG_SIMD_WORD_SIZE) : constraint->m_maxDOF;
							m_jacobianMemory.m_count += dof;
						}
					}

				} else if (body->m_invMass.m_w == dgFloat32 (0.0f)) { 
					dgConstraint* constraint;
					activeQueue.Insert (srcBody);
					_ASSERTE (activeQueue.m_lastIndex < activeQueue.m_firstIndex);

					constraint = cell.m_joint;
					if (dgInt32 (constraint->m_dynamicsLru) != m_markLru) {
						dgInt32 dof;
					
						constraint->m_dynamicsLru = m_markLru;
						if (m_jointGraph.m_count >= m_jointGraph.m_maxCount) {
							m_jointGraph.ReAlloc (m_jointGraph.m_count);
						}

						if (constraint->m_isUnilateral)	{
							hasUnilateralJoints = 1;
							_ASSERTE ((constraint->m_body0 == m_world->m_sentionelBody) || (constraint->m_body1 == m_world->m_sentionelBody));
						}			

						constraint->m_index = m_jointGraph.m_count;
						m_jointGraph.m_sysConstraintArray[m_jointGraph.m_count].m_code = 0;
						m_jointGraph.m_sysConstraintArray[m_jointGraph.m_count].m_joint = constraint;
						m_jointGraph.m_count ++;

						_ASSERTE (constraint->m_body0);
						_ASSERTE (constraint->m_body1);

						dof = (constraint->m_maxDOF & (DG_SIMD_WORD_SIZE - 1)) ? ((constraint->m_maxDOF & (-DG_SIMD_WORD_SIZE)) + DG_SIMD_WORD_SIZE) : constraint->m_maxDOF;
						m_jacobianMemory.m_count += dof;
					}
				}
			}

			index ++;
			if (index >= queue.m_mod) {
				_ASSERTE (0);
				index = 0;
			}
		}
	}

	if (isInEquilibrium | !isInWorld) {
		m_jointGraph.m_count = baseJointCount;
		for (dgInt32 i = 0; i < bodyCount; i ++) {
			dgBody* body;
			body = m_activeIslands.m_bodyList.m_pool[bodyIslandStart + i];
			body->m_dynamicsLru = m_markLru;
			body->m_sleeping = true;
		}
	} else {
		if (m_world->m_islandUpdate) {
			dgIslandCallbackStruct record;
			record.m_world = m_world;
			record.m_count = bodyCount;
			record.m_strideInByte = sizeof (dgBody*);
			record.m_bodyArray = &m_activeIslands.m_bodyList.m_pool[bodyIslandStart];
			if (!m_world->m_islandUpdate (&record, bodyCount)) {
				m_jointGraph.m_count = baseJointCount;
				for (dgInt32 i = 0; i < bodyCount; i ++) {
					dgBody* body;
					body = m_activeIslands.m_bodyList.m_pool[bodyIslandStart + i];
					body->m_dynamicsLru = m_markLru;
					body->m_sleeping = true;
				}
			}
		}

		if (m_activeIslands.m_count >= m_activeIslands.m_maxCount) {
			m_activeIslands.Realloc(m_activeIslands.m_count);
		}

		m_activeIslands.m_sysIsland[m_activeIslands.m_count].m_start = bodyIslandStart;
		m_activeIslands.m_sysIsland[m_activeIslands.m_count].m_count = bodyCount;
		m_activeIslands.m_bodyList.m_count += bodyCount;
		m_activeIslands.m_count ++;


		
		lruMark = m_markLru;
		hasUnilateralJoints = 0;

		if (activeQueue.m_firstIndex != activeQueue.m_lastIndex) {
			dgInt32 count;
			_ASSERTE ((m_jointGraph.m_count - baseJointCount) == (activeQueue.m_firstIndex - activeQueue.m_lastIndex));
			
			count = activeQueue.m_firstIndex - activeQueue.m_lastIndex;
			for (dgInt32 i = 0; i < count; i ++) {
				dgBody* body;
				dgBody* staticBody;
				dgConstraint* constraint;

				body = activeQueue.m_pool[activeQueue.m_lastIndex + i];
				body->m_dynamicsLru = m_markLru;
				_ASSERTE (dgInt32 (body->m_dynamicsLru) == m_markLru);

				constraint = m_jointGraph.m_sysConstraintArray[baseJointCount + i].m_joint;
				staticBody = constraint->GetBody1();
				if (staticBody == body) {
					staticBody = constraint->GetBody0();
				}
					
				if (staticBody->m_dynamicsLru != lruMark) {
					dgInt32 bodyIndex;
					staticBody->m_dynamicsLru = lruMark;
					queue.Insert (staticBody);

					bodyIndex = m_bodyArray.m_count; 
					if (bodyIndex >= m_bodyArray.m_maxCount) {
						m_bodyArray.ReAlloc (bodyIndex);
					}

					staticBody->m_index = bodyIndex;
					dgCudaBody::dgBodyInfo& bodyInfo = m_bodyArray.m_sysBody[bodyIndex];
					bodyInfo.m_body = staticBody;
					(simd_type&)bodyInfo.m_matrix[ 0] = (simd_type&)staticBody->m_matrix[0];
					(simd_type&)bodyInfo.m_matrix[ 4] = (simd_type&)staticBody->m_matrix[1];
					(simd_type&)bodyInfo.m_matrix[ 8] = (simd_type&)staticBody->m_matrix[2];
					(simd_type&)bodyInfo.m_matrix[12] = (simd_type&)staticBody->m_matrix[3];
					(simd_type&)bodyInfo.m_invInertia = (simd_type&)staticBody->m_invMass;
					(simd_type&)bodyInfo.m_veloc = (simd_type&)staticBody->m_veloc;
					(simd_type&)bodyInfo.m_omega = (simd_type&)staticBody->m_omega;
					(simd_type&)bodyInfo.m_accel = (simd_type&)staticBody->m_accel;
					(simd_type&)bodyInfo.m_alpha = (simd_type&)staticBody->m_alpha;
					(simd_type&)bodyInfo.m_dampCoef = (simd_type&)staticBody->m_dampCoef;
					(simd_type&)bodyInfo.m_globalCentreOfMass = (simd_type&)staticBody->m_globalCentreOfMass;
					bodyInfo.m_lock = 0;
					
					 m_bodyArray.m_count ++;
				}
			}

		} else {
			_ASSERTE (heaviestBody);
			activeQueue.Insert (heaviestBody);
			heaviestBody->m_dynamicsLru = m_markLru;
		}


		while (!activeQueue.IsEmpty()) {
			dgInt32 count;
			dgInt32 index;

			count = activeQueue.m_firstIndex - activeQueue.m_lastIndex;
			if (count < 0) {
				_ASSERTE (0);
				count += activeQueue.m_mod;
			}

			index = activeQueue.m_lastIndex;
			activeQueue.Reset ();

			for (dgInt32 i = 0; i < count; i ++) {
				dgInt32 bodyIndex;
				dgBody* body;
				body = activeQueue.m_pool[index];
				_ASSERTE (body->m_dynamicsLru == body->GetWorld()->m_dynamicsLru);

				bodyIndex = m_bodyArray.m_count; 
				if (bodyIndex >= m_bodyArray.m_maxCount) {
					m_bodyArray.ReAlloc (bodyIndex);
				}

				body->m_index = bodyIndex;
				dgCudaBody::dgBodyInfo& bodyInfo = m_bodyArray.m_sysBody[bodyIndex];
				bodyInfo.m_body = body;
				(simd_type&)bodyInfo.m_matrix[ 0] = (simd_type&)body->m_matrix[0];
				(simd_type&)bodyInfo.m_matrix[ 4] = (simd_type&)body->m_matrix[1];
				(simd_type&)bodyInfo.m_matrix[ 8] = (simd_type&)body->m_matrix[2];
				(simd_type&)bodyInfo.m_matrix[12] = (simd_type&)body->m_matrix[3];
				(simd_type&)bodyInfo.m_invInertia = (simd_type&)body->m_invMass;
				(simd_type&)bodyInfo.m_veloc = (simd_type&)body->m_veloc;
				(simd_type&)bodyInfo.m_omega = (simd_type&)body->m_omega;
				(simd_type&)bodyInfo.m_accel = (simd_type&)body->m_accel;
				(simd_type&)bodyInfo.m_alpha = (simd_type&)body->m_alpha;
				(simd_type&)bodyInfo.m_dampCoef = (simd_type&)body->m_dampCoef;
				(simd_type&)bodyInfo.m_globalCentreOfMass = (simd_type&)body->m_globalCentreOfMass;
				bodyInfo.m_lock = 0;
				 m_bodyArray.m_count ++;

				for (dgBodyMasterListRow::dgListNode* jointNode = body->m_masterNode->GetInfo().GetFirst(); jointNode; jointNode = jointNode->GetNext()) {
					dgBody* nextBody;
					
					dgBodyMasterListCell& cell = jointNode->GetInfo();
					
					nextBody = cell.m_bodyNode;
					if (!nextBody->m_sleeping) {
						dgConstraint* constraint;
						constraint = cell.m_joint;
						if (constraint->m_dynamicsLru != lruMark) {
							dgInt32 dof;
						
							constraint->m_dynamicsLru = lruMark;
							if (m_jointGraph.m_count >= m_jointGraph.m_maxCount) {
								m_jointGraph.ReAlloc (m_jointGraph.m_count);
							}

							if (constraint->m_isUnilateral)	{
								hasUnilateralJoints = 1;
								_ASSERTE ((constraint->m_body0 == m_world->m_sentionelBody) || (constraint->m_body1 == m_world->m_sentionelBody));
							}			

							//constraint->m_index = jointCount;
							constraint->m_index = m_jointGraph.m_count;
							m_jointGraph.m_sysConstraintArray[m_jointGraph.m_count].m_code = 0;
							m_jointGraph.m_sysConstraintArray[m_jointGraph.m_count].m_joint = constraint;
							m_jointGraph.m_count ++;

							_ASSERTE (constraint->m_body0);
							_ASSERTE (constraint->m_body1);

							dof = (constraint->m_maxDOF & (DG_SIMD_WORD_SIZE - 1)) ? ((constraint->m_maxDOF & (-DG_SIMD_WORD_SIZE)) + DG_SIMD_WORD_SIZE) : constraint->m_maxDOF;
							m_jacobianMemory.m_count += dof;
						}

						if (nextBody->m_dynamicsLru != lruMark) {
							nextBody->m_dynamicsLru = lruMark;
							activeQueue.Insert (nextBody);
						}
					}
				}

				index ++;
				if (index >= activeQueue.m_mod) {
					_ASSERTE (0);
					index = 0;
				}
			}
		}

		while (!queue.IsEmpty()) {
			dgBody* body;
			body = queue.Remove ();
			body->m_dynamicsLru = m_markLru - 2;
		}

	}
}


