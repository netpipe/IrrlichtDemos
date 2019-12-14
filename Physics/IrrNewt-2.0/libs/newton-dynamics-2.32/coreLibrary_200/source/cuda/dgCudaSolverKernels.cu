#include <cudaStdafx.h>
#include "driver_types.h"
#include "cuda_runtime_api.h"
#include "dgCudaSolverKernels.h"




enum dgContactFlags
{
	m_collisionEnable__					 = 1<<0,
	m_friction0Enable__					 = 1<<1,
	m_friction1Enable__					 = 1<<2,
	m_override0Accel__					 = 1<<3,
	m_override1Accel__					 = 1<<4,
	m_overrideNormalAccel__				 = 1<<5,
	m_collisionContinueCollisionEnable__ = 1<<6,
};


#define DG_BILATERAL_CONSTRAINT				-1
#define DG_NORMAL_CONSTRAINT				-2 
#define DG_BILATERAL_FRICTION_CONSTRAINT	-3 

#define DG_PSD_DAMP_TOL						float (1.0e-3f)
#define DG_MAX_BOUND						float (1.0e15f)
#define DG_MIN_BOUND						(-DG_MAX_BOUND)
#define REST_RELATIVE_VELOCITY				float (1.0e-3f)
#define MAX_DYNAMIC_FRICTION_SPEED			float (0.3f)


//static void* m_returnValuePtr;
//static void* m_userGlobalLockPtr;
//__device__ int m_returnValue;
//__device__ int m_userGlobalLock = 0;



void dgCudaExpandTraceMessage (const char *fmt, ...)
{
	va_list v_args;
	char text[1024];

	text[0] = 0;
	va_start (v_args, fmt);     
	vsprintf(text, fmt, v_args);
	va_end (v_args);            

	OutputDebugStringA (text);
}



#define dgCudaTrace(x)				\
{									\
	dgCudaExpandTraceMessage x;		\
}																	




__device__ float DotProduct (float* const A, const float* const B)
{
	return A[0] * B[0] + A[1] * B[1] + A[2] * B[2];
}



__global__ void InertiaMatricesAndKineticForces (dgCudaBody::dgBodyInfo* const bodies);

__global__ void CalculateContactDerivative (dgCudaJointGraph::dgJointInfo* const constraintInfo,
											dgCudaBody::dgBodyInfo* const bodyArray,
											dgCudaContactMemory::dgContact* const contactMemory, 
											dgCudaJacobianMemory::dgJacobianInfo* const jacobianMemory, 
											float invTimeStep); 

__global__ void dgParallelSolverBuildJacobianRows (dgCudaJointGraph::dgJointInfo* const constraintInfo,
												   dgCudaBody::dgBodyInfo* const bodyArray,
												   dgCudaJacobianMemory::dgJacobianInfo* const jacobianMemory);

__global__ void InitInternalForces (int* const jointIndex,
									dgCudaJointGraph::dgJointInfo* const constraintInfo,
								    dgCudaBody::dgBodyInfo* const bodyArray,
								    dgCudaBody::dgBodyWorkData* const bodyInternalForces,
									dgCudaJacobianMemory::dgJacobianInfo* const jacobianMemory);
									
__global__ void CalculateRowsAcceleration (dgCudaJointGraph::dgJointInfo* const constraintInfo,
										   dgCudaBody::dgBodyInfo* const bodyArray,
										   dgCudaJacobianMemory::dgJacobianInfo* const jacobianMemory,
										   float timestep,
										   float invTimestep,
										   float firstPassCoef);
										 
										   
__global__ void CalculateJointForces (int* const jointIndex,
									  dgCudaJointGraph::dgJointInfo* const constraintInfo,
									  dgCudaBody::dgBodyInfo* const bodyArray,
									  dgCudaBody::dgBodyWorkData* const bodyForceVeloc,
									  dgCudaJacobianMemory::dgJacobianInfo* const jacobianMemory);
									
__global__ void UpdateBodiesVelocities (dgCudaBody::dgBodyInfo* const bodyArray,
										dgCudaBody::dgBodyWorkData* const bodyForceVeloc,
										float timestep);
										
								
								
__device__ void SpinLock (int* lock)
{
	if (threadIdx.x == 0) {
		while (atomicCAS(lock, 0, 1));
	}
	__syncthreads();
}
		


__device__ void SpinUnlock (int* lock)
{
	if (threadIdx.x == 0) {
		atomicExch(lock, 0); 
	}
	__syncthreads();
}
								


void* CudaMalloc(int size)
{
	void* mem;
	cudaError_t error;

	error = cudaMalloc ((void**)&mem, size);
	_ASSERTE (error == cudaSuccess);

	return mem;
}

void CudaFree(void* ptr)
{
	cudaError_t error;

	error = cudaFree (ptr);
	_ASSERTE (error == cudaSuccess);
}


/*
__device__ void MatrixRotateVector(float* const out, const float* const vector, const float* const matrix)
{
	int row;
	__shared__ float tmp [16]; 
	
	row = threadIdx.x >> 2;
	
	tmp[threadIdx.x] = vector[row] * matrix[threadIdx.x];
	__syncthreads();
	if (threadIdx.x < 4) {
		out[threadIdx.x] = tmp[threadIdx.x] + tmp[threadIdx.x + 4] + tmp[threadIdx.x + 8];		
	}
}

__device__ void MatrixUnrotateVector(float* const out, const float* const vector, const float* const matrix)
{
	int colum;
	__shared__ float tmp [16]; 

	colum = threadIdx.x & 3;
	tmp[threadIdx.x] = vector[colum] * matrix[threadIdx.x];
//	__syncthreads();		
	if (threadIdx.x < 4) {
		int j = threadIdx.x << 2;
		out[threadIdx.x] = tmp[j] + tmp[j+1] + tmp[j+2];		
	}
}

__device__ void MatrixMultiply(float* const out, const float* const A, const float* const B)
{
	int row;
	int colum;
	int rowScale;
	
	row = threadIdx.x >> 2;
	colum = threadIdx.x & 3;
	rowScale = row << 2;
	
	out[rowScale + colum]  = A[rowScale + 0] * B[0 + colum];
	out[rowScale + colum] += A[rowScale + 1] * B[4 + colum];
	out[rowScale + colum] += A[rowScale + 2] * B[8 + colum];
//	__syncthreads();			
}
*/

void CalculateInertiaMatricesAndBodyKineticForces (dgCudaBody& bodyArray)
{
	cudaError_t error;
	if (bodyArray.m_count > 0) {	
		error = cudaMemcpy (&bodyArray.m_devBody[0], &bodyArray.m_sysBody[0], bodyArray.m_count * sizeof (dgCudaBody::dgBodyInfo), cudaMemcpyHostToDevice);
		_ASSERTE (error == cudaSuccess);
		
		error = cudaMemset (&bodyArray.m_devBodyWorkData[0], 0, bodyArray.m_count * sizeof (dgCudaBody::dgBodyWorkData));
		_ASSERTE (error == cudaSuccess);	

//		int count;
//		int threads;
//		threads = 1<<(4 + DG_CLUSTER_TREAD_SIZE_BITS);
//		count = ((bodyArray.m_count - 1) >> DG_CLUSTER_TREAD_SIZE_BITS) + 1;
//		InertiaMatricesAndKineticForces <<<count, threads>>> (&bodyArray.m_devBody[0]);
		InertiaMatricesAndKineticForces <<<bodyArray.m_count, 16>>> (&bodyArray.m_devBody[0]);
	}
}


void CalculateContactDerivatives (
	dgCudaJointGraph& contraints, 
	dgCudaContactMemory& contactMemory, 
	dgCudaJacobianMemory& jacobianMemory, 
	dgCudaBody& bodyArray, 
	float timestep)	
{
	cudaError_t error;
	float invTimestep;
	
	if (contraints.m_count > 0) {	
		error =	cudaMemcpy (&contraints.m_devConstraintArray[0], &contraints.m_sysConstraintArray[0], contraints.m_count * sizeof (dgCudaJointGraph::dgJointInfo), cudaMemcpyHostToDevice);
		_ASSERTE (error == cudaSuccess);

		error =	cudaMemcpy (&contactMemory.m_devContacts[0], &contactMemory.m_sysContacts[0], contactMemory.m_count * sizeof (dgCudaContactMemory::dgContact), cudaMemcpyHostToDevice);
		_ASSERTE (error == cudaSuccess);

		invTimestep = 1.0f / timestep;
		CalculateContactDerivative  <<<contraints.m_count, 16>>> (&contraints.m_devConstraintArray[0], 
																  &bodyArray.m_devBody[0], 
																  &contactMemory.m_devContacts[0],
																  &jacobianMemory.m_devJacobian[0],
																  invTimestep);
  }
}



void CalculateJacobianMatrix (dgCudaJointGraph& contraints, dgCudaBody& bodyArray, dgCudaJacobianMemory& jacobianMemory)
{
	if (contraints.m_count > 0) {
		dgParallelSolverBuildJacobianRows  <<<contraints.m_count, 16>>> (&contraints.m_devConstraintArray[0],
																		 &bodyArray.m_devBody[0],
																		 &jacobianMemory.m_devJacobian[0]);
	}																		 
}	


void InitInternalForces (
	int deviceVersion,
	dgCudaJointsSlack& slack, 
	dgCudaJointGraph& contraints, 
	dgCudaBody& bodyArray, 
	dgCudaJacobianMemory& jacobianMemory)
{
	if (deviceVersion >= 101) {
	
		cudaError_t error;
		error =	cudaMemcpy (&slack.m_slackArray[0].m_devJointSlack[0], &slack.m_slackArray[0].m_sysJointSlack[0], contraints.m_count * sizeof (int), cudaMemcpyHostToDevice);
		_ASSERTE (error == cudaSuccess);

		InitInternalForces  <<<contraints.m_count, 16>>> (&slack.m_slackArray[0].m_devJointSlack[0],
														  &contraints.m_devConstraintArray[0],
														  &bodyArray.m_devBody[0],
														  &bodyArray.m_devBodyWorkData[0],
														  &jacobianMemory.m_devJacobian[0]);

	
	} else {
		for (int i = 0; slack.m_slackArray[i].m_count; i ++) {
			cudaError_t error;

			error =	cudaMemcpy (&slack.m_slackArray[i].m_devJointSlack[0], &slack.m_slackArray[i].m_sysJointSlack[0], slack.m_slackArray[i].m_count * sizeof (int), cudaMemcpyHostToDevice);
			_ASSERTE (error == cudaSuccess);

			InitInternalForces  <<<slack.m_slackArray[i].m_count, 16>>> (&slack.m_slackArray[i].m_devJointSlack[0],
																		 &contraints.m_devConstraintArray[0],
																		 &bodyArray.m_devBody[0],
																		 &bodyArray.m_devBodyWorkData[0],
																		 &jacobianMemory.m_devJacobian[0]);
		}														  
	}
}	


void CalculateRowsAcceleration (
	dgCudaJointGraph& contraints, 
	dgCudaBody& bodyArray, 
	dgCudaJacobianMemory& jacobianMemory, 
	float timestep,
	float invTimestep,
	float firstPassCoef)
{
	if (contraints.m_count > 0) {
		CalculateRowsAcceleration  <<<contraints.m_count, 16>>> (&contraints.m_devConstraintArray[0],
																 &bodyArray.m_devBody[0],
																 &jacobianMemory.m_devJacobian[0],
																 timestep,
																 invTimestep,
																 firstPassCoef);
	}																 
} 


void CalculateJointForces (
	int deviceVersion,
	dgCudaJointsSlack& slack, 
	dgCudaJointGraph& contraints, 
	dgCudaBody& bodyArray, 
	dgCudaJacobianMemory& jacobianMemory)
{
	if (deviceVersion >= 101) {
		CalculateJointForces  <<<contraints.m_count, 16>>> (&slack.m_slackArray[0].m_devJointSlack[0],
											                &contraints.m_devConstraintArray[0],
															&bodyArray.m_devBody[0],
															&bodyArray.m_devBodyWorkData[0],
															&jacobianMemory.m_devJacobian[0]);
	} else {
		for (int i = 0; slack.m_slackArray[i].m_count; i ++) {
			CalculateJointForces  <<<slack.m_slackArray[i].m_count, 16>>> (&slack.m_slackArray[i].m_devJointSlack[0],
																		   &contraints.m_devConstraintArray[0],
																		   &bodyArray.m_devBody[0],
																		   &bodyArray.m_devBodyWorkData[0],
																		   &jacobianMemory.m_devJacobian[0]);
		}
	}
} 


void UpdateBodiesVelocities (dgCudaBody& bodyArray, float timestep)
{
	if (bodyArray.m_count > 0) {
		UpdateBodiesVelocities <<<bodyArray.m_count, 16>>> (&bodyArray.m_devBody[0],
															&bodyArray.m_devBodyWorkData[0],
															timestep);
	}
} 







__global__ void CalculateContactDerivative (
	dgCudaJointGraph::dgJointInfo* const constraintInfo,
	dgCudaBody::dgBodyInfo* const bodyArray,
	dgCudaContactMemory::dgContact* const contactMemory, 
	dgCudaJacobianMemory::dgJacobianInfo* const jacobianMemory, 
	float invTimestep) 
{
	int block;
	int thread;
	__shared__ dgCudaBody::dgBodyInfo bodyInfo0;
	__shared__ dgCudaBody::dgBodyInfo bodyInfo1;
	__shared__ dgCudaContactMemory::dgContact contact;
	__shared__ dgCudaJointGraph::dgJointInfo jointInfo;
	__shared__ dgCudaJacobianMemory::dgJacobianInfo jacobianInfo;
	__shared__ float r0[4];
	__shared__ float r1[4];
	__shared__ float veloc0[4];
	__shared__ float veloc1[4];
	__shared__ float velocError[4];
	
	block = blockIdx.x;
	thread = threadIdx.x;
	
	jointInfo.m_data0[thread] = constraintInfo[block].m_data0[thread];
	__syncthreads();
	

	if (jointInfo.m_contactStart >= 0) {
		int m0;
		int m1;
		int count;
		int start;
		int isMotor;
		int jacobIndex;
		int frictionIndex;
		float accel;
		float restitution;
		float penetration;
		float relVelocErr;
		float lowBound;
		float upperBound;
		float penetrationVeloc;
		float penetrationStiffness;
		
		int crossX = thread + 1;
		int crossY = thread + 2;
		if (crossX >= 3) {
			crossX -= 3;
		}
		if (crossY >= 3) {
			crossY -= 3;
		}
		if (thread > 2) {
			crossX = 3;
			crossY = 3;
		}

		count = jointInfo.m_autoPaircount;
		m0 = jointInfo.m_m0;
		m1 = jointInfo.m_m1;
		start = jointInfo.m_contactStart;
		jacobIndex = jointInfo.m_autoPairstart;
		frictionIndex = jacobIndex + count;

		bodyInfo0.m_data0[thread] = bodyArray[m0].m_data0[thread];
		bodyInfo0.m_data1[thread] = bodyArray[m0].m_data1[thread];
		bodyInfo1.m_data0[thread] = bodyArray[m1].m_data0[thread];
		bodyInfo1.m_data1[thread] = bodyArray[m1].m_data1[thread];
		__syncthreads();
		
		for (int i = 0; i < count; i ++) {
		
			contact.m_data0[thread] = contactMemory[start + i].m_data0[thread];
			contact.m_data1[thread] = contactMemory[start + i].m_data1[thread];
			__syncthreads();

			if (thread < 4) {
				r0[thread] = contact.m_point[thread] - bodyInfo0.m_globalCentreOfMass[thread];
				r1[thread] = contact.m_point[thread] - bodyInfo1.m_globalCentreOfMass[thread];
			}
			__syncthreads();

			if (thread < 4) {
				veloc0[thread] = bodyInfo0.m_veloc[thread] + bodyInfo0.m_omega[crossX] * r0[crossY] - bodyInfo0.m_omega[crossY] * r0[crossX];
				veloc1[thread] = bodyInfo1.m_veloc[thread] + bodyInfo1.m_omega[crossX] * r1[crossY] - bodyInfo1.m_omega[crossY] * r1[crossX];
				velocError[thread] = veloc1[thread] - veloc0[thread];
			}
			__syncthreads();

			if (thread < 4) {
				jacobianInfo.m_Jt.m_jacobian_IM0.m_linear[thread] = contact.m_normal[thread];
				jacobianInfo.m_Jt.m_jacobian_IM0.m_angular[thread] = r0[crossX] * contact.m_normal[crossY] - r0[crossY] * contact.m_normal[crossX];			
				
				jacobianInfo.m_Jt.m_jacobian_IM1.m_linear[thread] = -contact.m_normal[thread];
				jacobianInfo.m_Jt.m_jacobian_IM1.m_angular[thread] = contact.m_normal[crossX] * r1[crossY] - contact.m_normal[crossY] * r1[crossX];
			}
			__syncthreads();
			jacobianMemory[jacobIndex].m_Jt.m_jacobianData[thread] = jacobianInfo.m_Jt.m_jacobianData[thread];
			__syncthreads();

			if (thread == 0) {
				restitution	= contact.m_restitution;
				relVelocErr = DotProduct (velocError, contact.m_normal);
				
				penetration = (contact.m_penetration < 0.5f) ? contact.m_penetration : 0.5f;

				penetrationStiffness = 50.0f * contact.m_softness;
				penetrationVeloc = penetration * penetrationStiffness;
				if (relVelocErr > REST_RELATIVE_VELOCITY) {
					relVelocErr *= (restitution + 1.0f);
				}


				jacobianInfo.m_restitution = contact.m_restitution;
				jacobianInfo.m_penetration = penetration;
				jacobianInfo.m_penetrationStiffness = penetrationStiffness;
				jacobianInfo.m_lowerBoundFrictionCoefficent = 0.0f;
				jacobianInfo.m_upperBoundFrictionCoefficent = DG_MAX_BOUND;
				jacobianInfo.m_normalForceIndex = DG_NORMAL_CONSTRAINT;
				jacobianInfo.m_force = contact.m_normal_Force;
				jacobianInfo.m_jointFeebackForcePtr = contact.m_normal_ForcePtr;
				jacobianInfo.m_diagDamp = 1.0f;
				jacobianInfo.m_accelIsMotor = 0;
				accel = relVelocErr + penetrationVeloc;
				accel = ((accel > -4.0f) ? accel : -4.0f) * invTimestep;
				if (contact.m_flags & m_overrideNormalAccel__) {
					accel += contact.m_normal_Force;
				}
				jacobianInfo.m_coordenateAccel = accel;
			}
			__syncthreads();
			jacobianMemory[jacobIndex].m_data0[thread] = jacobianInfo.m_data0[thread];
			__syncthreads();
			
			// first dir friction force
			if (contact.m_flags & m_friction0Enable__) {
			
				if (thread < 4) {
					jacobianInfo.m_Jt.m_jacobian_IM0.m_linear[thread] = contact.m_dir0[thread];
					jacobianInfo.m_Jt.m_jacobian_IM0.m_angular[thread] = r0[crossX] * contact.m_dir0[crossY] - r0[crossY] * contact.m_dir0[crossX];			
													
					jacobianInfo.m_Jt.m_jacobian_IM1.m_linear[thread] = -contact.m_dir0[thread];
					jacobianInfo.m_Jt.m_jacobian_IM1.m_angular[thread] = contact.m_dir0[crossX] * r1[crossY] - contact.m_dir0[crossY] * r1[crossX];
				}
				__syncthreads();
				jacobianMemory[frictionIndex].m_Jt.m_jacobianData[thread] = jacobianInfo.m_Jt.m_jacobianData[thread];

				if (thread == 0) {
					relVelocErr = DotProduct (velocError, contact.m_dir0);
					jacobianInfo.m_normalForceIndex = jacobIndex;
					jacobianInfo.m_diagDamp = 1.0f;
					jacobianInfo.m_restitution = 0.0f;
					jacobianInfo.m_penetration = 0.0f;
					jacobianInfo.m_penetrationStiffness = 0.0f;
					if (contact.m_flags & m_override0Accel__) {
						accel = contact.m_dir0_Force;
						isMotor = 1;
					} else {
						accel = relVelocErr * invTimestep;
						isMotor = 0;
					}
					
					jacobianInfo.m_coordenateAccel = accel;
					jacobianInfo.m_accelIsMotor = isMotor;
					
					if (fabsf (relVelocErr) > MAX_DYNAMIC_FRICTION_SPEED) {
						lowBound = -contact.m_dynamicFriction0;
						upperBound = contact.m_dynamicFriction0;
					} else {
						lowBound = -contact.m_staticFriction0;
						upperBound = contact.m_staticFriction0;
					}
					
					jacobianInfo.m_lowerBoundFrictionCoefficent = lowBound;
					jacobianInfo.m_upperBoundFrictionCoefficent = upperBound;
					jacobianInfo.m_force = contact.m_dir0_Force;
					jacobianInfo.m_jointFeebackForcePtr = contact.m_dir0_ForcePtr;
				}
				__syncthreads();
				jacobianMemory[frictionIndex].m_data0[thread] = jacobianInfo.m_data0[thread];
				
				frictionIndex ++;
				__syncthreads();
			}

			if (contact.m_flags & m_friction1Enable__) {

				if (thread < 4) {
					jacobianInfo.m_Jt.m_jacobian_IM0.m_linear[thread] = contact.m_dir1[thread];
					jacobianInfo.m_Jt.m_jacobian_IM0.m_angular[thread] = r0[crossX] * contact.m_dir1[crossY] - r0[crossY] * contact.m_dir1[crossX];			
													
					jacobianInfo.m_Jt.m_jacobian_IM1.m_linear[thread] = -contact.m_dir1[thread];
					jacobianInfo.m_Jt.m_jacobian_IM1.m_angular[thread] = contact.m_dir1[crossX] * r1[crossY] - contact.m_dir1[crossY] * r1[crossX];
				}
				__syncthreads();
				jacobianMemory[frictionIndex].m_Jt.m_jacobianData[thread] = jacobianInfo.m_Jt.m_jacobianData[thread];
				__syncthreads();	
				
				if (thread == 0) {				
					relVelocErr = DotProduct (velocError, contact.m_dir1);
					jacobianInfo.m_normalForceIndex = jacobIndex;
					jacobianInfo.m_diagDamp = 1.0f;
					jacobianInfo.m_restitution = 0.0f;
					jacobianInfo.m_penetration = 0.0f;
					jacobianInfo.m_penetrationStiffness = 0.0f;
					if (contact.m_flags & m_override1Accel__) {
						accel = contact.m_dir1_Force;
						isMotor = 1;
					} else {
						accel = relVelocErr * invTimestep;
						isMotor = 0;
					}
					jacobianInfo.m_coordenateAccel = accel;
					jacobianInfo.m_accelIsMotor = isMotor;
					
					if (fabsf (relVelocErr) > MAX_DYNAMIC_FRICTION_SPEED) {
						lowBound = -contact.m_dynamicFriction1;
						upperBound = contact.m_dynamicFriction1;
					} else {
						lowBound = -contact.m_staticFriction1;
						upperBound = contact.m_staticFriction1;
					}
					
					jacobianInfo.m_lowerBoundFrictionCoefficent = lowBound;
					jacobianInfo.m_upperBoundFrictionCoefficent = upperBound;
					jacobianInfo.m_force = contact.m_dir1_Force;
					jacobianInfo.m_jointFeebackForcePtr = contact.m_dir1_ForcePtr;
				}
				__syncthreads();
				jacobianMemory[frictionIndex].m_data0[thread] = jacobianInfo.m_data0[thread];

				frictionIndex ++;
				__syncthreads();
			}

			__syncthreads();
			jacobIndex ++;
		}

		jointInfo.m_autoPaircount = frictionIndex - jointInfo.m_autoPairstart;

		jointInfo.m_autoPairActiveCount = jointInfo.m_autoPaircount;
		constraintInfo[block].m_data0[thread] = jointInfo.m_data0[thread];
	}
}



__global__ void dgParallelSolverBuildJacobianRows (
	dgCudaJointGraph::dgJointInfo* const constraintInfo,
	dgCudaBody::dgBodyInfo* const bodyArray,
	dgCudaJacobianMemory::dgJacobianInfo* const jacobianMemory)
{
	int block;
	int thread;
	int m0;
	int m1;
	int index;
	int count;
	int colum;
	int row;
	float diag;
	float accel;
	float stiffness;
	__shared__ dgCudaBody::dgBodyInfo bodyInfo0;
	__shared__ dgCudaBody::dgBodyInfo bodyInfo1;
	__shared__ dgCudaJointGraph::dgJointInfo jointInfo;
	__shared__ dgCudaJacobianMemory::dgJacobianInfo jacobianInfo;	
	__shared__ float tmpMatrix0[16];
	__shared__ float tmpMatrix1[16];
	__shared__ float tmpDiag [4];
	__shared__ float tmpAccel [4];	

	block = blockIdx.x;
	thread = threadIdx.x;

	jointInfo.m_data0[thread] = constraintInfo[block].m_data0[thread];
	colum = thread & 3;
	__syncthreads();

	m0 = jointInfo.m_m0;
	m1 = jointInfo.m_m1;
	count = jointInfo.m_autoPaircount;
	index = jointInfo.m_autoPairstart;

	bodyInfo0.m_invInertiaMatrix[thread] = bodyArray[m0].m_invInertiaMatrix[thread];
	bodyInfo1.m_invInertiaMatrix[thread] = bodyArray[m1].m_invInertiaMatrix[thread];
	
	bodyInfo0.m_data0[thread] = bodyArray[m0].m_data0[thread];
	bodyInfo1.m_data0[thread] = bodyArray[m1].m_data0[thread];
	bodyInfo0.m_data1[thread] = bodyArray[m0].m_data1[thread];
	bodyInfo1.m_data1[thread] = bodyArray[m1].m_data1[thread];	
	__syncthreads();

	for (int i = 0; i < count; i ++) {
		jacobianInfo.m_Jt.m_jacobianData[thread] = jacobianMemory[index].m_Jt.m_jacobianData[thread];
		jacobianInfo.m_data0[thread] = jacobianMemory[index].m_data0[thread];
		__syncthreads();

		tmpMatrix0[thread] = jacobianInfo.m_Jt.m_jacobian_IM0.m_angular[colum] * bodyInfo0.m_invInertiaMatrix[thread];
		tmpMatrix1[thread] = jacobianInfo.m_Jt.m_jacobian_IM1.m_angular[colum] * bodyInfo1.m_invInertiaMatrix[thread];
		__syncthreads();

		if (thread < 4) {
			row = thread << 2;
//			JMinv[index].m_jacobian_IM0.m_angular = invInertia0.UnrotateVector (Jt[index].m_jacobian_IM0.m_angular);
//			JMinv[index].m_jacobian_IM1.m_angular = invInertia1.UnrotateVector (Jt[index].m_jacobian_IM1.m_angular);
			jacobianInfo.m_JMinv.m_jacobian_IM0.m_angular[thread] = tmpMatrix0[row] + tmpMatrix0[row + 1] + tmpMatrix0[row + 2];		
			jacobianInfo.m_JMinv.m_jacobian_IM1.m_angular[thread] = tmpMatrix1[row] + tmpMatrix1[row + 1] + tmpMatrix1[row + 2];					

//			JMinv[index].m_jacobian_IM0.m_linear = Jt[index].m_jacobian_IM0.m_linear.Scale (invMass0);
//			JMinv[index].m_jacobian_IM1.m_linear = Jt[index].m_jacobian_IM1.m_linear.Scale (invMass1);
			jacobianInfo.m_JMinv.m_jacobian_IM0.m_linear[thread] = jacobianInfo.m_Jt.m_jacobian_IM0.m_linear[thread] * bodyInfo0.m_invInertia[3];		
			jacobianInfo.m_JMinv.m_jacobian_IM1.m_linear[thread] = jacobianInfo.m_Jt.m_jacobian_IM1.m_linear[thread] * bodyInfo1.m_invInertia[3];		
		}
		__syncthreads();
		jacobianMemory[index].m_JMinv.m_jacobianData[thread] = jacobianInfo.m_JMinv.m_jacobianData[thread];
		__syncthreads();		

		if (thread < 4) {			
	
//			dgVector tmpDiag (JMinv[index].m_jacobian_IM0.m_linear.CompProduct(Jt[index].m_jacobian_IM0.m_linear));
//			tmpDiag += JMinv[index].m_jacobian_IM0.m_angular.CompProduct(Jt[index].m_jacobian_IM0.m_angular);
//			tmpDiag += JMinv[index].m_jacobian_IM1.m_linear.CompProduct(Jt[index].m_jacobian_IM1.m_linear);
//			tmpDiag += JMinv[index].m_jacobian_IM1.m_angular.CompProduct(Jt[index].m_jacobian_IM1.m_angular);
			
			diag = jacobianInfo.m_JMinv.m_jacobian_IM0.m_linear[thread] * jacobianInfo.m_Jt.m_jacobian_IM0.m_linear[thread] + 
				   jacobianInfo.m_JMinv.m_jacobian_IM0.m_angular[thread] * jacobianInfo.m_Jt.m_jacobian_IM0.m_angular[thread] + 
				   jacobianInfo.m_JMinv.m_jacobian_IM1.m_linear[thread] * jacobianInfo.m_Jt.m_jacobian_IM1.m_linear[thread] + 
				   jacobianInfo.m_JMinv.m_jacobian_IM1.m_angular[thread] * jacobianInfo.m_Jt.m_jacobian_IM1.m_angular[thread]; 
				   
//			dgVector tmpAccel (JMinv[index].m_jacobian_IM0.m_linear.CompProduct(body0->m_accel));
//			tmpAccel += JMinv[index].m_jacobian_IM0.m_angular.CompProduct(body0->m_alpha);
//			tmpAccel += JMinv[index].m_jacobian_IM1.m_linear.CompProduct(body1->m_accel);
//			tmpAccel += JMinv[index].m_jacobian_IM1.m_angular.CompProduct(body1->m_alpha);
			accel = jacobianInfo.m_JMinv.m_jacobian_IM0.m_linear[thread] * bodyInfo0.m_accel[thread] + 
					jacobianInfo.m_JMinv.m_jacobian_IM0.m_angular[thread] * bodyInfo0.m_alpha[thread] +
					jacobianInfo.m_JMinv.m_jacobian_IM1.m_linear[thread] * bodyInfo1.m_accel[thread] + 
					jacobianInfo.m_JMinv.m_jacobian_IM1.m_angular[thread] * bodyInfo1.m_alpha[thread];
					
			tmpDiag[thread] = diag;
			tmpAccel[thread] = accel;	
		}
		__syncthreads();
	

		if (thread == 0) {
			float externalAcceleration;
			externalAcceleration = -(tmpAccel[0] + tmpAccel[1] + tmpAccel[2]);
			
//			m_deltaAccel[index] = externalAcceleration;
			jacobianInfo.m_deltaAccel = externalAcceleration;
			
//			coordenateAccel[index] += extenalAcceleration;
			jacobianInfo.m_coordenateAccel += externalAcceleration;
			
//			force[index] = jointForceFeeback[index][0];
//			jacobianInfo.m_force = jacobianInfo.m_forceValue;

//			_ASSERTE (diagDamp[index] >= float(0.1f));
//			_ASSERTE (diagDamp[index] <= float(100.0f));
//			stiffness = DG_PSD_DAMP_TOL * diagDamp[index];
			stiffness = DG_PSD_DAMP_TOL * jacobianInfo.m_diagDamp;

//			diag = (tmpDiag.m_x + tmpDiag.m_y + tmpDiag.m_z);
			diag = tmpDiag[0] + tmpDiag[1] + tmpDiag[2];
			
//			diagDamp[index] = diag * stiffness;
			jacobianInfo.m_diagDamp = diag * stiffness;

			diag *= (float(1.0f) + stiffness);

//			invDJMinvJt[index] = float(1.0f) / diag;
			jacobianInfo.m_invDJMinvJt = float(1.0f) / diag;
		}
		__syncthreads();
		jacobianMemory[index].m_data0[thread] = jacobianInfo.m_data0[thread];
		
		index ++;
	}
}






__global__ void CalculateRowsAcceleration (
	dgCudaJointGraph::dgJointInfo* const constraintInfo,
	dgCudaBody::dgBodyInfo* const bodyArray,
	dgCudaJacobianMemory::dgJacobianInfo* const jacobianMemory,
	float timestep,
	float invTimestep,
	float firstPassCoef)
{
	int block;
	int thread;
	int m0;
	int m1;
	int count;
	int first;
	float vRel;
	float aRel;
	float tmp;
	float restitution;
	float penetrationVeloc;
	float penetrationCorrection;
	__shared__ dgCudaJointGraph::dgJointInfo jointInfo;
	__shared__ dgCudaJacobianMemory::dgJacobianPair veloc0;
	__shared__ dgCudaJacobianMemory::dgJacobianPair veloc1;
	__shared__ dgCudaJacobianMemory::dgJacobianPair tmpVeloc;
	__shared__ dgCudaJacobianMemory::dgJacobianInfo jacobianRow;
	
	block = blockIdx.x;
	thread = threadIdx.x;
	
	jointInfo.m_data0[thread] = constraintInfo[block].m_data0[thread];	
	__syncthreads();

	m0 = jointInfo.m_m0;
	m1 = jointInfo.m_m1;
	count = jointInfo.m_autoPaircount;
	first = jointInfo.m_autoPairstart;

//	const dgJacobianPair* const Jt = params.m_Jt;
//	const dgVector& bodyVeloc0 = m_body0->m_veloc;
//	const dgVector& bodyOmega0 = m_body0->m_omega;
//	const dgVector& bodyVeloc1 = m_body1->m_veloc;
//	const dgVector& bodyOmega1 = m_body1->m_omega;

	veloc0.m_jacobianData[thread] = bodyArray[m0].m_data0[thread];
	veloc1.m_jacobianData[thread] = bodyArray[m1].m_data0[thread];
	__syncthreads();
	for (dgInt32 k = 0; k < count; k ++) {
		jacobianRow.m_data0[thread] = jacobianMemory[first].m_data0[thread];
		__syncthreads();
		
		if (!jacobianRow.m_accelIsMotor) {
			jacobianRow.m_Jt.m_jacobianData[thread] = jacobianMemory[first].m_Jt.m_jacobianData[thread];
			__syncthreads();

			if (thread < 8) {			
	//			dgVector relVeloc (Jt[k].m_jacobian_IM0.m_linear.CompProduct(bodyVeloc0));
	//			relVeloc += Jt[k].m_jacobian_IM0.m_angular.CompProduct(bodyOmega0);
				tmpVeloc.m_jacobianData[thread] = jacobianRow.m_Jt.m_jacobian_IM0.m_data[thread] * veloc0.m_jacobianData[thread];

	//			relVeloc += Jt[k].m_jacobian_IM1.m_linear.CompProduct(bodyVeloc1);
	//			relVeloc += Jt[k].m_jacobian_IM1.m_angular.CompProduct(bodyOmega1);
				tmpVeloc.m_jacobianData[thread] += jacobianRow.m_Jt.m_jacobian_IM1.m_data[thread] * veloc1.m_jacobianData[thread];
			}
			__syncthreads();
			
			if (thread < 4) {		
				tmpVeloc.m_jacobian_IM0.m_linear[thread] += tmpVeloc.m_jacobian_IM0.m_angular[thread]; 	
			}
			__syncthreads();
			
			if (thread == 0) {		
//				vRel = relVeloc.m_x + relVeloc.m_y + relVeloc.m_z;
//				aRel = params.m_externAccelaration[k];
				vRel = tmpVeloc.m_jacobian_IM0.m_linear[0] + tmpVeloc.m_jacobian_IM0.m_linear[1] + tmpVeloc.m_jacobian_IM0.m_linear[2];
				aRel = jacobianRow.m_deltaAccel;
							
				if (jacobianRow.m_normalForceIndex < 0) {
//					restitution = dgFloat32 (1.0f);
//					if (vRel <= dgFloat32 (0.0f)) {
//						restitution += params.m_restitution[k];
//					}
					restitution = (vRel <= 0.0f) ? 1.0f + jacobianRow.m_restitution : 1.0f;
					penetrationVeloc = 0.0f;
					
					if (jacobianRow.m_penetration > 1.0e-2f) {
						if (vRel > 0.0f) {
							penetrationCorrection = vRel * timestep;
//							params.m_penetration[k] = GetMax (dgFloat32 (0.0f), params.m_penetration[k] - penetrationCorrection);
							tmp = jacobianRow.m_penetration - penetrationCorrection;
							jacobianRow.m_penetration = (tmp > 0.0f) ? tmp : 0.0f;
						}
//						penetrationVeloc = -(params.m_penetration * params.m_penetrationStiffness[k]);
						penetrationVeloc = - (jacobianRow.m_penetration * jacobianRow.m_penetrationStiffness);
					}

					vRel *= restitution;
//					vRel = GetMin (dgFloat32 (4.0f), vRel + penetrationVeloc);
					tmp = vRel + penetrationVeloc;
					vRel = (tmp < 4.0f) ? tmp : 4.0f;
				}
//				params.m_coordenateAccel[k] =  (aRel - vRel * params.m_invTimeStep);
				jacobianRow.m_coordenateAccel = (aRel - vRel * invTimestep);
			}
			__syncthreads();			
			
			jacobianMemory[first].m_data0[thread] = jacobianRow.m_data0[thread];
			__syncthreads();			
		}
		first ++;
		__syncthreads();
	}
}




__global__ void UpdateBodiesVelocities (
	dgCudaBody::dgBodyInfo* const bodyArray,
	dgCudaBody::dgBodyWorkData* const bodyForceVeloc,
	float timestep)
{
	int row;
	int block;
	int thread;
	__shared__ float tmp[16];
	__shared__ dgCudaJacobian accelAlpha;
	__shared__ dgCudaJacobian forceAlpha;
	__shared__ dgCudaBody::dgBodyInfo bodyInfo;
	__shared__ dgCudaBody::dgBodyWorkData internalForceVeloc;

	block = blockIdx.x;
	thread = threadIdx.x;
	row = thread >> 2;

	bodyInfo.m_data1[thread] = bodyArray[block].m_data1[thread];
	__syncthreads();
	
	if (bodyInfo.m_invInertia[3] > 0.0f) {
		bodyInfo.m_invInertiaMatrix[thread] = bodyArray[block].m_invInertiaMatrix[thread];
		bodyInfo.m_data0[thread] = bodyArray[block].m_data0[thread];	
		internalForceVeloc.m_data0[thread] = bodyForceVeloc[block].m_data0[thread];
		__syncthreads();

	//	dgBody* body;
	//	body = m_bodyArray[i].m_body;
	//	dgVector force (body->m_accel + m_internalForces[i].m_linear);
	//	dgVector torque (body->m_alpha + m_internalForces[i].m_angular);
		if (thread < 8) {
			forceAlpha.m_data[thread] = bodyInfo.m_forceTorque.m_data[thread] + internalForceVeloc.m_internalForce.m_data[thread];
		}
		__syncthreads();	
	
//		dgVector accel (force.Scale (body->m_invMass.m_w));
//		dgVector alpha (body->m_invWorldInertiaMatrix.RotateVector (torque));
		tmp[thread] = forceAlpha.m_angular[row] * bodyInfo.m_invInertiaMatrix[thread];
		__syncthreads();
		if (thread < 4) {
			accelAlpha.m_linear[thread] = forceAlpha.m_linear[thread] * bodyInfo.m_invInertia[3];
			accelAlpha.m_angular[thread] = tmp[thread] + tmp[thread + 4] + tmp[thread + 8];		
		}
		__syncthreads();
		
//		body->m_veloc += accel.Scale(m_timeStep);
//		body->m_omega += alpha.Scale(m_timeStep);
//		m_internalVeloc[i].m_linear += body->m_veloc;
//		m_internalVeloc[i].m_angular += body->m_omega;
		if (thread < 8) {
			bodyInfo.m_veloOmega.m_data[thread] += accelAlpha.m_data[thread] * timestep;
			internalForceVeloc.m_internalVeloc.m_data[thread] += bodyInfo.m_veloOmega.m_data[thread];
		}
		__syncthreads();
		
		bodyArray[block].m_data0[thread] = bodyInfo.m_data0[thread];
		bodyForceVeloc[block].m_data0[thread] = internalForceVeloc.m_data0[thread];
	}
}




__global__ void InitInternalForces (
	int* const jointIndex,
	dgCudaJointGraph::dgJointInfo* const constraintInfo,
	dgCudaBody::dgBodyInfo* const bodyArray,
	dgCudaBody::dgBodyWorkData* const bodyInternalForces,
	dgCudaJacobianMemory::dgJacobianInfo* const jacobianMemory)
{
	int block;
	int thread;
	int m0;
	int m1;
	int count;
	int first;
	float force;
	__shared__ dgCudaJointGraph::dgJointInfo jointInfo;
	__shared__ dgCudaJacobianMemory::dgJacobianPair internalForce;
	
	block = jointIndex[blockIdx.x];
	thread = threadIdx.x;

	jointInfo.m_data0[thread] = constraintInfo[block].m_data0[thread];
	
	internalForce.m_jacobianData[thread] = 0.0f;
	__syncthreads();

	m0 = jointInfo.m_m0;
	m1 = jointInfo.m_m1;
	count = jointInfo.m_autoPaircount;
	first = jointInfo.m_autoPairstart;
	for (int j = 0; j < count; j ++) { 
		force = jacobianMemory[first].m_force;
		internalForce.m_jacobianData[thread] += jacobianMemory[first].m_Jt.m_jacobianData[thread] * force;
		first ++;
	}
	__syncthreads();


	if (thread < 8) {
		bodyInternalForces[m0].m_internalForce.m_data[thread] += internalForce.m_jacobian_IM0.m_data[thread];
		bodyInternalForces[m1].m_internalForce.m_data[thread] += internalForce.m_jacobian_IM1.m_data[thread];		
	}
}

/*
__global__ void InertiaMatricesAndKineticForces (dgCudaBody::dgBodyInfo* const bodies)
{
	int row0;
	int col0;
	int col1;
	int col2;
	int col3;
	int block;
	int blockBase;
	int thread;
	int row0Scale;
	int col0Scale;
	int col1Scale;
	int col2Scale;
	int col3Scale;
	int vectorEntry;	
	float val;
	__shared__ dgCudaBody::dgBodyInfo bodyInfo[1<<DG_CLUSTER_TREAD_SIZE_BITS];
	__shared__ float tmp [1<<DG_CLUSTER_TREAD_SIZE_BITS][16]; 
	__shared__ float transp [1<<DG_CLUSTER_TREAD_SIZE_BITS][16]; 
	
	block = blockIdx.x << DG_CLUSTER_TREAD_SIZE_BITS;
	thread = threadIdx.x & 15;
	blockBase = threadIdx.x >> 4;
	vectorEntry = threadIdx.x >> 2;
	
	row0 = thread >> 2;
	col0 = thread & 3;
	col1 = (thread + 1) & 3;
	col2 = (thread + 2) & 3;
	col3 = (thread + 3) & 3;
	
	row0Scale = row0 << 2;
	col0Scale = col0 << 2;
	col1Scale = col1 << 2;
	col2Scale = col2 << 2;
	col3Scale = col3 << 2;
	
	bodyInfo[blockBase].m_matrix[thread] = bodies[block + blockBase].m_matrix[thread];
	bodyInfo[blockBase].m_data0[thread] = bodies[block + blockBase].m_data0[thread];
	bodyInfo[blockBase].m_data1[thread] = bodies[block + blockBase].m_data1[thread];
	__syncthreads();

	// calculate the inertia Matrix
	val = (col0 < 3) ? bodyInfo[blockBase].m_invInertia[col0] : 0.0f;
	transp[blockBase][thread] = bodyInfo[blockBase].m_matrix[col0Scale + row0] * val;


//	MatrixMultiply(accMatrix, transp, tmpMatrix);	
	bodyInfo[blockBase].m_invInertiaMatrix[thread] = transp[blockBase][row0Scale + col0] * bodyInfo[blockBase].m_matrix[col0Scale + col0] +
													 transp[blockBase][row0Scale + col1] * bodyInfo[blockBase].m_matrix[col1Scale + col0] +
										             transp[blockBase][row0Scale + col2] * bodyInfo[blockBase].m_matrix[col2Scale + col0] +
										             transp[blockBase][row0Scale + col3] * bodyInfo[blockBase].m_matrix[col3Scale + col0];


//	dgVector omega (m_matrix.UnrotateVector (m_omega));
//	MatrixUnrotateVector(tmpOmega, tmpOmega, tmpMatrix);
	tmp[blockBase][thread] = bodyInfo[blockBase].m_omega[col0] * bodyInfo[blockBase].m_matrix[thread];
	__syncthreads();		

	if (threadIdx.x < (4 << DG_CLUSTER_TREAD_SIZE_BITS)) {
//		bodyInfo.m_omega[thread] = tmp[col0Scale] + tmp[col0Scale+1] + tmp[col0Scale+2];		
		val = tmp[vectorEntry][col0Scale] + tmp[vectorEntry][col0Scale + 1] + tmp[vectorEntry][col0Scale + 2];		
		
		//m_veloc -= m_veloc.Scale (m_dampCoef.m_w);
		bodyInfo[vectorEntry].m_veloc[col0] -= bodyInfo[vectorEntry].m_veloc[col0] * bodyInfo[vectorEntry].m_dampCoef[3];
		
		//omega -= omega.CompProduct (m_dampCoef);
//		bodyInfo.m_omega[thread] -= bodyInfo.m_omega[thread] * bodyInfo.m_dampCoef[col0];
		bodyInfo[vectorEntry].m_omega[col0] = val - val * bodyInfo[vectorEntry].m_dampCoef[col0];
	}
	__syncthreads();
	
//	m_omega = m_matrix.RotateVector (omega);
//	MatrixRotateVector(tmpOmega, tmpOmega, tmpMatrix);
	tmp[blockBase][thread] = bodyInfo[blockBase].m_omega[row0] * bodyInfo[blockBase].m_matrix[thread];
	__syncthreads();
	if (threadIdx.x < (4 << DG_CLUSTER_TREAD_SIZE_BITS)) {
		val = tmp[vectorEntry][col0] + tmp[vectorEntry][col0 + 4] + tmp[vectorEntry][col0 + 8];		
		
		bodyInfo[vectorEntry].m_omega[thread] = val;		
		bodyInfo[vectorEntry].m_netForce.m_angular[col0] = val;
		bodyInfo[vectorEntry].m_netForce.m_linear[col0] = bodyInfo[vectorEntry].m_veloc[col0];
	}
	__syncthreads();

	bodies[block + blockBase].m_data0[thread] = bodyInfo[blockBase].m_data0[thread];
	bodies[block + blockBase].m_data2[thread] = bodyInfo[blockBase].m_data2[thread];
	bodies[block + blockBase].m_invInertiaMatrix[thread] = bodyInfo[blockBase].m_invInertiaMatrix[thread];
}
*/

__global__ void InertiaMatricesAndKineticForces (dgCudaBody::dgBodyInfo* const bodies)
{
	int row;
	int colum;
	int block;
	int thread;
	int rowScale;
	int columScale;
	float val;
	__shared__ dgCudaBody::dgBodyInfo bodyInfo;
	__shared__ float tmp [16]; 
	__shared__ float transp [16]; 
	
	block = blockIdx.x;
	thread = threadIdx.x;
	
	row = thread >> 2;
	colum = thread & 3;
	rowScale = row << 2;
	columScale = thread << 2;

	bodyInfo.m_matrix[thread] = bodies[block].m_matrix[thread];
	bodyInfo.m_data0[thread] = bodies[block].m_data0[thread];
	bodyInfo.m_data1[thread] = bodies[block].m_data1[thread];
	__syncthreads();

	// calculate the inertia Matrix
	transp[rowScale + colum] = bodyInfo.m_matrix[(colum << 2) + row] * bodyInfo.m_invInertia[colum];
	__syncthreads();	

//	MatrixMultiply(accMatrix, transp, tmpMatrix);	
	bodyInfo.m_invInertiaMatrix[rowScale + colum] = transp[rowScale + 0] * bodyInfo.m_matrix[0 + colum] +
												    transp[rowScale + 1] * bodyInfo.m_matrix[4 + colum] +
												    transp[rowScale + 2] * bodyInfo.m_matrix[8 + colum];
	
//	dgVector omega (m_matrix.UnrotateVector (m_omega));
//	MatrixUnrotateVector(tmpOmega, tmpOmega, tmpMatrix);
	tmp[thread] = bodyInfo.m_omega[colum] * bodyInfo.m_matrix[thread];
	__syncthreads();		

	if (thread < 4) {
		bodyInfo.m_omega[thread] = tmp[columScale] + tmp[columScale+1] + tmp[columScale+2];		
		
		//m_veloc -= m_veloc.Scale (m_dampCoef.m_w);
		bodyInfo.m_veloc[thread] -= bodyInfo.m_veloc[thread] * bodyInfo.m_dampCoef[3];
	}
	__syncthreads();			
	
	if (thread < 4) {
		//omega -= omega.CompProduct (m_dampCoef);
		bodyInfo.m_omega[thread] -= bodyInfo.m_omega[thread] * bodyInfo.m_dampCoef[thread];
	}
	__syncthreads();
	
//	m_omega = m_matrix.RotateVector (omega);
//	MatrixRotateVector(tmpOmega, tmpOmega, tmpMatrix);
	tmp[thread] = bodyInfo.m_omega[row] * bodyInfo.m_matrix[thread];
	__syncthreads();
	if (thread < 4) {
		val = tmp[thread] + tmp[thread + 4] + tmp[thread + 8];		
		
		bodyInfo.m_omega[thread] = val;		
		bodyInfo.m_netForce.m_angular[thread] = val;
		bodyInfo.m_netForce.m_linear[thread] = bodyInfo.m_veloc[thread];
	}
	__syncthreads();

	bodies[block].m_data0[thread] = bodyInfo.m_data0[thread];
	bodies[block].m_data2[thread] = bodyInfo.m_data2[thread];
	bodies[block].m_invInertiaMatrix[thread] = bodyInfo.m_invInertiaMatrix[thread];
}





#if 0
__global__ void CalculateJointForces (
	int* const jointIndex,
	dgCudaJointGraph::dgJointInfo* const constraintInfo,
	dgCudaBody::dgBodyInfo* const bodyArray,
	dgCudaBody::dgBodyWorkData* const bodyForceVeloc,
	dgCudaJacobianMemory::dgJacobianInfo* const jacobianMemory)
{
	int m0;
	int m1;
	int count;
	int first;
	int block;
	int thread;
	int frictionIndex;
	float a;
	float f;
	float frictionNormal;
	float lowerFrictionForce;
	float upperFrictionForce;
	__shared__ float prevValue;
	__shared__ dgCudaJointGraph::dgJointInfo jointInfo;
	__shared__ dgCudaJacobianMemory::dgJacobianPair rowAccel;	
	__shared__ dgCudaJacobianMemory::dgJacobianInfo jacobianRow;	
	__shared__ dgCudaJacobianMemory::dgJacobianPair internalForce;	

//	block = blockIdx.x;
	block = jointIndex[blockIdx.x];
	thread = threadIdx.x;
	
	jointInfo.m_data0[thread] = constraintInfo[block].m_data0[thread];	
	__syncthreads();


//	currJoint = threadIndex + i;
//	index = m_constraintArray[currJoint].m_autoPairstart;
//	rowsCount = m_constraintArray[currJoint].m_autoPaircount;
//	m0 = m_constraintArray[currJoint].m_m0;
//	m1 = m_constraintArray[currJoint].m_m1;

	m0 = jointInfo.m_m0;
	m1 = jointInfo.m_m1;
	count = jointInfo.m_autoPaircount;
	first = jointInfo.m_autoPairstart;


//	dgVector linearM0 (m_internalForces[m0].m_linear);
//	dgVector angularM0 (m_internalForces[m0].m_angular);
//	dgVector linearM1 (m_internalForces[m1].m_linear);
//	dgVector angularM1 (m_internalForces[m1].m_angular);
	if (thread < 8) {
		internalForce.m_jacobian_IM0.m_data[thread] =  bodyForceVeloc[m0].m_internalForce.m_data[thread];
		internalForce.m_jacobian_IM1.m_data[thread] =  bodyForceVeloc[m1].m_internalForce.m_data[thread];
	}
	prevValue = 0.0f;
	__syncthreads();	
	
	for (int k = 0; k < count; k ++) {
//		dgVector acc (m_JMinv[index].m_jacobian_IM0.m_linear.CompProduct(linearM0));
//		acc += m_JMinv[index].m_jacobian_IM0.m_angular.CompProduct (angularM0);
//		acc += m_JMinv[index].m_jacobian_IM1.m_linear.CompProduct (linearM1);
//		acc += m_JMinv[index].m_jacobian_IM1.m_angular.CompProduct (angularM1);
		rowAccel.m_jacobianData[thread] = jacobianMemory[first].m_JMinv.m_jacobianData[thread] * internalForce.m_jacobianData[thread];
		jacobianRow.m_data0[thread] = jacobianMemory[first].m_data0[thread];		
		__syncthreads();	

		for (int n = 8; n; n >>= 1) {		
			if (thread < n) {	
				rowAccel.m_jacobianData[thread] += rowAccel.m_jacobianData[thread + n];
			}
			__syncthreads();			
		}

		if (thread == 0) {
//			a = m_coordenateAccel[index] - acc.m_x - acc.m_y - acc.m_z - m_force[index] * m_diagDamp[index];
			a = jacobianRow.m_coordenateAccel - rowAccel.m_jacobianData[0] - jacobianRow.m_force * jacobianRow.m_diagDamp;
			
//			f = m_force[index] + m_invDJMinvJt[index] * a;
			f = jacobianRow.m_force + jacobianRow.m_invDJMinvJt * a;

//			frictionIndex = m_normalForceIndex[index];
//			_ASSERTE (((frictionIndex < 0) && (m_force[frictionIndex] == dgFloat32 (1.0f))) || ((frictionIndex >= 0) && (m_force[frictionIndex] >= dgFloat32 (0.0f))));
//			frictionNormal = m_force[frictionIndex];
			
			frictionNormal = 1.0f;
			frictionIndex = jacobianRow.m_normalForceIndex;
			if (frictionIndex >= 0) {
				frictionNormal = jacobianMemory[frictionIndex].m_force;
			} 

//			lowerFrictionForce = frictionNormal * m_lowerFrictionCoef[index];
//			upperFrictionForce = frictionNormal * m_upperFrictionCoef[index];
			lowerFrictionForce = frictionNormal * jacobianRow.m_lowerBoundFrictionCoefficent;
			upperFrictionForce = frictionNormal * jacobianRow.m_upperBoundFrictionCoefficent;

			if (f > upperFrictionForce) {
				a = dgFloat32 (0.0f);
				f = upperFrictionForce;
			} else if (f < lowerFrictionForce) {
				a = dgFloat32 (0.0f);
				f = lowerFrictionForce;
			}

//			accNorm = GetMax (accNorm, dgAbsf (a));
			prevValue = f - jacobianRow.m_force;
//			jacobianRow.m_force = f;
			jacobianMemory[first].m_force = f;
		}
		__syncthreads();	

//		linearM0 += m_Jt[index].m_jacobian_IM0.m_linear.Scale (prevValue);
//		angularM0 += m_Jt[index].m_jacobian_IM0.m_angular.Scale (prevValue);
//		linearM1 += m_Jt[index].m_jacobian_IM1.m_linear.Scale (prevValue);
//		angularM1 += m_Jt[index].m_jacobian_IM1.m_angular.Scale (prevValue);
		internalForce.m_jacobianData[thread] += jacobianMemory[first].m_Jt.m_jacobianData[thread] * prevValue;
		__syncthreads();	
		first ++;
	}


//	m_internalForces[m0].m_linear = linearM0;
//	m_internalForces[m0].m_angular = angularM0;
//	m_internalForces[m1].m_linear = linearM1;
//	m_internalForces[m1].m_angular = angularM1;
	if (thread < 8) {
		bodyForceVeloc[m0].m_internalForce.m_data[thread] = internalForce.m_jacobian_IM0.m_data[thread];  
		bodyForceVeloc[m1].m_internalForce.m_data[thread] = internalForce.m_jacobian_IM1.m_data[thread];
	}
}

#else

__global__ void CalculateJointForces (
	int* const jointIndex,
	dgCudaJointGraph::dgJointInfo* const constraintInfo,
	dgCudaBody::dgBodyInfo* const bodyArray,
	dgCudaBody::dgBodyWorkData* const bodyForceVeloc,
	dgCudaJacobianMemory::dgJacobianInfo* const jacobianMemory)
{
	int m0;
	int m1;
	int count;
	int first;
	int block;
	int thread;
	int frictionIndex;
	float a;
	float f;
	float frictionNormal;
	float lowerFrictionForce;
	float upperFrictionForce;
	__shared__ float prevValue;
	__shared__ dgCudaJointGraph::dgJointInfo jointInfo;
	__shared__ dgCudaJacobianMemory::dgJacobianPair rowAccel;	
	__shared__ dgCudaJacobianMemory::dgJacobianInfo jacobianRow;	
	__shared__ dgCudaJacobianMemory::dgJacobianPair internalForce;	

//	block = blockIdx.x;
	block = jointIndex[blockIdx.x];
	thread = threadIdx.x;
	
	jointInfo.m_data0[thread] = constraintInfo[block].m_data0[thread];	
	__syncthreads();

	m0 = jointInfo.m_m0;
	m1 = jointInfo.m_m1;
	count = jointInfo.m_autoPaircount;
	first = jointInfo.m_autoPairstart;

	if (thread < 8) {
		internalForce.m_jacobian_IM0.m_data[thread] =  bodyForceVeloc[m0].m_internalForce.m_data[thread];
		internalForce.m_jacobian_IM1.m_data[thread] =  bodyForceVeloc[m1].m_internalForce.m_data[thread];
	}
	prevValue = 0.0f;
	__syncthreads();	
	
	for (int k = 0; k < count; k ++) {
		rowAccel.m_jacobianData[thread] = jacobianMemory[first].m_JMinv.m_jacobianData[thread] * internalForce.m_jacobianData[thread];
		jacobianRow.m_data0[thread] = jacobianMemory[first].m_data0[thread];		
		__syncthreads();	

//		for (int n = 8; n; n >>= 1) {		
//			rowAccel.m_jacobianData[thread] += rowAccel.m_jacobianData[(thread + n) & 15];
//		}
		rowAccel.m_jacobianData[thread] += rowAccel.m_jacobianData[(thread + 8) & 15];
		rowAccel.m_jacobianData[thread] += rowAccel.m_jacobianData[(thread + 4) & 15];
		rowAccel.m_jacobianData[thread] += rowAccel.m_jacobianData[(thread + 2) & 15];
		rowAccel.m_jacobianData[thread] += rowAccel.m_jacobianData[(thread + 1) & 15];

		a = jacobianRow.m_coordenateAccel - rowAccel.m_jacobianData[0] - jacobianRow.m_force * jacobianRow.m_diagDamp;
		f = jacobianRow.m_force + jacobianRow.m_invDJMinvJt * a;
		frictionIndex = jacobianRow.m_normalForceIndex;
		frictionNormal = (frictionIndex >= 0) ? jacobianMemory[frictionIndex].m_force : 1.0f;
		lowerFrictionForce = frictionNormal * jacobianRow.m_lowerBoundFrictionCoefficent;
		upperFrictionForce = frictionNormal * jacobianRow.m_upperBoundFrictionCoefficent;
		if (f > upperFrictionForce) {
			f = upperFrictionForce;
		} else if (f < lowerFrictionForce) {
			f = lowerFrictionForce;
		}

		prevValue = f - jacobianRow.m_force;
		jacobianMemory[first].m_force = f;
//		__syncthreads();	

		internalForce.m_jacobianData[thread] += jacobianMemory[first].m_Jt.m_jacobianData[thread] * prevValue;
//		__syncthreads();	
		first ++;
	}

	if (thread < 8) {
		bodyForceVeloc[m0].m_internalForce.m_data[thread] = internalForce.m_jacobian_IM0.m_data[thread];  
		bodyForceVeloc[m1].m_internalForce.m_data[thread] = internalForce.m_jacobian_IM1.m_data[thread];
	}
}


#endif