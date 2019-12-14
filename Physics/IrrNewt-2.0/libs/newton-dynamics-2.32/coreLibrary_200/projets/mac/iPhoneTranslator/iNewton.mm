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

//
//  iNewton.mm
//  newton_iPhone
//
//  Created by Julio Jerez on 7/27/09.
//  Copyright 2009 Newton Game Dynamics. All rights reserved.
//

#import "iNewton.h"
#include "Newton.h"
#include "JointLibrary.h"


@implementation iNewton


-(id) init
{
	[super init];
	m_world = NewtonCreate();
	NewtonSetPlatformArchitecture((const NewtonWorld*) m_world, 0);
	// set the linear solver model for faster speed 
	NewtonSetSolverModel ((const NewtonWorld*) m_world, 0);
	
	NewtonSetThreadsCount((const NewtonWorld*) m_world, 1);
	
	// set the adaptive friction model for faster speed
	NewtonSetFrictionModel ((const NewtonWorld*) m_world, 0);
	
	return self;
}

-(id) initWithAllocator: (iNewtonAllocMemory) allocator: (iNewtonFreeMemory) deallocator;
{
	[super init];
	// Set the global memory allocator
	NewtonSetMemorySystem (allocator, deallocator);

	m_world = NewtonCreate();
	NewtonSetPlatformArchitecture((const NewtonWorld*) m_world, 0);
	// set the linear solver model for faster speed 
	NewtonSetSolverModel ((const NewtonWorld*) m_world, 0);
	
	NewtonSetThreadsCount((const NewtonWorld*) m_world, 1);
	
	// set the adaptive friction model for faster speed
	NewtonSetFrictionModel ((const NewtonWorld*) m_world, 0);
	
	return self;
}

-(void) dealloc
{
	NewtonDestroy((const NewtonWorld*) m_world);
	[super dealloc];
}

-(int) WorldGetVersion
{
	return (int) NewtonWorldGetVersion();
}

-(int) WorldFloatSize
{
	return (int) NewtonWorldFloatSize();
}

-(int) GetMemoryUsed
{
	return (int) NewtonGetMemoryUsed();
}

-(void) SetMemorySystem: (iNewtonAllocMemory) malloc: (iNewtonFreeMemory) mfree
{
	NewtonSetMemorySystem (( NewtonAllocMemory) malloc, ( NewtonFreeMemory) mfree);
}

-(struct iNewtonWorld*) Create
{
	return (struct iNewtonWorld*) NewtonCreate();
}

-(void) DestroyAllBodies
{
	NewtonDestroyAllBodies ((const NewtonWorld*) m_world);
}

-(void) Update: (float) timestep
{
	NewtonUpdate ((const NewtonWorld*) m_world, (float) timestep);
}

-(void) InvalidateCache
{
	NewtonInvalidateCache ((const NewtonWorld*) m_world);
}

-(void) CollisionUpdate
{
	NewtonCollisionUpdate ((const NewtonWorld*) m_world);
}

-(void) SetSolverModel: (int) model
{
	NewtonSetSolverModel ((const NewtonWorld*) m_world, (int) model);
}

-(void) SetPlatformArchitecture: (int) mode
{
	NewtonSetPlatformArchitecture ((const NewtonWorld*) m_world, (int) mode);
}

-(int) GetPlatformArchitecture: (char*) description
{
	return (int) NewtonGetPlatformArchitecture((const NewtonWorld*) m_world, (char*) description);
}

-(void) SetMultiThreadSolverOnSingleIsland: (int) mode
{
	NewtonSetMultiThreadSolverOnSingleIsland ((const NewtonWorld*) m_world, (int) mode);
}

-(int) GetMultiThreadSolverOnSingleIsland
{
	return (int) NewtonGetMultiThreadSolverOnSingleIsland((const NewtonWorld*) m_world);
}

-(void) SetPerformanceClock: (iNewtonGetTicksCountCallback) callback
{
	NewtonSetPerformanceClock ((const NewtonWorld*) m_world, ( NewtonGetTicksCountCallback) callback);
}

-(unsigned) ReadPerformanceTicks: (unsigned) performanceEntry
{
	return (unsigned) NewtonReadPerformanceTicks((const NewtonWorld*) m_world, (unsigned) performanceEntry);
}

-(unsigned) ReadThreadPerformanceTicks: (unsigned) threadIndex
{
	return (unsigned) NewtonReadThreadPerformanceTicks((const NewtonWorld*) m_world, (unsigned) threadIndex);
}

-(void) WorldCriticalSectionLock
{
	NewtonWorldCriticalSectionLock ((const NewtonWorld*) m_world);
}

-(void) WorldCriticalSectionUnlock
{
	NewtonWorldCriticalSectionUnlock ((const NewtonWorld*) m_world);
}

-(void) SetThreadsCount: (int) threads
{
	NewtonSetThreadsCount ((const NewtonWorld*) m_world, (int) threads);
}

-(int) GetThreadsCount
{
	return (int) NewtonGetThreadsCount((const NewtonWorld*) m_world);
}

-(int) GetMaxThreadsCount
{
	return (int) NewtonGetMaxThreadsCount((const NewtonWorld*) m_world);
}

-(void) SetFrictionModel: (int) model
{
	NewtonSetFrictionModel ((const NewtonWorld*) m_world, (int) model);
}

-(void) SetMinimumFrameRate: (float) frameRate
{
	NewtonSetMinimumFrameRate ((const NewtonWorld*) m_world, (float) frameRate);
}

-(void) SetBodyLeaveWorldEvent: (iNewtonBodyLeaveWorld) callback
{
	NewtonSetBodyLeaveWorldEvent ((const NewtonWorld*) m_world, ( NewtonBodyLeaveWorld) callback);
}

-(void) SetWorldSize: (const float*) minPoint: (const float*) maxPoint
{
	NewtonSetWorldSize ((const NewtonWorld*) m_world, (const float*) minPoint, (const float*) maxPoint);
}

-(void) SetIslandUpdateEvent: (iNewtonIslandUpdate) islandUpdate
{
	NewtonSetIslandUpdateEvent ((const NewtonWorld*) m_world, ( NewtonIslandUpdate) islandUpdate);
}

-(void) SetCollisionDestructor: (iNewtonCollisionDestructor) callback
{
	NewtonSetCollisionDestructor ((const NewtonWorld*) m_world, ( NewtonCollisionDestructor) callback);
}

-(void) SetDestroyBodyByExeciveForce: (iNewtonDestroyBodyByExeciveForce) callback
{
	NewtonSetDestroyBodyByExeciveForce ((const NewtonWorld*) m_world, ( NewtonDestroyBodyByExeciveForce) callback);
}

-(void) WorldForEachJointDo: (iNewtonJointIterator) callback: (void*) userData
{
	NewtonWorldForEachJointDo ((const NewtonWorld*) m_world, ( NewtonJointIterator) callback, (void*) userData);
}

-(void) WorldForEachBodyInAABBDo: (const float*) p0: (const float*) p1: (iNewtonBodyIterator) callback: (void*) userData
{
	NewtonWorldForEachBodyInAABBDo ((const NewtonWorld*) m_world, (const float*) p0, (const float*) p1, ( NewtonBodyIterator) callback, (void*) userData);
}

-(void) WorldSetUserData: (void*) userData
{
	NewtonWorldSetUserData ((const NewtonWorld*) m_world, (void*) userData);
}

-(void*) WorldGetUserData
{
	return (void*) NewtonWorldGetUserData((const NewtonWorld*) m_world);
}

-(void) WorldSetDestructorCallBack: (iNewtonDestroyWorld) destructor
{
	NewtonWorldSetDestructorCallBack ((const NewtonWorld*) m_world, ( NewtonDestroyWorld) destructor);
}

-(iNewtonDestroyWorld) WorldGetDestructorCallBack
{
	return (iNewtonDestroyWorld) NewtonWorldGetDestructorCallBack((const NewtonWorld*) m_world);
}

-(void) WorldRayCast: (const float*) p0: (const float*) p1: (iNewtonWorldRayFilterCallback) filter: (void*) userData: (iNewtonWorldRayPrefilterCallback) prefilter
{
	NewtonWorldRayCast ((const NewtonWorld*) m_world, (const float*) p0, (const float*) p1, ( NewtonWorldRayFilterCallback) filter, (void*) userData, ( NewtonWorldRayPrefilterCallback) prefilter);
}

-(int) WorldConvexCast: (const float*) matrix: (const float*) target: (const struct iNewtonCollision*) shape: (float*) hitParam: (void*) userData: (iNewtonWorldRayPrefilterCallback) prefilter: (struct iNewtonWorldConvexCastReturnInfo*) info: (int) maxContactsCount: (int) threadIndex
{
	return (int) NewtonWorldConvexCast((const NewtonWorld*) m_world, (const float*) matrix, (const float*) target, (const struct  NewtonCollision*) shape, (float*) hitParam, (void*) userData, ( NewtonWorldRayPrefilterCallback) prefilter, (struct  NewtonWorldConvexCastReturnInfo*) info, (int) maxContactsCount, (int) threadIndex);
}

-(int) WorldGetBodyCount
{
	return (int) NewtonWorldGetBodyCount((const NewtonWorld*) m_world);
}

-(int) WorldGetConstraintCount
{
	return (int) NewtonWorldGetConstraintCount((const NewtonWorld*) m_world);
}

-(struct iNewtonBody*) IslandGetBody: (const void*) island: (int) bodyIndex
{
	return (struct iNewtonBody*) NewtonIslandGetBody((const void*) island, (int) bodyIndex);
}

-(void) IslandGetBodyAABB: (const void*) island: (int) bodyIndex: (const float*) p0: (const float*) p1
{
	NewtonIslandGetBodyAABB ((const void*) island, (int) bodyIndex, (const float*) p0, (const float*) p1);
}

-(int) MaterialCreateGroupID
{
	return (int) NewtonMaterialCreateGroupID((const NewtonWorld*) m_world);
}

-(int) MaterialGetDefaultGroupID
{
	return (int) NewtonMaterialGetDefaultGroupID((const NewtonWorld*) m_world);
}

-(void) MaterialDestroyAllGroupID
{
	NewtonMaterialDestroyAllGroupID ((const NewtonWorld*) m_world);
}

-(void*) MaterialGetUserData: (int) id0: (int) id1
{
	return (void*) NewtonMaterialGetUserData((const NewtonWorld*) m_world, (int) id0, (int) id1);
}

-(void) MaterialSetSurfaceThickness: (int) id0: (int) id1: (float) thickness
{
	NewtonMaterialSetSurfaceThickness ((const NewtonWorld*) m_world, (int) id0, (int) id1, (float) thickness);
}

-(void) MaterialSetContinuousCollisionMode: (int) id0: (int) id1: (int) state
{
	NewtonMaterialSetContinuousCollisionMode ((const NewtonWorld*) m_world, (int) id0, (int) id1, (int) state);
}

-(void) MaterialSetCollisionCallback: (int) id0: (int) id1: (void*) userData: (iNewtonOnAABBOverlap) aabbOverlap: (iNewtonContactsProcess) process
{
	NewtonMaterialSetCollisionCallback ((const NewtonWorld*) m_world, (int) id0, (int) id1, (void*) userData, ( NewtonOnAABBOverlap) aabbOverlap, ( NewtonContactsProcess) process);
}

-(void) MaterialSetDefaultSoftness: (int) id0: (int) id1: (float) value
{
	NewtonMaterialSetDefaultSoftness ((const NewtonWorld*) m_world, (int) id0, (int) id1, (float) value);
}

-(void) MaterialSetDefaultElasticity: (int) id0: (int) id1: (float) elasticCoef
{
	NewtonMaterialSetDefaultElasticity ((const NewtonWorld*) m_world, (int) id0, (int) id1, (float) elasticCoef);
}

-(void) MaterialSetDefaultCollidable: (int) id0: (int) id1: (int) state
{
	NewtonMaterialSetDefaultCollidable ((const NewtonWorld*) m_world, (int) id0, (int) id1, (int) state);
}

-(void) MaterialSetDefaultFriction: (int) id0: (int) id1: (float) staticFriction: (float) kineticFriction
{
	NewtonMaterialSetDefaultFriction ((const NewtonWorld*) m_world, (int) id0, (int) id1, (float) staticFriction, (float) kineticFriction);
}

-(struct iNewtonMaterial*) WorldGetFirstMaterial
{
	return (struct iNewtonMaterial*) NewtonWorldGetFirstMaterial((const NewtonWorld*) m_world);
}

-(struct iNewtonMaterial*) WorldGetNextMaterial: (const struct iNewtonMaterial*) material
{
	return (struct iNewtonMaterial*) NewtonWorldGetNextMaterial((const NewtonWorld*) m_world, (const struct  NewtonMaterial*) material);
}

-(struct iNewtonBody*) WorldGetFirstBody
{
	return (struct iNewtonBody*) NewtonWorldGetFirstBody((const NewtonWorld*) m_world);
}

-(struct iNewtonBody*) WorldGetNextBody: (const struct iNewtonBody*) curBody
{
	return (struct iNewtonBody*) NewtonWorldGetNextBody((const NewtonWorld*) m_world, (const struct  NewtonBody*) curBody);
}

-(void*) MaterialGetMaterialPairUserData: (const struct iNewtonMaterial*) material
{
	return (void*) NewtonMaterialGetMaterialPairUserData((const struct  NewtonMaterial*) material);
}

-(unsigned) MaterialGetContactFaceAttribute: (const struct iNewtonMaterial*) material
{
	return (unsigned) NewtonMaterialGetContactFaceAttribute((const struct  NewtonMaterial*) material);
}

-(unsigned) MaterialGetBodyCollisionID: (const struct iNewtonMaterial*) material: (const struct iNewtonBody*) body
{
	return (unsigned) NewtonMaterialGetBodyCollisionID((const struct  NewtonMaterial*) material, (const struct  NewtonBody*) body);
}

-(float) MaterialGetContactNormalSpeed: (const struct iNewtonMaterial*) material
{
	return (float) NewtonMaterialGetContactNormalSpeed((const struct  NewtonMaterial*) material);
}

-(void) MaterialGetContactForce: (const struct iNewtonMaterial*) material: (float*) force
{
	NewtonMaterialGetContactForce ((const struct  NewtonMaterial*) material, (float*) force);
}

-(void) MaterialGetContactPositionAndNormal: (const struct iNewtonMaterial*) material: (float*) posit: (float*) normal
{
	NewtonMaterialGetContactPositionAndNormal ((const struct  NewtonMaterial*) material, (float*) posit, (float*) normal);
}

-(void) MaterialGetContactTangentDirections: (const struct iNewtonMaterial*) material: (float*) dir0: (float*) dir1
{
	NewtonMaterialGetContactTangentDirections ((const struct  NewtonMaterial*) material, (float*) dir0, (float*) dir1);
}

-(float) MaterialGetContactTangentSpeed: (const struct iNewtonMaterial*) material: (int) index
{
	return (float) NewtonMaterialGetContactTangentSpeed((const struct  NewtonMaterial*) material, (int) index);
}

-(void) MaterialSetContactSoftness: (const struct iNewtonMaterial*) material: (float) softness
{
	NewtonMaterialSetContactSoftness ((const struct  NewtonMaterial*) material, (float) softness);
}

-(void) MaterialSetContactElasticity: (const struct iNewtonMaterial*) material: (float) restitution
{
	NewtonMaterialSetContactElasticity ((const struct  NewtonMaterial*) material, (float) restitution);
}

-(void) MaterialSetContactFrictionState: (const struct iNewtonMaterial*) material: (int) state: (int) index
{
	NewtonMaterialSetContactFrictionState ((const struct  NewtonMaterial*) material, (int) state, (int) index);
}

-(void) MaterialSetContactFrictionCoef: (const struct iNewtonMaterial*) material: (float) staticFrictionCoef: (float) kineticFrictionCoef: (int) index
{
	NewtonMaterialSetContactFrictionCoef ((const struct  NewtonMaterial*) material, (float) staticFrictionCoef, (float) kineticFrictionCoef, (int) index);
}

-(void) MaterialSetContactNormalAcceleration: (const struct iNewtonMaterial*) material: (float) accel
{
	NewtonMaterialSetContactNormalAcceleration ((const struct  NewtonMaterial*) material, (float) accel);
}

-(void) MaterialSetContactNormalDirection: (const struct iNewtonMaterial*) material: (const float*) directionVector
{
	NewtonMaterialSetContactNormalDirection ((const struct  NewtonMaterial*) material, (const float*) directionVector);
}

-(void) MaterialSetContactTangentAcceleration: (const struct iNewtonMaterial*) material: (float) accel: (int) index
{
	NewtonMaterialSetContactTangentAcceleration ((const struct  NewtonMaterial*) material, (float) accel, (int) index);
}

-(void) MaterialContactRotateTangentDirections: (const struct iNewtonMaterial*) material: (const float*) directionVector
{
	NewtonMaterialContactRotateTangentDirections ((const struct  NewtonMaterial*) material, (const float*) directionVector);
}

-(struct iNewtonCollision*) CreateNull
{
	return (struct iNewtonCollision*) NewtonCreateNull((const NewtonWorld*) m_world);
}

-(struct iNewtonCollision*) CreateSphere: (float) radiusX: (float) radiusY: (float) radiusZ: (int) shapeID: (const float*) offsetMatrix
{
	return (struct iNewtonCollision*) NewtonCreateSphere((const NewtonWorld*) m_world, (float) radiusX, (float) radiusY, (float) radiusZ, (int) shapeID, (const float*) offsetMatrix);
}

-(struct iNewtonCollision*) CreateBox: (float) dx: (float) dy: (float) dz: (int) shapeID: (const float*) offsetMatrix
{
	return (struct iNewtonCollision*) NewtonCreateBox((const NewtonWorld*) m_world, (float) dx, (float) dy, (float) dz, (int) shapeID, (const float*) offsetMatrix);
}

-(struct iNewtonCollision*) CreateCone: (float) radius: (float) height: (int) shapeID: (const float*) offsetMatrix
{
	return (struct iNewtonCollision*) NewtonCreateCone((const NewtonWorld*) m_world, (float) radius, (float) height, (int) shapeID, (const float*) offsetMatrix);
}

-(struct iNewtonCollision*) CreateCapsule: (float) radius: (float) height: (int) shapeID: (const float*) offsetMatrix
{
	return (struct iNewtonCollision*) NewtonCreateCapsule((const NewtonWorld*) m_world, (float) radius, (float) height, (int) shapeID, (const float*) offsetMatrix);
}

-(struct iNewtonCollision*) CreateCylinder: (float) radius: (float) height: (int) shapeID: (const float*) offsetMatrix
{
	return (struct iNewtonCollision*) NewtonCreateCylinder((const NewtonWorld*) m_world, (float) radius, (float) height, (int) shapeID, (const float*) offsetMatrix);
}

-(struct iNewtonCollision*) CreateChamferCylinder: (float) radius: (float) height: (int) shapeID: (const float*) offsetMatrix
{
	return (struct iNewtonCollision*) NewtonCreateChamferCylinder((const NewtonWorld*) m_world, (float) radius, (float) height, (int) shapeID, (const float*) offsetMatrix);
}

-(struct iNewtonCollision*) CreateConvexHull: (int) count: (const float*) vertexCloud: (int) strideInBytes: (float) tolerance: (int) shapeID: (const float*) offsetMatrix
{
	return (struct iNewtonCollision*) NewtonCreateConvexHull((const NewtonWorld*) m_world, (int) count, (const float*) vertexCloud, (int) strideInBytes, (float) tolerance, (int) shapeID, (const float*) offsetMatrix);
}

-(struct iNewtonCollision*) CreateConvexHullFromMesh: (const struct iNewtonMesh*) mesh: (float) tolerance: (int) shapeID
{
	return (struct iNewtonCollision*) NewtonCreateConvexHullFromMesh((const NewtonWorld*) m_world, (const struct  NewtonMesh*) mesh, (float) tolerance, (int) shapeID);
}

-(struct iNewtonCollision*) CreateConvexHullModifier: (const struct iNewtonCollision*) convexHullCollision: (int) shapeID
{
	return (struct iNewtonCollision*) NewtonCreateConvexHullModifier((const NewtonWorld*) m_world, (const struct  NewtonCollision*) convexHullCollision, (int) shapeID);
}

-(void) ConvexHullModifierGetMatrix: (const struct iNewtonCollision*) convexHullCollision: (float*) matrix
{
	NewtonConvexHullModifierGetMatrix ((const struct  NewtonCollision*) convexHullCollision, (float*) matrix);
}

-(void) ConvexHullModifierSetMatrix: (const struct iNewtonCollision*) convexHullCollision: (const float*) matrix
{
	NewtonConvexHullModifierSetMatrix ((const struct  NewtonCollision*) convexHullCollision, (const float*) matrix);
}

-(int) CollisionIsTriggerVolume: (const struct iNewtonCollision*) convexCollision
{
	return (int) NewtonCollisionIsTriggerVolume((const struct  NewtonCollision*) convexCollision);
}

-(void) CollisionSetAsTriggerVolume: (const struct iNewtonCollision*) convexCollision: (int) trigger
{
	NewtonCollisionSetAsTriggerVolume ((const struct  NewtonCollision*) convexCollision, (int) trigger);
}

-(void) CollisionSetMaxBreakImpactImpulse: (const struct iNewtonCollision*) convexHullCollision: (float) maxImpactImpulse
{
	NewtonCollisionSetMaxBreakImpactImpulse ((const struct  NewtonCollision*) convexHullCollision, (float) maxImpactImpulse);
}

-(float) CollisionGetMaxBreakImpactImpulse: (const struct iNewtonCollision*) convexHullCollision
{
	return (float) NewtonCollisionGetMaxBreakImpactImpulse((const struct  NewtonCollision*) convexHullCollision);
}

-(void) CollisionSetUserID: (const struct iNewtonCollision*) convexCollision: (unsigned) id
{
	NewtonCollisionSetUserID ((const struct  NewtonCollision*) convexCollision, (unsigned) id);
}

-(unsigned) CollisionGetUserID: (const struct iNewtonCollision*) convexCollision
{
	return (unsigned) NewtonCollisionGetUserID((const struct  NewtonCollision*) convexCollision);
}

-(int) ConvexHullGetFaceIndices: (const struct iNewtonCollision*) convexHullCollision: (int) face: (int*) faceIndices
{
	return (int) NewtonConvexHullGetFaceIndices((const struct  NewtonCollision*) convexHullCollision, (int) face, (int*) faceIndices);
}

-(float) ConvexCollisionCalculateVolume: (const struct iNewtonCollision*) convexCollision
{
	return (float) NewtonConvexCollisionCalculateVolume((const struct  NewtonCollision*) convexCollision);
}

-(void) ConvexCollisionCalculateInertialMatrix: (const struct iNewtonCollision*) convexCollision: (float*) inertia: (float*) origin
{
	NewtonConvexCollisionCalculateInertialMatrix ((const struct  NewtonCollision*) convexCollision, (float*) inertia, (float*) origin);
}

-(void) CollisionMakeUnique: (const struct iNewtonCollision*) collision
{
	NewtonCollisionMakeUnique ((const NewtonWorld*) m_world, (const struct  NewtonCollision*) collision);
}

-(void) ReleaseCollision: (const struct iNewtonCollision*) collision
{
	NewtonReleaseCollision ((const NewtonWorld*) m_world, (const struct  NewtonCollision*) collision);
}

-(int) AddCollisionReference: (const struct iNewtonCollision*) collision
{
	return (int) NewtonAddCollisionReference((const struct  NewtonCollision*) collision);
}

-(struct iNewtonCollision*) CreateCompoundCollision: (int) count: (struct iNewtonCollision* const*) collisionPrimitiveArray: (int) shapeID
{
	return (struct iNewtonCollision*) NewtonCreateCompoundCollision((const NewtonWorld*) m_world, (int) count, (struct  NewtonCollision* const*) collisionPrimitiveArray, (int) shapeID);
}

-(struct iNewtonCollision*) CreateCompoundCollisionFromMesh: (const struct iNewtonMesh*) mesh: (int) maxSubShapesCount: (int) shapeID: (int) subShapeID
{
	return (struct iNewtonCollision*) NewtonCreateCompoundCollisionFromMesh((const NewtonWorld*) m_world, (const struct  NewtonMesh*) mesh, (int) maxSubShapesCount, (int) shapeID, (int) subShapeID);
}

-(struct iNewtonCollision*) CreateCompoundBreakable: (int) meshCount: (const struct iNewtonMesh* const*) solids: (const int* const) shapeIDArray: (const float* const) densities: (const int* const) internalFaceMaterial: (int) shapeID: (int) debriID: (float) debriSeparationGap
{
	return (struct iNewtonCollision*) NewtonCreateCompoundBreakable((const NewtonWorld*) m_world, (int) meshCount, (const struct  NewtonMesh* const*) solids, (const int* const) shapeIDArray, (const float* const) densities, (const int* const) internalFaceMaterial, (int) shapeID, (int) debriID, (float) debriSeparationGap);
}

-(void) CompoundBreakableResetAnchoredPieces: (const struct iNewtonCollision*) compoundBreakable
{
	NewtonCompoundBreakableResetAnchoredPieces ((const struct  NewtonCollision*) compoundBreakable);
}

-(void) CompoundBreakableSetAnchoredPieces: (const struct iNewtonCollision*) compoundBreakable: (int) fixShapesCount: (float* const) matrixPallete: (struct iNewtonCollision**) fixedShapesArray
{
	NewtonCompoundBreakableSetAnchoredPieces ((const struct  NewtonCollision*) compoundBreakable, (int) fixShapesCount, (float* const) matrixPallete, (struct  NewtonCollision**) fixedShapesArray);
}

-(int) CompoundBreakableGetVertexCount: (const struct iNewtonCollision*) compoundBreakable
{
	return (int) NewtonCompoundBreakableGetVertexCount((const struct  NewtonCollision*) compoundBreakable);
}

-(void) CompoundBreakableGetVertexStreams: (const struct iNewtonCollision*) compoundBreakable: (int) vertexStrideInByte: (float*) vertex: (int) normalStrideInByte: (float*) normal: (int) uvStrideInByte: (float*) uv
{
	NewtonCompoundBreakableGetVertexStreams ((const struct  NewtonCollision*) compoundBreakable, (int) vertexStrideInByte, (float*) vertex, (int) normalStrideInByte, (float*) normal, (int) uvStrideInByte, (float*) uv);
}

-(struct iNewtonbreakableComponentMesh*) BreakableGetMainMesh: (const struct iNewtonCollision*) compoundBreakable
{
	return (struct iNewtonbreakableComponentMesh*) NewtonBreakableGetMainMesh((const struct  NewtonCollision*) compoundBreakable);
}

-(struct iNewtonbreakableComponentMesh*) BreakableGetFirstComponent: (const struct iNewtonCollision*) compoundBreakable
{
	return (struct iNewtonbreakableComponentMesh*) NewtonBreakableGetFirstComponent((const struct  NewtonCollision*) compoundBreakable);
}

-(struct iNewtonbreakableComponentMesh*) BreakableGetNextComponent: (const struct iNewtonbreakableComponentMesh*) component
{
	return (struct iNewtonbreakableComponentMesh*) NewtonBreakableGetNextComponent((const struct  NewtonbreakableComponentMesh*) component);
}

-(void) BreakableBeginDelete: (const struct iNewtonCollision*) compoundBreakable
{
	NewtonBreakableBeginDelete ((const struct  NewtonCollision*) compoundBreakable);
}

-(struct iNewtonBody*) BreakableCreateDebrieBody: (const struct iNewtonCollision*) compoundBreakable: (const struct iNewtonbreakableComponentMesh*) component
{
	return (struct iNewtonBody*) NewtonBreakableCreateDebrieBody((const struct  NewtonCollision*) compoundBreakable, (const struct  NewtonbreakableComponentMesh*) component);
}

-(void) BreakableDeleteComponent: (const struct iNewtonCollision*) compoundBreakable: (const struct iNewtonbreakableComponentMesh*) component
{
	NewtonBreakableDeleteComponent ((const struct  NewtonCollision*) compoundBreakable, (const struct  NewtonbreakableComponentMesh*) component);
}

-(void) BreakableEndDelete: (const struct iNewtonCollision*) compoundBreakable
{
	NewtonBreakableEndDelete ((const struct  NewtonCollision*) compoundBreakable);
}

-(int) BreakableGetComponentsInRadius: (const struct iNewtonCollision*) compoundBreakable: (const float*) position: (float) radius: (struct iNewtonbreakableComponentMesh**) segments: (int) maxCount
{
	return (int) NewtonBreakableGetComponentsInRadius((const struct  NewtonCollision*) compoundBreakable, (const float*) position, (float) radius, (struct  NewtonbreakableComponentMesh**) segments, (int) maxCount);
}

-(void*) BreakableGetFirstSegment: (const struct iNewtonbreakableComponentMesh*) breakableComponent
{
	return (void*) NewtonBreakableGetFirstSegment((const struct  NewtonbreakableComponentMesh*) breakableComponent);
}

-(void*) BreakableGetNextSegment: (const void*) segment
{
	return (void*) NewtonBreakableGetNextSegment((const void*) segment);
}

-(int) BreakableSegmentGetMaterial: (const void*) segment
{
	return (int) NewtonBreakableSegmentGetMaterial((const void*) segment);
}

-(int) BreakableSegmentGetIndexCount: (const void*) segment
{
	return (int) NewtonBreakableSegmentGetIndexCount((const void*) segment);
}

-(int) BreakableSegmentGetIndexStream: (const struct iNewtonCollision*) compoundBreakable: (const struct iNewtonbreakableComponentMesh*) meshOwner: (const void*) segment: (int*) index
{
	return (int) NewtonBreakableSegmentGetIndexStream((const struct  NewtonCollision*) compoundBreakable, (const struct  NewtonbreakableComponentMesh*) meshOwner, (const void*) segment, (int*) index);
}

-(int) BreakableSegmentGetIndexStreamShort: (const struct iNewtonCollision*) compoundBreakable: (const struct iNewtonbreakableComponentMesh*) meshOwner: (const void*) segment: (short int*) index
{
	return (int) NewtonBreakableSegmentGetIndexStreamShort((const struct  NewtonCollision*) compoundBreakable, (const struct  NewtonbreakableComponentMesh*) meshOwner, (const void*) segment, (short int*) index);
}

-(struct iNewtonCollision*) CreateUserMeshCollision: (const float*) minBox: (const float*) maxBox: (void*) userData: (iNewtonUserMeshCollisionCollideCallback) collideCallback: (iNewtonUserMeshCollisionRayHitCallback) rayHitCallback: (iNewtonUserMeshCollisionDestroyCallback) destroyCallback: (iNewtonUserMeshCollisionGetCollisionInfo) getInfoCallback: (iNewtonUserMeshCollisionGetFacesInAABB) facesInAABBCallback: (int) shapeID
{
	return (struct iNewtonCollision*) NewtonCreateUserMeshCollision((const NewtonWorld*) m_world, (const float*) minBox, (const float*) maxBox, (void*) userData, ( NewtonUserMeshCollisionCollideCallback) collideCallback, ( NewtonUserMeshCollisionRayHitCallback) rayHitCallback, ( NewtonUserMeshCollisionDestroyCallback) destroyCallback, ( NewtonUserMeshCollisionGetCollisionInfo) getInfoCallback, ( NewtonUserMeshCollisionGetFacesInAABB) facesInAABBCallback, (int) shapeID);
}

-(struct iNewtonCollision*) CreateSceneCollision: (int) shapeID
{
	return (struct iNewtonCollision*) NewtonCreateSceneCollision((const NewtonWorld*) m_world, (int) shapeID);
}

-(struct iNewtonSceneProxy*) SceneCollisionCreateProxy: (struct iNewtonCollision*) scene: (struct iNewtonCollision*) collision
{
	return (struct iNewtonSceneProxy*) NewtonSceneCollisionCreateProxy((struct  NewtonCollision*) scene, (struct  NewtonCollision*) collision);
}

-(void) SceneCollisionDestroyProxy: (struct iNewtonCollision*) scene: (struct iNewtonSceneProxy*) Proxy
{
	NewtonSceneCollisionDestroyProxy ((struct  NewtonCollision*) scene, (struct  NewtonSceneProxy*) Proxy);
}

-(void) SceneProxySetMatrix: (struct iNewtonSceneProxy*) proxy: (const float*) matrix
{
	NewtonSceneProxySetMatrix ((struct  NewtonSceneProxy*) proxy, (const float*) matrix);
}

-(void) SceneProxyGetMatrix: (struct iNewtonSceneProxy*) proxy: (float*) matrix
{
	NewtonSceneProxyGetMatrix ((struct  NewtonSceneProxy*) proxy, (float*) matrix);
}

-(void) SceneCollisionOptimize: (struct iNewtonCollision*) scene
{
	NewtonSceneCollisionOptimize ((struct  NewtonCollision*) scene);
}

-(struct iNewtonCollision*) CreateCollisionFromSerialization: (iNewtonDeserialize) deserializeFunction: (void*) serializeHandle
{
	return (struct iNewtonCollision*) NewtonCreateCollisionFromSerialization((const NewtonWorld*) m_world, ( NewtonDeserialize) deserializeFunction, (void*) serializeHandle);
}

-(void) CollisionSerialize: (const struct iNewtonCollision*) collision: (iNewtonSerialize) serializeFunction: (void*) serializeHandle
{
	NewtonCollisionSerialize ((const NewtonWorld*) m_world, (const struct  NewtonCollision*) collision, ( NewtonSerialize) serializeFunction, (void*) serializeHandle);
}

-(void) CollisionGetInfo: (const struct iNewtonCollision*) collision: (struct iNewtonCollisionInfoRecord*) collisionInfo
{
	NewtonCollisionGetInfo ((const struct  NewtonCollision*) collision, (struct  NewtonCollisionInfoRecord*) collisionInfo);
}

-(struct iNewtonCollision*) CreateHeightFieldCollision: (int) width: (int) height: (int) gridsDiagonals: (unsigned short*) elevationMap: (char*) attributeMap: (float) horizontalScale: (float) verticalScale: (int) shapeID
{
	return (struct iNewtonCollision*) NewtonCreateHeightFieldCollision((const NewtonWorld*) m_world, (int) width, (int) height, (int) gridsDiagonals, (unsigned short*) elevationMap, (char*) attributeMap, (float) horizontalScale, (float) verticalScale, (int) shapeID);
}

-(void) HeightFieldSetUserRayCastCallback: (const struct iNewtonCollision*) treeCollision: (iNewtonHeightFieldRayCastCallback) rayHitCallback
{
	NewtonHeightFieldSetUserRayCastCallback ((const struct  NewtonCollision*) treeCollision, ( NewtonHeightFieldRayCastCallback) rayHitCallback);
}

-(struct iNewtonCollision*) CreateTreeCollision: (int) shapeID
{
	return (struct iNewtonCollision*) NewtonCreateTreeCollision((const NewtonWorld*) m_world, (int) shapeID);
}

-(void) TreeCollisionSetUserRayCastCallback: (const struct iNewtonCollision*) treeCollision: (iNewtonCollisionTreeRayCastCallback) rayHitCallback
{
	NewtonTreeCollisionSetUserRayCastCallback ((const struct  NewtonCollision*) treeCollision, ( NewtonCollisionTreeRayCastCallback) rayHitCallback);
}

-(void) TreeCollisionBeginBuild: (const struct iNewtonCollision*) treeCollision
{
	NewtonTreeCollisionBeginBuild ((const struct  NewtonCollision*) treeCollision);
}

-(void) TreeCollisionAddFace: (const struct iNewtonCollision*) treeCollision: (int) vertexCount: (const float*) vertexPtr: (int) strideInBytes: (int) faceAttribute
{
	NewtonTreeCollisionAddFace ((const struct  NewtonCollision*) treeCollision, (int) vertexCount, (const float*) vertexPtr, (int) strideInBytes, (int) faceAttribute);
}

-(void) TreeCollisionEndBuild: (const struct iNewtonCollision*) treeCollision: (int) optimize
{
	NewtonTreeCollisionEndBuild ((const struct  NewtonCollision*) treeCollision, (int) optimize);
}

-(int) TreeCollisionGetFaceAtribute: (const struct iNewtonCollision*) treeCollision: (const int*) faceIndexArray
{
	return (int) NewtonTreeCollisionGetFaceAtribute((const struct  NewtonCollision*) treeCollision, (const int*) faceIndexArray);
}

-(void) TreeCollisionSetFaceAtribute: (const struct iNewtonCollision*) treeCollision: (const int*) faceIndexArray: (int) attribute
{
	NewtonTreeCollisionSetFaceAtribute ((const struct  NewtonCollision*) treeCollision, (const int*) faceIndexArray, (int) attribute);
}

-(int) TreeCollisionGetVertexListIndexListInAABB: (const struct iNewtonCollision*) treeCollision: (const float*) p0: (const float*) p1: (const float**) vertexArray: (int*) vertexCount: (int*) vertexStrideInBytes: (const int*) indexList: (int) maxIndexCount: (const int*) faceAttribute
{
	return (int) NewtonTreeCollisionGetVertexListIndexListInAABB((const struct  NewtonCollision*) treeCollision, (const float*) p0, (const float*) p1, (const float**) vertexArray, (int*) vertexCount, (int*) vertexStrideInBytes, (const int*) indexList, (int) maxIndexCount, (const int*) faceAttribute);
}

-(void) StaticCollisionSetDebugCallback: (const struct iNewtonCollision*) staticCollision: (iNewtonTreeCollisionCallback) userCallback
{
	NewtonStaticCollisionSetDebugCallback ((const struct  NewtonCollision*) staticCollision, ( NewtonTreeCollisionCallback) userCallback);
}

-(int) CollisionPointDistance: (const float*) point: (const struct iNewtonCollision*) collision: (const float*) matrix: (float*) contact: (float*) normal: (int) threadIndex
{
	return (int) NewtonCollisionPointDistance((const NewtonWorld*) m_world, (const float*) point, (const struct  NewtonCollision*) collision, (const float*) matrix, (float*) contact, (float*) normal, (int) threadIndex);
}

-(int) CollisionClosestPoint: (const struct iNewtonCollision*) collisionA: (const float*) matrixA: (const struct iNewtonCollision*) collisionB: (const float*) matrixB: (float*) contactA: (float*) contactB: (float*) normalAB: (int) threadIndex
{
	return (int) NewtonCollisionClosestPoint((const NewtonWorld*) m_world, (const struct  NewtonCollision*) collisionA, (const float*) matrixA, (const struct  NewtonCollision*) collisionB, (const float*) matrixB, (float*) contactA, (float*) contactB, (float*) normalAB, (int) threadIndex);
}

-(int) CollisionCollide: (int) maxSize: (const struct iNewtonCollision*) collisionA: (const float*) matrixA: (const struct iNewtonCollision*) collisionB: (const float*) matrixB: (float*) contacts: (float*) normals: (float*) penetration: (int) threadIndex
{
	return (int) NewtonCollisionCollide((const NewtonWorld*) m_world, (int) maxSize, (const struct  NewtonCollision*) collisionA, (const float*) matrixA, (const struct  NewtonCollision*) collisionB, (const float*) matrixB, (float*) contacts, (float*) normals, (float*) penetration, (int) threadIndex);
}

-(int) CollisionCollideContinue: (int) maxSize: (const float) timestep: (const struct iNewtonCollision*) collisionA: (const float*) matrixA: (const float*) velocA: (const float*) omegaA: (const struct iNewtonCollision*) collisionB: (const float*) matrixB: (const float*) velocB: (const float*) omegaB: (float*) timeOfImpact: (float*) contacts: (float*) normals: (float*) penetration: (int) threadIndex
{
	return (int) NewtonCollisionCollideContinue((const NewtonWorld*) m_world, (int) maxSize, (const float) timestep, (const struct  NewtonCollision*) collisionA, (const float*) matrixA, (const float*) velocA, (const float*) omegaA, (const struct  NewtonCollision*) collisionB, (const float*) matrixB, (const float*) velocB, (const float*) omegaB, (float*) timeOfImpact, (float*) contacts, (float*) normals, (float*) penetration, (int) threadIndex);
}

-(void) CollisionSupportVertex: (const struct iNewtonCollision*) collision: (const float*) dir: (float*) vertex
{
	NewtonCollisionSupportVertex ((const struct  NewtonCollision*) collision, (const float*) dir, (float*) vertex);
}

-(float) CollisionRayCast: (const struct iNewtonCollision*) collision: (const float*) p0: (const float*) p1: (float*) normals: (int*) attribute
{
	return (float) NewtonCollisionRayCast((const struct  NewtonCollision*) collision, (const float*) p0, (const float*) p1, (float*) normals, (int*) attribute);
}

-(void) CollisionCalculateAABB: (const struct iNewtonCollision*) collision: (const float*) matrix: (float*) p0: (float*) p1
{
	NewtonCollisionCalculateAABB ((const struct  NewtonCollision*) collision, (const float*) matrix, (float*) p0, (float*) p1);
}

-(void) CollisionForEachPolygonDo: (const struct iNewtonCollision*) collision: (const float*) matrix: (iNewtonCollisionIterator) callback: (void*) userData
{
	NewtonCollisionForEachPolygonDo ((const struct  NewtonCollision*) collision, (const float*) matrix, ( NewtonCollisionIterator) callback, (void*) userData);
}

-(void) GetEulerAngle: (const float*) matrix: (float*) eulersAngles
{
	NewtonGetEulerAngle ((const float*) matrix, (float*) eulersAngles);
}

-(void) SetEulerAngle: (const float*) eulersAngles: (float*) matrix
{
	NewtonSetEulerAngle ((const float*) eulersAngles, (float*) matrix);
}

-(float) CalculateSpringDamperAcceleration: (float) dt: (float) ks: (float) x: (float) kd: (float) s
{
	return (float) NewtonCalculateSpringDamperAcceleration((float) dt, (float) ks, (float) x, (float) kd, (float) s);
}

-(struct iNewtonBody*) CreateBody: (const struct iNewtonCollision*) collision: (const float*) matrix
{
	return (struct iNewtonBody*) NewtonCreateBody((const NewtonWorld*) m_world, (const struct  NewtonCollision*) collision, (const float*) matrix);
}

-(void) DestroyBody: (const struct iNewtonBody*) body
{
	NewtonDestroyBody ((const NewtonWorld*) m_world, (const struct  NewtonBody*) body);
}

-(void) BodyAddForce: (const struct iNewtonBody*) body: (const float*) force
{
	NewtonBodyAddForce ((const struct  NewtonBody*) body, (const float*) force);
}

-(void) BodyAddTorque: (const struct iNewtonBody*) body: (const float*) torque
{
	NewtonBodyAddTorque ((const struct  NewtonBody*) body, (const float*) torque);
}

-(void) BodyCalculateInverseDynamicsForce: (const struct iNewtonBody*) body: (float) timestep: (const float*) desiredVeloc: (float*) forceOut
{
	NewtonBodyCalculateInverseDynamicsForce ((const struct  NewtonBody*) body, (float) timestep, (const float*) desiredVeloc, (float*) forceOut);
}

-(void) BodySetMatrix: (const struct iNewtonBody*) body: (const float*) matrix
{
	NewtonBodySetMatrix ((const struct  NewtonBody*) body, (const float*) matrix);
}

-(void) BodySetMatrixRecursive: (const struct iNewtonBody*) body: (const float*) matrix
{
	NewtonBodySetMatrixRecursive ((const struct  NewtonBody*) body, (const float*) matrix);
}

-(void) BodySetMassMatrix: (const struct iNewtonBody*) body: (float) mass: (float) Ixx: (float) Iyy: (float) Izz
{
	NewtonBodySetMassMatrix ((const struct  NewtonBody*) body, (float) mass, (float) Ixx, (float) Iyy, (float) Izz);
}

-(void) BodySetMaterialGroupID: (const struct iNewtonBody*) body: (int) id
{
	NewtonBodySetMaterialGroupID ((const struct  NewtonBody*) body, (int) id);
}

-(void) BodySetContinuousCollisionMode: (const struct iNewtonBody*) body: (unsigned) state
{
	NewtonBodySetContinuousCollisionMode ((const struct  NewtonBody*) body, (unsigned) state);
}

-(void) BodySetJointRecursiveCollision: (const struct iNewtonBody*) body: (unsigned) state
{
	NewtonBodySetJointRecursiveCollision ((const struct  NewtonBody*) body, (unsigned) state);
}

-(void) BodySetOmega: (const struct iNewtonBody*) body: (const float*) omega
{
	NewtonBodySetOmega ((const struct  NewtonBody*) body, (const float*) omega);
}

-(void) BodySetVelocity: (const struct iNewtonBody*) body: (const float*) velocity
{
	NewtonBodySetVelocity ((const struct  NewtonBody*) body, (const float*) velocity);
}

-(void) BodySetForce: (const struct iNewtonBody*) body: (const float*) force
{
	NewtonBodySetForce ((const struct  NewtonBody*) body, (const float*) force);
}

-(void) BodySetTorque: (const struct iNewtonBody*) body: (const float*) torque
{
	NewtonBodySetTorque ((const struct  NewtonBody*) body, (const float*) torque);
}

-(void) BodySetCentreOfMass: (const struct iNewtonBody*) body: (const float*) com
{
	NewtonBodySetCentreOfMass ((const struct  NewtonBody*) body, (const float*) com);
}

-(void) BodySetLinearDamping: (const struct iNewtonBody*) body: (float) linearDamp
{
	NewtonBodySetLinearDamping ((const struct  NewtonBody*) body, (float) linearDamp);
}

-(void) BodySetAngularDamping: (const struct iNewtonBody*) body: (const float*) angularDamp
{
	NewtonBodySetAngularDamping ((const struct  NewtonBody*) body, (const float*) angularDamp);
}

-(void) BodySetUserData: (const struct iNewtonBody*) body: (void*) userData
{
	NewtonBodySetUserData ((const struct  NewtonBody*) body, (void*) userData);
}

-(void) BodySetCollision: (const struct iNewtonBody*) body: (const struct iNewtonCollision*) collision
{
	NewtonBodySetCollision ((const struct  NewtonBody*) body, (const struct  NewtonCollision*) collision);
}

-(int) BodyGetSleepState: (const struct iNewtonBody*) body
{
	return (int) NewtonBodyGetSleepState((const struct  NewtonBody*) body);
}

-(int) BodyGetAutoSleep: (const struct iNewtonBody*) body
{
	return (int) NewtonBodyGetAutoSleep((const struct  NewtonBody*) body);
}

-(void) BodySetAutoSleep: (const struct iNewtonBody*) body: (int) state
{
	NewtonBodySetAutoSleep ((const struct  NewtonBody*) body, (int) state);
}

-(int) BodyGetFreezeState: (const struct iNewtonBody*) body
{
	return (int) NewtonBodyGetFreezeState((const struct  NewtonBody*) body);
}

-(void) BodySetFreezeState: (const struct iNewtonBody*) body: (int) state
{
	NewtonBodySetFreezeState ((const struct  NewtonBody*) body, (int) state);
}

-(void) BodySetDestructorCallback: (const struct iNewtonBody*) body: (iNewtonBodyDestructor) callback
{
	NewtonBodySetDestructorCallback ((const struct  NewtonBody*) body, ( NewtonBodyDestructor) callback);
}

-(void) BodySetTransformCallback: (const struct iNewtonBody*) body: (iNewtonSetTransform) callback
{
	NewtonBodySetTransformCallback ((const struct  NewtonBody*) body, ( NewtonSetTransform) callback);
}

-(iNewtonSetTransform) BodyGetTransformCallback: (const struct iNewtonBody*) body
{
	return (iNewtonSetTransform) NewtonBodyGetTransformCallback((const struct  NewtonBody*) body);
}

-(void) BodySetForceAndTorqueCallback: (const struct iNewtonBody*) body: (iNewtonApplyForceAndTorque) callback
{
	NewtonBodySetForceAndTorqueCallback ((const struct  NewtonBody*) body, ( NewtonApplyForceAndTorque) callback);
}

-(iNewtonApplyForceAndTorque) BodyGetForceAndTorqueCallback: (const struct iNewtonBody*) body
{
	return (iNewtonApplyForceAndTorque) NewtonBodyGetForceAndTorqueCallback((const struct  NewtonBody*) body);
}

-(void*) BodyGetUserData: (const struct iNewtonBody*) body
{
	return (void*) NewtonBodyGetUserData((const struct  NewtonBody*) body);
}

-(struct iNewtonWorld*) BodyGetWorld: (const struct iNewtonBody*) body
{
	return (struct iNewtonWorld*) NewtonBodyGetWorld((const struct  NewtonBody*) body);
}

-(struct iNewtonCollision*) BodyGetCollision: (const struct iNewtonBody*) body
{
	return (struct iNewtonCollision*) NewtonBodyGetCollision((const struct  NewtonBody*) body);
}

-(int) BodyGetMaterialGroupID: (const struct iNewtonBody*) body
{
	return (int) NewtonBodyGetMaterialGroupID((const struct  NewtonBody*) body);
}

-(int) BodyGetContinuousCollisionMode: (const struct iNewtonBody*) body
{
	return (int) NewtonBodyGetContinuousCollisionMode((const struct  NewtonBody*) body);
}

-(int) BodyGetJointRecursiveCollision: (const struct iNewtonBody*) body
{
	return (int) NewtonBodyGetJointRecursiveCollision((const struct  NewtonBody*) body);
}

-(void) BodyGetMatrix: (const struct iNewtonBody*) body: (float*) matrix
{
	NewtonBodyGetMatrix ((const struct  NewtonBody*) body, (float*) matrix);
}

-(void) BodyGetRotation: (const struct iNewtonBody*) body: (float*) rotation
{
	NewtonBodyGetRotation ((const struct  NewtonBody*) body, (float*) rotation);
}

-(void) BodyGetMassMatrix: (const struct iNewtonBody*) body: (float*) mass: (float*) Ixx: (float*) Iyy: (float*) Izz
{
	NewtonBodyGetMassMatrix ((const struct  NewtonBody*) body, (float*) mass, (float*) Ixx, (float*) Iyy, (float*) Izz);
}

-(void) BodyGetInvMass: (const struct iNewtonBody*) body: (float*) invMass: (float*) invIxx: (float*) invIyy: (float*) invIzz
{
	NewtonBodyGetInvMass ((const struct  NewtonBody*) body, (float*) invMass, (float*) invIxx, (float*) invIyy, (float*) invIzz);
}

-(void) BodyGetOmega: (const struct iNewtonBody*) body: (float*) vector
{
	NewtonBodyGetOmega ((const struct  NewtonBody*) body, (float*) vector);
}

-(void) BodyGetVelocity: (const struct iNewtonBody*) body: (float*) vector
{
	NewtonBodyGetVelocity ((const struct  NewtonBody*) body, (float*) vector);
}

-(void) BodyGetForce: (const struct iNewtonBody*) body: (float*) vector
{
	NewtonBodyGetForce ((const struct  NewtonBody*) body, (float*) vector);
}

-(void) BodyGetTorque: (const struct iNewtonBody*) body: (float*) vector
{
	NewtonBodyGetTorque ((const struct  NewtonBody*) body, (float*) vector);
}

-(void) BodyGetForceAcc: (const struct iNewtonBody*) body: (float*) vector
{
	NewtonBodyGetForceAcc ((const struct  NewtonBody*) body, (float*) vector);
}

-(void) BodyGetTorqueAcc: (const struct iNewtonBody*) body: (float*) vector
{
	NewtonBodyGetTorqueAcc ((const struct  NewtonBody*) body, (float*) vector);
}

-(void) BodyGetCentreOfMass: (const struct iNewtonBody*) body: (float*) com
{
	NewtonBodyGetCentreOfMass ((const struct  NewtonBody*) body, (float*) com);
}

-(float) BodyGetLinearDamping: (const struct iNewtonBody*) body
{
	return (float) NewtonBodyGetLinearDamping((const struct  NewtonBody*) body);
}

-(void) BodyGetAngularDamping: (const struct iNewtonBody*) body: (float*) vector
{
	NewtonBodyGetAngularDamping ((const struct  NewtonBody*) body, (float*) vector);
}

-(void) BodyGetAABB: (const struct iNewtonBody*) body: (float*) p0: (float*) p1
{
	NewtonBodyGetAABB ((const struct  NewtonBody*) body, (float*) p0, (float*) p1);
}

-(struct iNewtonJoint*) BodyGetFirstJoint: (const struct iNewtonBody*) body
{
	return (struct iNewtonJoint*) NewtonBodyGetFirstJoint((const struct  NewtonBody*) body);
}

-(struct iNewtonJoint*) BodyGetNextJoint: (const struct iNewtonBody*) body: (const struct iNewtonJoint*) joint
{
	return (struct iNewtonJoint*) NewtonBodyGetNextJoint((const struct  NewtonBody*) body, (const struct  NewtonJoint*) joint);
}

-(struct iNewtonJoint*) BodyGetFirstContactJoint: (const struct iNewtonBody*) body
{
	return (struct iNewtonJoint*) NewtonBodyGetFirstContactJoint((const struct  NewtonBody*) body);
}

-(struct iNewtonJoint*) BodyGetNextContactJoint: (const struct iNewtonBody*) body: (const struct iNewtonJoint*) contactJoint
{
	return (struct iNewtonJoint*) NewtonBodyGetNextContactJoint((const struct  NewtonBody*) body, (const struct  NewtonJoint*) contactJoint);
}

-(void*) ContactJointGetFirstContact: (const struct iNewtonJoint*) contactJoint
{
	return (void*) NewtonContactJointGetFirstContact((const struct  NewtonJoint*) contactJoint);
}

-(void*) ContactJointGetNextContact: (const struct iNewtonJoint*) contactJoint: (void*) contact
{
	return (void*) NewtonContactJointGetNextContact((const struct  NewtonJoint*) contactJoint, (void*) contact);
}

-(int) ContactJointGetContactCount: (const struct iNewtonJoint*) contactJoint
{
	return (int) NewtonContactJointGetContactCount((const struct  NewtonJoint*) contactJoint);
}

-(void) ContactJointRemoveContact: (const struct iNewtonJoint*) contactJoint: (void*) contact
{
	NewtonContactJointRemoveContact ((const struct  NewtonJoint*) contactJoint, (void*) contact);
}

-(struct iNewtonMaterial*) ContactGetMaterial: (const void*) contact
{
	return (struct iNewtonMaterial*) NewtonContactGetMaterial((const void*) contact);
}

-(void) BodyAddBuoyancyForce: (const struct iNewtonBody*) body: (float) fluidDensity: (float) fluidLinearViscosity: (float) fluidAngularViscosity: (const float*) gravityVector: (iNewtonGetBuoyancyPlane) buoyancyPlane: (void*) context
{
	NewtonBodyAddBuoyancyForce ((const struct  NewtonBody*) body, (float) fluidDensity, (float) fluidLinearViscosity, (float) fluidAngularViscosity, (const float*) gravityVector, ( NewtonGetBuoyancyPlane) buoyancyPlane, (void*) context);
}

-(void) BodyAddImpulse: (const struct iNewtonBody*) body: (const float*) pointDeltaVeloc: (const float*) pointPosit
{
	NewtonBodyAddImpulse ((const struct  NewtonBody*) body, (const float*) pointDeltaVeloc, (const float*) pointPosit);
}

-(void*) JointGetUserData: (const struct iNewtonJoint*) joint
{
	return (void*) NewtonJointGetUserData((const struct  NewtonJoint*) joint);
}

-(void) JointSetUserData: (const struct iNewtonJoint*) joint: (void*) userData
{
	NewtonJointSetUserData ((const struct  NewtonJoint*) joint, (void*) userData);
}

-(struct iNewtonBody*) JointGetBody0: (const struct iNewtonJoint*) joint
{
	return (struct iNewtonBody*) NewtonJointGetBody0((const struct  NewtonJoint*) joint);
}

-(struct iNewtonBody*) JointGetBody1: (const struct iNewtonJoint*) joint
{
	return (struct iNewtonBody*) NewtonJointGetBody1((const struct  NewtonJoint*) joint);
}

-(void) JointGetInfo: (const struct iNewtonJoint*) joint: (struct iNewtonJointRecord*) info
{
	NewtonJointGetInfo ((const struct  NewtonJoint*) joint, (struct  NewtonJointRecord*) info);
}

-(int) JointGetCollisionState: (const struct iNewtonJoint*) joint
{
	return (int) NewtonJointGetCollisionState((const struct  NewtonJoint*) joint);
}

-(void) JointSetCollisionState: (const struct iNewtonJoint*) joint: (int) state
{
	NewtonJointSetCollisionState ((const struct  NewtonJoint*) joint, (int) state);
}

-(float) JointGetStiffness: (const struct iNewtonJoint*) joint
{
	return (float) NewtonJointGetStiffness((const struct  NewtonJoint*) joint);
}

-(void) JointSetStiffness: (const struct iNewtonJoint*) joint: (float) state
{
	NewtonJointSetStiffness ((const struct  NewtonJoint*) joint, (float) state);
}

-(void) DestroyJoint: (const struct iNewtonJoint*) joint
{
	NewtonDestroyJoint ((const NewtonWorld*) m_world, (const struct  NewtonJoint*) joint);
}

-(void) JointSetDestructor: (const struct iNewtonJoint*) joint: (iNewtonConstraintDestructor) destructor
{
	NewtonJointSetDestructor ((const struct  NewtonJoint*) joint, ( NewtonConstraintDestructor) destructor);
}

-(struct iNewtonJoint*) ConstraintCreateUpVector: (const float*) pinDir: (const struct iNewtonBody*) body
{
	return (struct iNewtonJoint*) NewtonConstraintCreateUpVector((const NewtonWorld*) m_world, (const float*) pinDir, (const struct  NewtonBody*) body);
}

-(void) UpVectorGetPin: (const struct iNewtonJoint*) upVector: (float*) pin
{
	NewtonUpVectorGetPin ((const struct  NewtonJoint*) upVector, (float*) pin);
}

-(void) UpVectorSetPin: (const struct iNewtonJoint*) upVector: (const float*) pin
{
	NewtonUpVectorSetPin ((const struct  NewtonJoint*) upVector, (const float*) pin);
}

-(struct iNewtonJoint*) ConstraintCreateUserJoint: (int) maxDOF: (iNewtonUserBilateralCallBack) callback: (iNewtonUserBilateralGetInfoCallBack) getInfo: (const struct iNewtonBody*) childBody: (const struct iNewtonBody*) parentBody
{
	return (struct iNewtonJoint*) NewtonConstraintCreateUserJoint((const NewtonWorld*) m_world, (int) maxDOF, ( NewtonUserBilateralCallBack) callback, ( NewtonUserBilateralGetInfoCallBack) getInfo, (const struct  NewtonBody*) childBody, (const struct  NewtonBody*) parentBody);
}

-(void) UserJointSetFeedbackCollectorCallback: (const struct iNewtonJoint*) joint: (iNewtonUserBilateralCallBack) getFeedback
{
	NewtonUserJointSetFeedbackCollectorCallback ((const struct  NewtonJoint*) joint, ( NewtonUserBilateralCallBack) getFeedback);
}

-(void) UserJointAddLinearRow: (const struct iNewtonJoint*) joint: (const float*) pivot0: (const float*) pivot1: (const float*) dir
{
	NewtonUserJointAddLinearRow ((const struct  NewtonJoint*) joint, (const float*) pivot0, (const float*) pivot1, (const float*) dir);
}

-(void) UserJointAddAngularRow: (const struct iNewtonJoint*) joint: (float) relativeAngle: (const float*) dir
{
	NewtonUserJointAddAngularRow ((const struct  NewtonJoint*) joint, (float) relativeAngle, (const float*) dir);
}

-(void) UserJointAddGeneralRow: (const struct iNewtonJoint*) joint: (const float*) jacobian0: (const float*) jacobian1
{
	NewtonUserJointAddGeneralRow ((const struct  NewtonJoint*) joint, (const float*) jacobian0, (const float*) jacobian1);
}

-(void) UserJointSetRowMinimumFriction: (const struct iNewtonJoint*) joint: (float) friction
{
	NewtonUserJointSetRowMinimumFriction ((const struct  NewtonJoint*) joint, (float) friction);
}

-(void) UserJointSetRowMaximumFriction: (const struct iNewtonJoint*) joint: (float) friction
{
	NewtonUserJointSetRowMaximumFriction ((const struct  NewtonJoint*) joint, (float) friction);
}

-(void) UserJointSetRowAcceleration: (const struct iNewtonJoint*) joint: (float) acceleration
{
	NewtonUserJointSetRowAcceleration ((const struct  NewtonJoint*) joint, (float) acceleration);
}

-(void) UserJointSetRowSpringDamperAcceleration: (const struct iNewtonJoint*) joint: (float) springK: (float) springD
{
	NewtonUserJointSetRowSpringDamperAcceleration ((const struct  NewtonJoint*) joint, (float) springK, (float) springD);
}

-(void) UserJointSetRowStiffness: (const struct iNewtonJoint*) joint: (float) stiffness
{
	NewtonUserJointSetRowStiffness ((const struct  NewtonJoint*) joint, (float) stiffness);
}

-(float) UserJointGetRowForce: (const struct iNewtonJoint*) joint: (int) row
{
	return (float) NewtonUserJointGetRowForce((const struct  NewtonJoint*) joint, (int) row);
}

-(struct iNewtonMesh*) MeshCreate
{
	return (struct iNewtonMesh*) NewtonMeshCreate((const NewtonWorld*) m_world);
}

-(struct iNewtonMesh*) MeshCreateFromMesh: (const struct iNewtonMesh*) mesh
{
	return (struct iNewtonMesh*) NewtonMeshCreateFromMesh((const struct  NewtonMesh*) mesh);
}

-(struct iNewtonMesh*) MeshCreateFromCollision: (const struct iNewtonCollision*) collision
{
	return (struct iNewtonMesh*) NewtonMeshCreateFromCollision((const struct  NewtonCollision*) collision);
}

-(struct iNewtonMesh*) MeshConvexHull: (int) count: (const float*) vertexCloud: (int) strideInBytes: (float) tolerance
{
	return (struct iNewtonMesh*) NewtonMeshConvexHull((const NewtonWorld*) m_world, (int) count, (const float*) vertexCloud, (int) strideInBytes, (float) tolerance);
}

-(struct iNewtonMesh*) MeshCreatePlane: (const float*) locationMatrix: (float) witdth: (float) breadth: (int) material: (const float*) textureMatrix0: (const float*) textureMatrix1
{
	return (struct iNewtonMesh*) NewtonMeshCreatePlane((const NewtonWorld*) m_world, (const float*) locationMatrix, (float) witdth, (float) breadth, (int) material, (const float*) textureMatrix0, (const float*) textureMatrix1);
}

-(void) MeshDestroy: (const struct iNewtonMesh*) mesh
{
	NewtonMeshDestroy ((const struct  NewtonMesh*) mesh);
}

-(void) MeshCalculateOOBB: (const struct iNewtonMesh*) mesh: (float*) matrix: (float*) x: (float*) y: (float*) z
{
	NewtonMeshCalculateOOBB ((const struct  NewtonMesh*) mesh, (float*) matrix, (float*) x, (float*) y, (float*) z);
}

-(void) MeshCalculateVertexNormals: (const struct iNewtonMesh*) mesh: (float) angleInRadians
{
	NewtonMeshCalculateVertexNormals ((const struct  NewtonMesh*) mesh, (float) angleInRadians);
}

-(void) MeshApplySphericalMapping: (const struct iNewtonMesh*) mesh: (int) material
{
	NewtonMeshApplySphericalMapping ((const struct  NewtonMesh*) mesh, (int) material);
}

-(void) MeshApplyBoxMapping: (const struct iNewtonMesh*) mesh: (int) front: (int) side: (int) top
{
	NewtonMeshApplyBoxMapping ((const struct  NewtonMesh*) mesh, (int) front, (int) side, (int) top);
}

-(void) MeshApplyCylindricalMapping: (const struct iNewtonMesh*) mesh: (int) cylinderMaterial: (int) capMaterial
{
	NewtonMeshApplyCylindricalMapping ((const struct  NewtonMesh*) mesh, (int) cylinderMaterial, (int) capMaterial);
}

-(int) MeshIsOpenMesh: (const struct iNewtonMesh*) mesh
{
	return (int) NewtonMeshIsOpenMesh((const struct  NewtonMesh*) mesh);
}

-(void) MeshFixTJoints: (const struct iNewtonMesh*) mesh
{
	NewtonMeshFixTJoints ((const struct  NewtonMesh*) mesh);
}

-(void) MeshPolygonize: (const struct iNewtonMesh*) mesh
{
	NewtonMeshPolygonize ((const struct  NewtonMesh*) mesh);
}

-(void) MeshTriangulate: (const struct iNewtonMesh*) mesh
{
	NewtonMeshTriangulate ((const struct  NewtonMesh*) mesh);
}

-(struct iNewtonMesh*) MeshUnion: (const struct iNewtonMesh*) mesh: (const struct iNewtonMesh*) clipper: (const float*) clipperMatrix
{
	return (struct iNewtonMesh*) NewtonMeshUnion((const struct  NewtonMesh*) mesh, (const struct  NewtonMesh*) clipper, (const float*) clipperMatrix);
}

-(struct iNewtonMesh*) MeshDifference: (const struct iNewtonMesh*) mesh: (const struct iNewtonMesh*) clipper: (const float*) clipperMatrix
{
	return (struct iNewtonMesh*) NewtonMeshDifference((const struct  NewtonMesh*) mesh, (const struct  NewtonMesh*) clipper, (const float*) clipperMatrix);
}

-(struct iNewtonMesh*) MeshIntersection: (const struct iNewtonMesh*) mesh: (const struct iNewtonMesh*) clipper: (const float*) clipperMatrix
{
	return (struct iNewtonMesh*) NewtonMeshIntersection((const struct  NewtonMesh*) mesh, (const struct  NewtonMesh*) clipper, (const float*) clipperMatrix);
}

-(void) MeshClip: (const struct iNewtonMesh*) mesh: (const struct iNewtonMesh*) clipper: (const float*) clipperMatrix: (struct iNewtonMesh**) topMesh: (struct iNewtonMesh**) bottomMesh
{
	NewtonMeshClip ((const struct  NewtonMesh*) mesh, (const struct  NewtonMesh*) clipper, (const float*) clipperMatrix, (struct  NewtonMesh**) topMesh, (struct  NewtonMesh**) bottomMesh);
}

-(void) RemoveUnusedVertices: (const struct iNewtonMesh*) mesh: (int*) vertexRemapTable
{
	NewtonRemoveUnusedVertices ((const struct  NewtonMesh*) mesh, (int*) vertexRemapTable);
}

-(void) MeshBeginFace: (const struct iNewtonMesh* const) mesh
{
	NewtonMeshBeginFace ((const struct  NewtonMesh* const) mesh);
}

-(void) MeshAddFace: (const struct iNewtonMesh* const) mesh: (int) vertexCount: (const float*) vertex: (int) strideInBytes: (int) materialIndex
{
	NewtonMeshAddFace ((const struct  NewtonMesh* const) mesh, (int) vertexCount, (const float*) vertex, (int) strideInBytes, (int) materialIndex);
}

-(void) MeshEndFace: (const struct iNewtonMesh* const) mesh
{
	NewtonMeshEndFace ((const struct  NewtonMesh* const) mesh);
}

-(void) MeshBuildFromVertexListIndexList: (const struct iNewtonMesh* const) mesh: (int) faceCount: (const int* const) faceIndexCount: (const int* const) faceMaterialIndex: (const float* const) vertex: (int) vertexStrideInBytes: (const int* const) vertexIndex: (const float* const) normal: (int) normalStrideInBytes: (const int* const) normalIndex: (const float* const) uv0: (int) uv0StrideInBytes: (const int* const) uv0Index: (const float* const) uv1: (int) uv1StrideInBytes: (const int* const) uv1Index
{
	NewtonMeshBuildFromVertexListIndexList ((const struct  NewtonMesh* const) mesh, (int) faceCount, (const int* const) faceIndexCount, (const int* const) faceMaterialIndex, (const float* const) vertex, (int) vertexStrideInBytes, (const int* const) vertexIndex, (const float* const) normal, (int) normalStrideInBytes, (const int* const) normalIndex, (const float* const) uv0, (int) uv0StrideInBytes, (const int* const) uv0Index, (const float* const) uv1, (int) uv1StrideInBytes, (const int* const) uv1Index);
}

-(void) MeshGetVertexStreams: (const struct iNewtonMesh* const) mesh: (int) vertexStrideInByte: (float*) vertex: (int) normalStrideInByte: (float*) normal: (int) uvStrideInByte0: (float*) uv0: (int) uvStrideInByte1: (float*) uv1
{
	NewtonMeshGetVertexStreams ((const struct  NewtonMesh* const) mesh, (int) vertexStrideInByte, (float*) vertex, (int) normalStrideInByte, (float*) normal, (int) uvStrideInByte0, (float*) uv0, (int) uvStrideInByte1, (float*) uv1);
}

-(void) MeshGetIndirectVertexStreams: (const struct iNewtonMesh* const) mesh: (int) vertexStrideInByte: (float*) vertex: (int*) vertexIndices: (int*) vertexCount: (int) normalStrideInByte: (float*) normal: (int*) normalIndices: (int*) normalCount: (int) uvStrideInByte0: (float*) uv0: (int*) uvIndices0: (int*) uvCount0: (int) uvStrideInByte1: (float*) uv1: (int*) uvIndices1: (int*) uvCount1
{
	NewtonMeshGetIndirectVertexStreams ((const struct  NewtonMesh* const) mesh, (int) vertexStrideInByte, (float*) vertex, (int*) vertexIndices, (int*) vertexCount, (int) normalStrideInByte, (float*) normal, (int*) normalIndices, (int*) normalCount, (int) uvStrideInByte0, (float*) uv0, (int*) uvIndices0, (int*) uvCount0, (int) uvStrideInByte1, (float*) uv1, (int*) uvIndices1, (int*) uvCount1);
}

-(void*) MeshBeginHandle: (const struct iNewtonMesh* const) mesh
{
	return (void*) NewtonMeshBeginHandle((const struct  NewtonMesh* const) mesh);
}

-(void) MeshEndHandle: (const struct iNewtonMesh* const) mesh: (void* const) handle
{
	NewtonMeshEndHandle ((const struct  NewtonMesh* const) mesh, (void* const) handle);
}

-(int) MeshFirstMaterial: (const struct iNewtonMesh* const) mesh: (void* const) handle
{
	return (int) NewtonMeshFirstMaterial((const struct  NewtonMesh* const) mesh, (void* const) handle);
}

-(int) MeshNextMaterial: (const struct iNewtonMesh* const) mesh: (void* const) handle: (int) materialId
{
	return (int) NewtonMeshNextMaterial((const struct  NewtonMesh* const) mesh, (void* const) handle, (int) materialId);
}

-(int) MeshMaterialGetMaterial: (const struct iNewtonMesh* const) mesh: (void* const) handle: (int) materialId
{
	return (int) NewtonMeshMaterialGetMaterial((const struct  NewtonMesh* const) mesh, (void* const) handle, (int) materialId);
}

-(int) MeshMaterialGetIndexCount: (const struct iNewtonMesh* const) mesh: (void* const) handle: (int) materialId
{
	return (int) NewtonMeshMaterialGetIndexCount((const struct  NewtonMesh* const) mesh, (void* const) handle, (int) materialId);
}

-(void) MeshMaterialGetIndexStream: (const struct iNewtonMesh* const) mesh: (void* const) handle: (int) materialId: (int*) index
{
	NewtonMeshMaterialGetIndexStream ((const struct  NewtonMesh* const) mesh, (void* const) handle, (int) materialId, (int*) index);
}

-(void) MeshMaterialGetIndexStreamShort: (const struct iNewtonMesh* const) mesh: (void* const) handle: (int) materialId: (short int*) index
{
	NewtonMeshMaterialGetIndexStreamShort ((const struct  NewtonMesh* const) mesh, (void* const) handle, (int) materialId, (short int*) index);
}

-(struct iNewtonMesh*) MeshCreateFirstSingleSegment: (const struct iNewtonMesh* const) mesh
{
	return (struct iNewtonMesh*) NewtonMeshCreateFirstSingleSegment((const struct  NewtonMesh* const) mesh);
}

-(struct iNewtonMesh*) MeshCreateNextSingleSegment: (const struct iNewtonMesh* const) mesh: (const struct iNewtonMesh*) segment
{
	return (struct iNewtonMesh*) NewtonMeshCreateNextSingleSegment((const struct  NewtonMesh* const) mesh, (const struct  NewtonMesh*) segment);
}

-(int) MeshGetTotalFaceCount: (const struct iNewtonMesh* const) mesh
{
	return (int) NewtonMeshGetTotalFaceCount((const struct  NewtonMesh* const) mesh);
}

-(int) MeshGetTotalIndexCount: (const struct iNewtonMesh* const) mesh
{
	return (int) NewtonMeshGetTotalIndexCount((const struct  NewtonMesh* const) mesh);
}

-(void) MeshGetFaces: (const struct iNewtonMesh* const) mesh: (int* const) faceIndexCount: (int* const) faceMaterial: (void** const) faceIndices
{
	NewtonMeshGetFaces ((const struct  NewtonMesh* const) mesh, (int* const) faceIndexCount, (int* const) faceMaterial, (void** const) faceIndices);
}

-(int) MeshGetPointCount: (const struct iNewtonMesh* const) mesh
{
	return (int) NewtonMeshGetPointCount((const struct  NewtonMesh* const) mesh);
}

-(int) MeshGetPointStrideInByte: (const struct iNewtonMesh* const) mesh
{
	return (int) NewtonMeshGetPointStrideInByte((const struct  NewtonMesh* const) mesh);
}

-(float*) MeshGetPointArray: (const struct iNewtonMesh* const) mesh
{
	return (float*) NewtonMeshGetPointArray((const struct  NewtonMesh* const) mesh);
}

-(float*) MeshGetNormalArray: (const struct iNewtonMesh* const) mesh
{
	return (float*) NewtonMeshGetNormalArray((const struct  NewtonMesh* const) mesh);
}

-(float*) MeshGetUV0Array: (const struct iNewtonMesh* const) mesh
{
	return (float*) NewtonMeshGetUV0Array((const struct  NewtonMesh* const) mesh);
}

-(float*) MeshGetUV1Array: (const struct iNewtonMesh* const) mesh
{
	return (float*) NewtonMeshGetUV1Array((const struct  NewtonMesh* const) mesh);
}

-(int) MeshGetVertexCount: (const struct iNewtonMesh* const) mesh
{
	return (int) NewtonMeshGetVertexCount((const struct  NewtonMesh* const) mesh);
}

-(int) MeshGetVertexStrideInByte: (const struct iNewtonMesh* const) mesh
{
	return (int) NewtonMeshGetVertexStrideInByte((const struct  NewtonMesh* const) mesh);
}

-(float*) MeshGetVertexArray: (const struct iNewtonMesh* const) mesh
{
	return (float*) NewtonMeshGetVertexArray((const struct  NewtonMesh* const) mesh);
}

-(void*) MeshGetFirstVertex: (const struct iNewtonMesh* const) mesh
{
	return (void*) NewtonMeshGetFirstVertex((const struct  NewtonMesh* const) mesh);
}

-(void*) MeshGetNextVertex: (const struct iNewtonMesh* const) mesh: (const void*) vertex
{
	return (void*) NewtonMeshGetNextVertex((const struct  NewtonMesh* const) mesh, (const void*) vertex);
}

-(int) MeshGetVertexIndex: (const struct iNewtonMesh* const) mesh: (const void*) vertex
{
	return (int) NewtonMeshGetVertexIndex((const struct  NewtonMesh* const) mesh, (const void*) vertex);
}

-(void*) MeshGetFirstPoint: (const struct iNewtonMesh* const) mesh
{
	return (void*) NewtonMeshGetFirstPoint((const struct  NewtonMesh* const) mesh);
}

-(void*) MeshGetNextPoint: (const struct iNewtonMesh* const) mesh: (const void*) point
{
	return (void*) NewtonMeshGetNextPoint((const struct  NewtonMesh* const) mesh, (const void*) point);
}

-(int) MeshGetPointIndex: (const struct iNewtonMesh* const) mesh: (const void*) point
{
	return (int) NewtonMeshGetPointIndex((const struct  NewtonMesh* const) mesh, (const void*) point);
}

-(int) MeshGetVertexIndexFromPoint: (const struct iNewtonMesh* const) mesh: (const void*) point
{
	return (int) NewtonMeshGetVertexIndexFromPoint((const struct  NewtonMesh* const) mesh, (const void*) point);
}

-(void*) MeshGetFirstEdge: (const struct iNewtonMesh* const) mesh
{
	return (void*) NewtonMeshGetFirstEdge((const struct  NewtonMesh* const) mesh);
}

-(void*) MeshGetNextEdge: (const struct iNewtonMesh* const) mesh: (const void*) edge
{
	return (void*) NewtonMeshGetNextEdge((const struct  NewtonMesh* const) mesh, (const void*) edge);
}

-(void) MeshGetEdgeIndices: (const struct iNewtonMesh* const) mesh: (const void*) edge: (int*) v0: (int*) v1
{
	NewtonMeshGetEdgeIndices ((const struct  NewtonMesh* const) mesh, (const void*) edge, (int*) v0, (int*) v1);
}

-(void*) MeshGetFirstFace: (const struct iNewtonMesh* const) mesh
{
	return (void*) NewtonMeshGetFirstFace((const struct  NewtonMesh* const) mesh);
}

-(void*) MeshGetNextFace: (const struct iNewtonMesh* const) mesh: (const void*) face
{
	return (void*) NewtonMeshGetNextFace((const struct  NewtonMesh* const) mesh, (const void*) face);
}

-(int) MeshIsFaceOpen: (const struct iNewtonMesh* const) mesh: (const void*) face
{
	return (int) NewtonMeshIsFaceOpen((const struct  NewtonMesh* const) mesh, (const void*) face);
}

-(int) MeshGetFaceMaterial: (const struct iNewtonMesh* const) mesh: (const void*) face
{
	return (int) NewtonMeshGetFaceMaterial((const struct  NewtonMesh* const) mesh, (const void*) face);
}

-(int) MeshGetFaceIndexCount: (const struct iNewtonMesh* const) mesh: (const void*) face
{
	return (int) NewtonMeshGetFaceIndexCount((const struct  NewtonMesh* const) mesh, (const void*) face);
}

-(void) MeshGetFaceIndices: (const struct iNewtonMesh* const) mesh: (const void*) face: (int*) indices
{
	NewtonMeshGetFaceIndices ((const struct  NewtonMesh* const) mesh, (const void*) face, (int*) indices);
}

-(void) MeshGetFacePointIndices: (const struct iNewtonMesh* const) mesh: (const void*) face: (int*) indices
{
	NewtonMeshGetFacePointIndices ((const struct  NewtonMesh* const) mesh, (const void*) face, (int*) indices);
}

-(void) CustomDestroyJoint: (const struct iNewtonUserJoint*) joint
{
	CustomDestroyJoint ((const struct  NewtonUserJoint*) joint);
}

-(struct iNewtonJoint*) CustomGetNewtonJoint: (const struct iNewtonUserJoint*) joint
{
	return (struct iNewtonJoint*) CustomGetNewtonJoint((const struct  NewtonUserJoint*) joint);
}

-(int) CustomGetJointID: (const struct iNewtonUserJoint*) joint
{
	return (int) CustomGetJointID((const struct  NewtonUserJoint*) joint);
}

-(void) CustomSetJointID: (const struct iNewtonUserJoint*) joint: (int) rttI
{
	CustomSetJointID ((const struct  NewtonUserJoint*) joint, (int) rttI);
}

-(const struct iNewtonBody*) CustomGetBody0: (const struct iNewtonUserJoint*) joint
{
	return (const struct iNewtonBody*) CustomGetBody0((const struct  NewtonUserJoint*) joint);
}

-(const struct iNewtonBody*) CustomGetBody1: (const struct iNewtonUserJoint*) joint
{
	return (const struct iNewtonBody*) CustomGetBody1((const struct  NewtonUserJoint*) joint);
}

-(int) CustomGetBodiesCollisionState: (const struct iNewtonUserJoint*) joint
{
	return (int) CustomGetBodiesCollisionState((const struct  NewtonUserJoint*) joint);
}

-(void) CustomSetBodiesCollisionState: (const struct iNewtonUserJoint*) joint: (int) state
{
	CustomSetBodiesCollisionState ((const struct  NewtonUserJoint*) joint, (int) state);
}

-(void*) CustomGetUserData: (const struct iNewtonUserJoint*) joint
{
	return (void*) CustomGetUserData((const struct  NewtonUserJoint*) joint);
}

-(void) CustomSetUserData: (const struct iNewtonUserJoint*) joint: (void*) userData
{
	CustomSetUserData ((const struct  NewtonUserJoint*) joint, (void*) userData);
}

-(void) CustomSetDestructorCallback: (const struct iNewtonUserJoint*) joint: (iNewtonUserJointDestructorCallback) callback
{
	CustomSetDestructorCallback ((const struct  NewtonUserJoint*) joint, ( NewtonUserJointDestructorCallback) callback);
}

-(void) CustomSetSubmitContraintCallback: (const struct iNewtonUserJoint*) joint: (iNewtonUserJointSubmitConstraintCallback) callback
{
	CustomSetSubmitContraintCallback ((const struct  NewtonUserJoint*) joint, ( NewtonUserJointSubmitConstraintCallback) callback);
}

-(struct iNewtonUserJoint*) CustomCreateBlankJoint: (int) maxDof: (const struct iNewtonBody*) body0: (const struct iNewtonBody*) body1: (iBlankJointGetInfo) info
{
	return (struct iNewtonUserJoint*) CustomCreateBlankJoint((int) maxDof, (const struct  NewtonBody*) body0, (const struct  NewtonBody*) body1, ( BlankJointGetInfo) info);
}

-(struct iNewtonUserJoint*) CreateCustomKinematicController: (const struct iNewtonBody*) targetBody: (float*) attachmentPointInGlobalSpace
{
	return (struct iNewtonUserJoint*) CreateCustomKinematicController((const struct  NewtonBody*) targetBody, (float*) attachmentPointInGlobalSpace);
}

-(void) CustomKinematicControllerSetPickMode: (const struct iNewtonUserJoint*) pick: (int) mode
{
	CustomKinematicControllerSetPickMode ((const struct  NewtonUserJoint*) pick, (int) mode);
}

-(void) CustomKinematicControllerSetMaxLinearFriction: (const struct iNewtonUserJoint*) pick: (float) accel
{
	CustomKinematicControllerSetMaxLinearFriction ((const struct  NewtonUserJoint*) pick, (float) accel);
}

-(void) CustomKinematicControllerSetMaxAngularFriction: (const struct iNewtonUserJoint*) pick: (float) alpha
{
	CustomKinematicControllerSetMaxAngularFriction ((const struct  NewtonUserJoint*) pick, (float) alpha);
}

-(void) CustomKinematicControllerSetTargetPosit: (const struct iNewtonUserJoint*) pick: (float*) posit
{
	CustomKinematicControllerSetTargetPosit ((const struct  NewtonUserJoint*) pick, (float*) posit);
}

-(void) CustomKinematicControllerSetTargetRotation: (const struct iNewtonUserJoint*) pick: (float*) rotation
{
	CustomKinematicControllerSetTargetRotation ((const struct  NewtonUserJoint*) pick, (float*) rotation);
}

-(void) CustomKinematicControllerSetTargetMatrix: (const struct iNewtonUserJoint*) pick: (float*) matrix
{
	CustomKinematicControllerSetTargetMatrix ((const struct  NewtonUserJoint*) pick, (float*) matrix);
}

-(void) CustomKinematicControllerGetTargetMatrix: (const struct iNewtonUserJoint*) pick: (float*) matrix
{
	CustomKinematicControllerGetTargetMatrix ((const struct  NewtonUserJoint*) pick, (float*) matrix);
}

-(struct iNewtonUserJoint*) CreateCustomJoint6DOF: (const float*) pinsAndPivotChildFrame: (const float*) pinsAndPivotParentFrame: (const struct iNewtonBody*) child: (const struct iNewtonBody*) parent
{
	return (struct iNewtonUserJoint*) CreateCustomJoint6DOF((const float*) pinsAndPivotChildFrame, (const float*) pinsAndPivotParentFrame, (const struct  NewtonBody*) child, (const struct  NewtonBody*) parent);
}

-(void) CustomJoint6DOF_SetLinearLimits: (struct iNewtonUserJoint*) customJoint6DOF: (const float*) minLinearLimits: (const float*) maxLinearLimits
{
	CustomJoint6DOF_SetLinearLimits ((struct  NewtonUserJoint*) customJoint6DOF, (const float*) minLinearLimits, (const float*) maxLinearLimits);
}

-(void) CustomJoint6DOF_SetAngularLimits: (struct iNewtonUserJoint*) customJoint6DOF: (const float*) minAngularLimits: (const float*) maxAngularLimits
{
	CustomJoint6DOF_SetAngularLimits ((struct  NewtonUserJoint*) customJoint6DOF, (const float*) minAngularLimits, (const float*) maxAngularLimits);
}

-(void) CustomJoint6DOF_GetLinearLimits: (struct iNewtonUserJoint*) customJoint6DOF: (float*) minLinearLimits: (float*) maxLinearLimits
{
	CustomJoint6DOF_GetLinearLimits ((struct  NewtonUserJoint*) customJoint6DOF, (float*) minLinearLimits, (float*) maxLinearLimits);
}

-(void) CustomJoint6DOF_GetAngularLimits: (struct iNewtonUserJoint*) customJoint6DOF: (float*) minAngularLimits: (float*) maxAngularLimits
{
	CustomJoint6DOF_GetAngularLimits ((struct  NewtonUserJoint*) customJoint6DOF, (float*) minAngularLimits, (float*) maxAngularLimits);
}

-(void) CustomJoint6DOF_SetReverseUniversal: (struct iNewtonUserJoint*) customJoint6DOF: (int) order
{
	CustomJoint6DOF_SetReverseUniversal ((struct  NewtonUserJoint*) customJoint6DOF, (int) order);
}

-(struct iNewtonUserJoint*) CreateCustomBallAndSocket: (const float*) pinsAndPivotChildFrame: (const struct iNewtonBody*) child: (const struct iNewtonBody*) parent
{
	return (struct iNewtonUserJoint*) CreateCustomBallAndSocket((const float*) pinsAndPivotChildFrame, (const struct  NewtonBody*) child, (const struct  NewtonBody*) parent);
}

-(void) BallAndSocketSetConeAngle: (struct iNewtonUserJoint*) ballJoint: (float) angle
{
	BallAndSocketSetConeAngle ((struct  NewtonUserJoint*) ballJoint, (float) angle);
}

-(void) BallAndSocketSetTwistAngle: (struct iNewtonUserJoint*) ballJoint: (float) minAngle: (float) maxAngle
{
	BallAndSocketSetTwistAngle ((struct  NewtonUserJoint*) ballJoint, (float) minAngle, (float) maxAngle);
}

-(struct iNewtonUserJoint*) CreateCustomHinge: (const float*) pinsAndPivotChildFrame: (const struct iNewtonBody*) child: (const struct iNewtonBody*) parent
{
	return (struct iNewtonUserJoint*) CreateCustomHinge((const float*) pinsAndPivotChildFrame, (const struct  NewtonBody*) child, (const struct  NewtonBody*) parent);
}

-(void) HingeEnableLimits: (struct iNewtonUserJoint*) hingeJoint: (int) state
{
	HingeEnableLimits ((struct  NewtonUserJoint*) hingeJoint, (int) state);
}

-(void) HingeSetLimits: (struct iNewtonUserJoint*) hingeJoint: (float) minAngle: (float) maxAngle
{
	HingeSetLimits ((struct  NewtonUserJoint*) hingeJoint, (float) minAngle, (float) maxAngle);
}

-(float) HingeGetJointAngle: (const struct iNewtonUserJoint*) hingeJoint
{
	return (float) HingeGetJointAngle((const struct  NewtonUserJoint*) hingeJoint);
}

-(void) HingeGetPinAxis: (const struct iNewtonUserJoint*) hingeJoint: (float*) pin
{
	HingeGetPinAxis ((const struct  NewtonUserJoint*) hingeJoint, (float*) pin);
}

-(float) HingeCalculateJointOmega: (const struct iNewtonUserJoint*) hingeJoint
{
	return (float) HingeCalculateJointOmega((const struct  NewtonUserJoint*) hingeJoint);
}

-(struct iNewtonUserJoint*) CreateCustomSlider: (const float*) pinsAndPivotChildFrame: (const struct iNewtonBody*) child: (const struct iNewtonBody*) parent
{
	return (struct iNewtonUserJoint*) CreateCustomSlider((const float*) pinsAndPivotChildFrame, (const struct  NewtonBody*) child, (const struct  NewtonBody*) parent);
}

-(void) SliderEnableLimits: (struct iNewtonUserJoint*) sliderJoint: (int) state
{
	SliderEnableLimits ((struct  NewtonUserJoint*) sliderJoint, (int) state);
}

-(void) SliderSetLimits: (struct iNewtonUserJoint*) sliderJoint: (float) mindist: (float) maxdist
{
	SliderSetLimits ((struct  NewtonUserJoint*) sliderJoint, (float) mindist, (float) maxdist);
}

-(struct iNewtonUserJoint*) CreateCustomPlayerController: (const float*) pins: (const struct iNewtonBody*) player: (float) maxStairStepFactor: (float) cushion
{
	return (struct iNewtonUserJoint*) CreateCustomPlayerController((const float*) pins, (const struct  NewtonBody*) player, (float) maxStairStepFactor, (float) cushion);
}

-(void) CustomPlayerControllerSetVelocity: (const struct iNewtonUserJoint*) playerController: (float) forwardSpeed: (float) sideSpeed: (float) heading
{
	CustomPlayerControllerSetVelocity ((const struct  NewtonUserJoint*) playerController, (float) forwardSpeed, (float) sideSpeed, (float) heading);
}

-(void) CustomPlayerControllerGetVisualMaTrix: (const struct iNewtonUserJoint*) playerController: (float*) matrix
{
	CustomPlayerControllerGetVisualMaTrix ((const struct  NewtonUserJoint*) playerController, (float*) matrix);
}

-(float) CustomPlayerControllerGetMaxSlope: (const struct iNewtonUserJoint*) playerController
{
	return (float) CustomPlayerControllerGetMaxSlope((const struct  NewtonUserJoint*) playerController);
}

-(void) CustomPlayerControllerSetMaxSlope: (const struct iNewtonUserJoint*) playerController: (float) maxSlopeAngleIndRadian
{
	CustomPlayerControllerSetMaxSlope ((const struct  NewtonUserJoint*) playerController, (float) maxSlopeAngleIndRadian);
}

-(const struct iNewtonCollision*) CustomPlayerControllerGetSensorShape: (const struct iNewtonUserJoint*) playerController
{
	return (const struct iNewtonCollision*) CustomPlayerControllerGetSensorShape((const struct  NewtonUserJoint*) playerController);
}

-(struct iNewtonUserJoint*) DGRaycastVehicleCreate: (int) maxTireCount: (const float*) cordenateSytemInLocalSpace: (struct iNewtonBody*) carBody
{
	return (struct iNewtonUserJoint*) DGRaycastVehicleCreate((int) maxTireCount, (const float*) cordenateSytemInLocalSpace, (struct  NewtonBody*) carBody);
}

-(void) DGRaycastVehicleAddTire: (struct iNewtonUserJoint*) car: (void*) userData: (const float*) localPosition: (float) mass: (float) radius: (float) width: (float) friction: (float) suspensionLength: (float) springConst: (float) springDamper: (int) castMode
{
	DGRaycastVehicleAddTire ((struct  NewtonUserJoint*) car, (void*) userData, (const float*) localPosition, (float) mass, (float) radius, (float) width, (float) friction, (float) suspensionLength, (float) springConst, (float) springDamper, (int) castMode);
}

-(void) DGRaycastVehicleSetTireTransformCallback: (struct iNewtonUserJoint*) car: (iDGRaycastVehicleTireTransformCallback) callback
{
	DGRaycastVehicleSetTireTransformCallback ((struct  NewtonUserJoint*) car, ( DGRaycastVehicleTireTransformCallback) callback);
}

-(int) DGRaycastVehicleGetTiresCount: (struct iNewtonUserJoint*) car
{
	return (int) DGRaycastVehicleGetTiresCount((struct  NewtonUserJoint*) car);
}

-(void*) DGRaycastVehicleGetTiresUserData: (struct iNewtonUserJoint*) car: (int) tireIndex
{
	return (void*) DGRaycastVehicleGetTiresUserData((struct  NewtonUserJoint*) car, (int) tireIndex);
}

-(void) DGRaycastVehicleGetTireMatrix: (struct iNewtonUserJoint*) car: (int) tireIndex: (float*) tireMatrix
{
	DGRaycastVehicleGetTireMatrix ((struct  NewtonUserJoint*) car, (int) tireIndex, (float*) tireMatrix);
}

-(void) DGRaycastVehicleInitNormalizeTireLateralForce: (struct iNewtonUserJoint*) car: (int) pointsCount: (float* const) piceSizeStepAxis: (float* const) normalizedForceValue
{
	DGRaycastVehicleInitNormalizeTireLateralForce ((struct  NewtonUserJoint*) car, (int) pointsCount, (float* const) piceSizeStepAxis, (float* const) normalizedForceValue);
}

-(void) DGRaycastVehicleInitNormalizeTireLongitudinalForce: (struct iNewtonUserJoint*) car: (int) pointsCount: (float* const) piceSizeStepAxis: (float* const) normalizedForceValue
{
	DGRaycastVehicleInitNormalizeTireLongitudinalForce ((struct  NewtonUserJoint*) car, (int) pointsCount, (float* const) piceSizeStepAxis, (float* const) normalizedForceValue);
}

@end
