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

#ifndef __CUDA_SOLVER_KERNERS__
#define __CUDA_SOLVER_KERNERS__


#include "dgVector.h"
#include "dgMatrix.h"
#include "dgMemory.h"


class dgBody;
class dgConstraint;
void* CudaMalloc(int size);
void CudaFree(void* ptr);


#define DG_CLUSTER_TREAD_SIZE_BITS			 0


template<class T>
class dgDeviceArray
{
	public:
	~dgDeviceArray()
	{
		if (m_data) {
			Free ();
		}
	}

	void Init (int size)
	{
		m_data = (T*) CudaMalloc(size * sizeof (T));
	}

	void Free ()
	{
		CudaFree (m_data);
		m_data = NULL;
	}

	void Realloc (int newSize)
	{
		Free ();
		Init (newSize);
	}

	T& operator[] (int i) { return m_data[i]; }
	const T& operator[] (int i) const { return m_data[i]; }
	
	T* m_data;
};


template<class T>
class dgSystemArray
{
	public:
	~dgSystemArray()
	{
		if (m_data) {
			Free ();
		}
	}

	void Init (int size)
	{
		m_data = (T*) dgMallocAligned (size * sizeof (T), 256);
	}

	void Free ()
	{
		dgFreeStack (m_data);
		m_data = NULL;
	}

	void Realloc (int newSize, int copySize)
	{
		T* tmp;
		tmp = m_data;
		Init (newSize);
		memcpy (m_data, tmp, copySize * sizeof(T));
		dgFreeStack (tmp);
	}


	T& operator[] (int i) { return m_data[i]; }
	const T& operator[] (int i) const { return m_data[i]; }

	T* m_data;
};


class dgCudaJacobian
{
	public:
	union {
		float m_data[8];
		struct {
			float m_linear[4];
			float m_angular[4];
		};
	};
};

class dgCudaBody
{	
	public:
	struct dgBodyInfo
	{
		float m_matrix[16];
		float m_invInertiaMatrix[16];

		union {
			float m_data0[16]; 
			struct {
				dgCudaJacobian m_veloOmega;
				dgCudaJacobian m_forceTorque;
			};
			struct {
				float m_veloc[4];
				float m_omega[4];
				float m_accel[4];
				float m_alpha[4];
			};
		};

		union {
			float m_data1[16]; 
			struct {
				float m_dampCoef[4];
				float m_invInertia[4];
				float m_globalCentreOfMass[4];
				int m_lock;
				dgBody* m_body;
			};
		};

		union {
			float m_data2[16]; 
			struct {
				dgCudaJacobian m_netForce;
			};
		};
	};


	struct dgBodyWorkData
	{
		union {
			float m_data0[16]; 
			struct {
				dgCudaJacobian m_internalVeloc;
				dgCudaJacobian m_internalForce;
			};
		};
	};

	void Init (dgInt32 count)
	{
		m_maxCount = count;
		m_sysBody.Init (m_maxCount);
		m_devBody.Init (m_maxCount);

		m_sysBodyWorkData.Init (m_maxCount);
		m_devBodyWorkData.Init (m_maxCount);
	}

	void Free ()
	{
		m_sysBody.Free();
		m_devBody.Free();

		m_sysBodyWorkData.Free ();
		m_devBodyWorkData.Free ();
	}

	void ReAlloc (int copyCount)
	{
		_ASSERTE (m_maxCount * 2 > m_count);
		_ASSERTE (m_maxCount * 2 > copyCount);
		
		m_maxCount *= 2;

		m_devBody.Realloc (m_maxCount);
		m_sysBody.Realloc (m_maxCount, copyCount);

		m_devBodyWorkData.Realloc (m_maxCount);
		m_sysBodyWorkData.Realloc (m_maxCount, 0);
	}


	int m_count;
	int m_maxCount;
	dgSystemArray<dgBodyInfo> m_sysBody;
	dgDeviceArray<dgBodyInfo> m_devBody;
	dgDeviceArray<dgBodyWorkData> m_devBodyWorkData;
	dgSystemArray<dgBodyWorkData> m_sysBodyWorkData;
};


class dgCudaJointsSlack
{
	public:
	class dgSlack
	{
		public:
		void Init (dgInt32 count)
		{
			m_maxCount = count;
			m_sysJointSlack.Init (m_maxCount);
			m_devJointSlack.Init (m_maxCount);
		}

		void Free ()
		{
			m_sysJointSlack.Free();
			m_devJointSlack.Free();
		}

		void ReAlloc (int copyCount)
		{
			_ASSERTE (m_maxCount * 2 > m_count);
			_ASSERTE (m_maxCount * 2 > copyCount);
			
			m_maxCount *= 2;

			m_devJointSlack.Realloc (m_maxCount);
			m_sysJointSlack.Realloc (m_maxCount, copyCount);
		}

		int m_count;
		int m_maxCount;
		dgDeviceArray<int> m_devJointSlack;
		dgSystemArray<int> m_sysJointSlack;
	};

	void Init (dgInt32 count)
	{
		for (dgInt32 i = 0; i < sizeof (m_slackArray) / sizeof (m_slackArray[0]); i ++) {
			m_slackArray[i].Init (count);
		}
	}

	void Free ()
	{
		for (dgInt32 i = 0; i < sizeof (m_slackArray) / sizeof (m_slackArray[0]); i ++) {
			m_slackArray[i].Free();
		}
	}

	dgSlack m_slackArray[32];
};

class dgCudaJointGraph
{
	public:
	struct dgJointInfo
	{
		union {
			float m_data0[16];
			struct {
				int m_autoPairstart;
				int m_autoPaircount;
				int m_autoPairActiveCount;
				int m_m0;
				int m_m1;
				unsigned m_code;
				int m_contactStart;
				dgConstraint* m_joint;
			};
		}; 
	};

	void Init (dgInt32 count)
	{
		m_maxCount = count;
		m_sysConstraintArray.Init (m_maxCount);
		m_devConstraintArray.Init (m_maxCount);
	}

	void Free ()
	{
		m_sysConstraintArray.Free();
		m_devConstraintArray.Free();
	}

	void ReAlloc (int copyCount)
	{
		_ASSERTE (m_maxCount * 2 > m_count);
		_ASSERTE (m_maxCount * 2 > copyCount);

		m_maxCount *= 2;
		m_sysConstraintArray.Realloc (m_maxCount, copyCount);
		m_devConstraintArray.Realloc (m_maxCount);
	}

	int m_count;
	int m_maxCount;
	dgSystemArray<dgJointInfo> m_sysConstraintArray; 
	dgDeviceArray<dgJointInfo> m_devConstraintArray; 
};

class dgCudaContactMemory
{
	public:
	class dgContact
	{
		public:
		union {
			float m_data0[16];
			struct {
				float m_point[4];
				float m_normal[4];
				float m_dir0[4];
				float m_dir1[4];
			};
		};

		union {
			float m_data1[16];
			struct {
				float m_normal_Force;
				float m_dir0_Force;
				float m_dir1_Force;
				float m_softness;

				float m_penetration;
				float m_restitution;
				float m_staticFriction0;
				float m_staticFriction1;

				float m_dynamicFriction0;
				float m_dynamicFriction1;
				int m_flags;

				void* m_normal_ForcePtr;
				void* m_dir0_ForcePtr;
				void* m_dir1_ForcePtr;
			};
		};
	};

	void Init(dgInt32 count)
	{
		m_maxCount = count;
		m_sysContacts.Init (m_maxCount);;
		m_devContacts.Init (m_maxCount);; 
	}

	void Free()
	{
		m_sysContacts.Free();
		m_devContacts.Free();
	}

	void Realloc(int copyCount)
	{
		_ASSERTE (m_maxCount * 2 > m_count);
		_ASSERTE (m_maxCount * 2 > copyCount);

		m_maxCount *= 2;

		m_devContacts.Realloc (m_maxCount);
		m_sysContacts.Realloc (m_maxCount, copyCount);
	}

	int m_count;
	int m_maxCount;
	dgSystemArray<dgContact> m_sysContacts;
	dgDeviceArray<dgContact> m_devContacts;
};


class dgCudaJacobianMemory
{
	public:
	class dgJacobianPair
	{
		public:
		union {
			float m_jacobianData[16];
			struct {
				dgCudaJacobian m_jacobian_IM0;
				dgCudaJacobian m_jacobian_IM1;
			};
		};
	};

	struct dgJacobianInfo
	{
		dgJacobianPair m_Jt;
		dgJacobianPair m_JMinv;
		union {
			float m_data0[16];
			struct {
				float m_diagDamp;			                        //0						
				float m_coordenateAccel;							//1	
				float m_restitution;								//2	
				float m_penetration;								//3	
				float m_penetrationStiffness;						//4	
				float m_lowerBoundFrictionCoefficent;				//5
				float m_upperBoundFrictionCoefficent;				//6
				float m_jointFeebackForce;							//7
				float m_force;										//8
				float m_deltaAccel;									//9
				float m_invDJMinvJt;								//10
				int m_accelIsMotor;									//11
				int m_normalForceIndex;								//12
				void* m_jointFeebackForcePtr;						//13
			};														
		};															   
	};

	void Init (dgInt32 count)
	{
		m_maxCount = count;
		m_sysJacobian.Init (m_maxCount);
		m_devJacobian.Init (m_maxCount);
	}

	void Free ()
	{
		m_sysJacobian.Free ();
		m_devJacobian.Free ();
	}

	void Realloc (int copyCount)
	{
		m_maxCount *= 2;
		m_sysJacobian.Realloc (m_maxCount, copyCount);
		m_devJacobian.Realloc (m_maxCount);
	}
		
	int m_count;
	int m_maxCount;
	dgSystemArray<dgJacobianInfo> m_sysJacobian;
	dgDeviceArray<dgJacobianInfo> m_devJacobian;
};



class dgCudaActiveIslands
{
	public:
	struct dgIsland
	{
		dgInt32 m_start;
		dgInt32 m_count;
	};

	class dgBodtList
	{
		public:
		void Init(dgInt32 count)
		{
			m_maxCount = count;
			m_pool.Init (m_maxCount);;
		}

		void Free()
		{
			m_pool.Free();
		}

		void Realloc(int copyCount)
		{
			_ASSERTE (m_maxCount * 2 > m_count);

			m_maxCount *= 2;
			m_pool.Realloc (m_maxCount, copyCount);
		}

		int m_count;
		int m_maxCount;
		dgSystemArray<dgBody*> m_pool;
	};

	void Clear()
	{
		m_count = 0;
		m_bodyList.m_count = 0;
	}

	void Init(dgInt32 count)
	{
		m_maxCount = count;
		m_sysIsland.Init (m_maxCount);
		m_bodyList.Init (m_maxCount);
	}

	void Free()
	{
		m_sysIsland.Free();
		m_bodyList.Free();
	}

	void Realloc(int copyCount)
	{
		_ASSERTE (m_maxCount * 2 > m_count);

		m_maxCount *= 2;
		m_sysIsland.Realloc (m_maxCount, copyCount);
	}

	int m_count;
	int m_maxCount;
	dgBodtList m_bodyList;
	dgSystemArray<dgIsland> m_sysIsland;
};



void CalculateInertiaMatricesAndBodyKineticForces (dgCudaBody& bodies);
void CalculateContactDerivatives (dgCudaJointGraph& contraints, dgCudaContactMemory& contacts, 
								 dgCudaJacobianMemory& m_jacobianMemory, dgCudaBody& bodyArray, float timestep);	
void CalculateJacobianMatrix (dgCudaJointGraph& contraints, dgCudaBody& bodyArray, dgCudaJacobianMemory& jacobianMemory);	
void CalculateRowsAcceleration (dgCudaJointGraph& contraints, dgCudaBody& bodyArray, dgCudaJacobianMemory& jacobianMemory, 
							   float timestep, float invTimestep, float firstPassCoef);

void InitInternalForces (int deviceVersion, dgCudaJointsSlack& slack, dgCudaJointGraph& contraints, dgCudaBody& bodyArray, dgCudaJacobianMemory& jacobianMemory);	
void CalculateJointForces (int deviceVersion, dgCudaJointsSlack& slack, dgCudaJointGraph& contraints, dgCudaBody& bodyArray, dgCudaJacobianMemory& jacobianMemory); 
void UpdateBodiesVelocities (dgCudaBody& bodyArray, float timestep); 


#endif
