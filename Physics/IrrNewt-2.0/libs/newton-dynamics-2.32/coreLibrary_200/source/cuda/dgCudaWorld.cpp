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

#include "cudaStdafx.h"
//#include "dgCudaWorld.h"
#include "dgWorld.h"
#include "driver_types.h"
#include "cuda_runtime_api.h"



//int  GetCudaDevice ();
//void InitCudaDevice (int device, char *name);

dgCudaWorld::dgCudaWorld(int device)
	:dgCudaSolver(), dgCudaBroadPhase()
{
	cudaError_t result;
	cudaDeviceProp prop;

	result = cudaGetDeviceProperties (&prop, device);
	_ASSERTE (result == cudaSuccess);

	dgCudaSolver::m_version = prop.major * 100 + prop.minor;
dgCudaSolver::m_version = 100;
	strcpy (m_name, prop.name);

	
		
//	result = cudaChooseDevice (&m_device, &prop);
//	_ASSERTE (result == cudaSuccess);
//	result = cudaGetSymbolAddress(&m_returnValuePtr, m_returnValue);
//	_ASSERTE (result == cudaSuccess);
//	result = cudaGetSymbolAddress(&m_userGlobalLockPtr, m_userGlobalLock);
//	_ASSERTE (result == cudaSuccess);
}

dgCudaWorld::~dgCudaWorld(void)
{
//		_ASSERTE (0);
}


dgCudaWorld* dgCudaWorld::Create (void)
{
	int count;
	int bestDevice;	
	cudaError_t result;

	bestDevice = -1;
	//initialize the CUDA driver API 
	result = cudaGetDeviceCount (&count);
	if (result == cudaSuccess) { 

		int bestValue;

		bestValue = 0;
		for (int i = 0; i < count; i ++) {
			int major;
			cudaDeviceProp prop;
			result = cudaGetDeviceProperties (&prop, i);
			if (result == cudaSuccess) {
				major = prop.major * 100 + prop.minor;
				if (major > bestValue) { 
					bestValue = major;
					bestDevice = i;
				}
			}
		}
	} 

	if (bestDevice >= 0) {	
		cudaError_t result;
		cudaDeviceProp prop;

		result = cudaGetDeviceProperties (&prop, bestDevice);
		_ASSERTE (result == cudaSuccess);
		
		result = cudaChooseDevice (&bestDevice, &prop);
		_ASSERTE (result == cudaSuccess);
		return new dgCudaWorld (bestDevice);
	}

	return NULL;
}

void dgCudaWorld::Destroy ()
{
	cudaError_t result;

	delete this;
	result = cudaThreadExit();
	_ASSERTE (result == cudaSuccess);
}

void dgCudaWorld::Update (dgWorld* world, dgFloat32 timestep)
{
	simd_env rounding;

	rounding = simd_get_ctrl();
	simd_set_FZ_mode();

//	world->UpdateContactsSimd (timestep, false);
	world->UpdateContacts (timestep, false);

	UpdateDynamics (world, timestep);

	simd_set_ctrl (rounding);
}





