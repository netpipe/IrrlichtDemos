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

#ifndef __NEWTON_CUDA__
#define __NEWTON_CUDA__

#include <cudaStdafx.h>
#include <dg.h>
#include "dgCudaSolverKernels.h"

class dgBody;
class dgWorld; 
template<class T>
class dgQueue;


class dgCudaSolver
{
	public:
//	DG_CLASS_ALLOCATOR;

/*
	class dgWorkerScanLayer: public dgWorkerThread
	{
		public: 
		virtual void ThreadExecute();

		dgInt32 m_count;
		dgInt32 m_step;
//		dgInt32 m_thread;
		dgInt32 m_base;
		dgInt32 m_mod;
		dgUnsigned32 m_lruMark;
		dgWorld* m_world;
		dgInt32 *m_topIndex;
		dgBody** bodyList;
		//dgWorldDynamicUpdate* m_dynamics;
	};
*/

	void UpdateDynamics (dgWorld* world, dgFloat32 timestep);

	protected:
	dgCudaSolver ();
	~dgCudaSolver ();

	void ScanBodyGraph ();
	void SpanningTree (dgBody* body);
	dgBody* GetIslandFirstBody (dgBody* body, dgBody** const stack, dgInt32 size);
	void BuildJacobianMatrix (dgFloat32 timestep);
	void CalculateReactionsForces (dgFloat32 timestep, dgFloat32 tolerance);
	void IntegrateArray (dgFloat32 timestep, dgFloat32 accelTolerance) const;


	dgInt32 m_version;
	dgInt32 m_markLru;
	dgWorld* m_world;
	
	dgCudaBody m_bodyArray;
	dgCudaJointGraph m_jointGraph;
	dgCudaJointsSlack m_jointSlacks;
	dgCudaActiveIslands m_activeIslands;

	dgCudaContactMemory m_contactMemory;
	dgCudaJacobianMemory m_jacobianMemory;
};



#endif
