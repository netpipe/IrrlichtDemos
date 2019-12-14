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

#import <Foundation/Foundation.h>


#define NEWTON_MAJOR_VERSION	2
#define NEWTON_MINOR_VERSION	30
#define NEWTON_PROFILER_WORLD_UPDATE	0
#define NEWTON_PROFILER_COLLISION_UPDATE	1
#define NEWTON_PROFILER_FORCE_CALLBACK_UPDATE	2
#define NEWTON_PROFILER_COLLISION_UPDATE_BROAD_PHASE	3
#define NEWTON_PROFILER_COLLISION_UPDATE_NARROW_PHASE	4
#define NEWTON_PROFILER_DYNAMICS_UPDATE	5
#define NEWTON_PROFILER_DYNAMICS_CONSTRAINT_GRAPH	6
#define NEWTON_PROFILER_DYNAMICS_SOLVE_CONSTRAINT_GRAPH	7
#define SERIALIZE_ID_BOX	0
#define SERIALIZE_ID_CONE	1
#define SERIALIZE_ID_SPHERE	2
#define SERIALIZE_ID_CAPSULE	3
#define SERIALIZE_ID_CYLINDER	4
#define SERIALIZE_ID_COMPOUND	5
#define SERIALIZE_ID_CONVEXHULL	6
#define SERIALIZE_ID_CONVEXMODIFIER	7
#define SERIALIZE_ID_CHAMFERCYLINDER	8
#define SERIALIZE_ID_TREE	9
#define SERIALIZE_ID_NULL	10
#define SERIALIZE_ID_HEIGHTFIELD	11
#define SERIALIZE_ID_USERMESH	12
#define SERIALIZE_ID_SCENE	13
#define SERIALIZE_ID_COMPOUND_BREAKABLE	14


typedef struct iNewtonMesh{} iNewtonMesh;
typedef struct iNewtonBody{} iNewtonBody;
typedef struct iNewtonWorld{} iNewtonWorld;
typedef struct iNewtonJoint{} iNewtonJoint;
typedef struct iNewtonMaterial{} iNewtonMaterial;
typedef struct iNewtonCollision{} iNewtonCollision;
typedef struct iNewtonSceneProxy{} iNewtonSceneProxy;
typedef struct iNewtonbreakableComponentMesh{} iNewtonbreakableComponentMesh;
typedef struct iNewtonUserJoint{} iNewtonUserJoint;


struct iNewtonCollisionInfoRecord {
	float m_offsetMatrix[4][4];
	int m_collisionType;
	int m_referenceCount;
	int m_collisionUserID;
	struct iNewtonBoxParam {
		float m_x;
		float m_y;
		float m_z;
	};

	struct iNewtonSphereParam {
		float m_r0;
		float m_r1;
		float m_r2;
	};

	struct iNewtonCylinderParam {
		float m_r0;
		float m_r1;
		float m_height;
	};

	struct iNewtonCapsuleParam {
		float m_r0;
		float m_r1;
		float m_height;
	};

	struct iNewtonConeParam {
		float m_r;
		float m_height;
	};

	struct iNewtonChamferCylinderParam {
		float m_r;
		float m_height;
	};

	struct iNewtonConvexHullParam {
		int m_vertexCount;
		int m_vertexStrideInBytes;
		int m_faceCount;
		float* m_vertex;
	};

	struct iNewtonConvexHullModifierParam {
		struct iNewtonCollision* m_chidren;
	};

	struct iNewtonCompoundCollisionParam {
		int m_chidrenCount;
		struct iNewtonCollision** m_chidren;
	};

	struct iNewtonCollisionTreeParam {
		int m_vertexCount;
		int m_indexCount;
	};

	struct iNewtonHeightFieldCollisionParam {
		int m_width;
		int m_height;
		int m_gridsDiagonals;
		float m_horizonalScale;
		float m_verticalScale;
		unsigned short* m_elevation;
		char* m_atributes;
	};

	struct iNewtonSceneCollisionParam {
		int m_childrenProxyCount;
	};

	union {
		struct iNewtonBoxParam m_box;
		struct iNewtonConeParam m_cone;
		struct iNewtonSphereParam m_sphere;
		struct iNewtonCapsuleParam m_capsule;
		struct iNewtonCylinderParam m_cylinder;
		struct iNewtonChamferCylinderParam m_chamferCylinder;
		struct iNewtonConvexHullParam m_convexHull;
		struct iNewtonCompoundCollisionParam m_compoundCollision;
		struct iNewtonConvexHullModifierParam m_convexHullModifier;
		struct iNewtonCollisionTreeParam m_collisionTree;
		struct iNewtonHeightFieldCollisionParam m_heightField;
		struct iNewtonSceneCollisionParam m_sceneCollision;
		float m_paramArray[64];
	};

};

struct iNewtonJointRecord {
	float m_attachmenMatrix_0[4][4];
	float m_attachmenMatrix_1[4][4];
	float m_minLinearDof[3];
	float m_maxLinearDof[3];
	float m_minAngularDof[3];
	float m_maxAngularDof[3];
	const struct iNewtonBody* m_attachBody_0;
	const struct iNewtonBody* m_attachBody_1;
	float m_extraParameters[16];
	int m_bodiesCollisionOn;
	char m_descriptionType[32];
};

struct iNewtonUserMeshCollisionCollideDesc {
	float m_boxP0[4];
	float m_boxP1[4];
	int m_threadNumber;
	int m_faceCount;
	int m_vertexStrideInBytes;
	void* m_userData;
	float* m_vertex;
	int* m_userAttribute;
	int* m_faceIndexCount;
	int* m_faceVertexIndex;
	struct iNewtonBody* m_objBody;
	struct iNewtonBody* m_polySoupBody;
};

struct iNewtonWorldConvexCastReturnInfo {
	float m_point[4];
	float m_normal[4];
	float m_normalOnHitPoint[4];
	float m_penetration;
	int m_contactID;
	const struct iNewtonBody* m_hitBody;
};

struct iNewtonUserMeshCollisionRayHitDesc {
	float m_p0[4];
	float m_p1[4];
	float m_normalOut[4];
	int m_userIdOut;
	void* m_userData;
};



typedef void* (*iNewtonAllocMemory) (int sizeInBytes);
typedef void (*iNewtonFreeMemory) (void* ptr, int sizeInBytes);
typedef void (*iNewtonDestroyWorld) (const struct iNewtonWorld* newtonWorld);
typedef unsigned (*iNewtonGetTicksCountCallback) ();
typedef void (*iNewtonSerialize) (void* serializeHandle, const void* buffer, int size);
typedef void (*iNewtonDeserialize) (void* serializeHandle, void* buffer, int size);
typedef void (*iNewtonUserMeshCollisionDestroyCallback) (void* userData);
typedef void (*iNewtonUserMeshCollisionCollideCallback) (struct iNewtonUserMeshCollisionCollideDesc* collideDescData);
typedef float (*iNewtonUserMeshCollisionRayHitCallback) (struct iNewtonUserMeshCollisionRayHitDesc* lineDescData);
typedef void (*iNewtonUserMeshCollisionGetCollisionInfo) (void* userData, struct iNewtonCollisionInfoRecord* infoRecord);
typedef int (*iNewtonUserMeshCollisionGetFacesInAABB) (void* userData, const float* p0, const float* p1, const float** vertexArray, int* vertexCount, int* vertexStrideInBytes, const int* indexList, int maxIndexCount, const int* userDataList);
typedef float (*iNewtonCollisionTreeRayCastCallback) (const struct iNewtonBody* const body, const struct iNewtonCollision* const treeCollision, float interception, float* normal, int faceId, void* usedData);
typedef float (*iNewtonHeightFieldRayCastCallback) (const struct iNewtonBody* const body, const struct iNewtonCollision* const heightFieldCollision, float interception, int row, int col, float* normal, int faceId, void* usedData);
typedef void (*iNewtonTreeCollisionCallback) (const struct iNewtonBody* bodyWithTreeCollision, const struct iNewtonBody* body, int faceID, int vertexCount, const float* vertex, int vertexStrideInBytes);
typedef void (*iNewtonBodyDestructor) (const struct iNewtonBody* body);
typedef void (*iNewtonApplyForceAndTorque) (const struct iNewtonBody* body, float timestep, int threadIndex);
typedef void (*iNewtonSetTransform) (const struct iNewtonBody* body, const float* matrix, int threadIndex);
typedef int (*iNewtonIslandUpdate) (const struct iNewtonWorld* world, const void* islandHandle, int bodyCount);
typedef void (*iNewtonBodyLeaveWorld) (const struct iNewtonBody* body, int threadIndex);
typedef void (*iNewtonDestroyBodyByExeciveForce) (const struct iNewtonBody* body, const struct iNewtonJoint* contact);
typedef void (*iNewtonCollisionDestructor) (const struct iNewtonWorld* world, const struct iNewtonCollision* collision);
typedef int (*iNewtonCollisionCompoundBreakableCallback) (struct iNewtonMesh* const mesh, void* userData, float* planeMatrixOut);
typedef int (*iNewtonGetBuoyancyPlane) (const int collisionID, void* context, const float* globalSpaceMatrix, float* globalSpacePlane);
typedef unsigned (*iNewtonWorldRayPrefilterCallback) (const struct iNewtonBody* body, const struct iNewtonCollision* collision, void* userData);
typedef float (*iNewtonWorldRayFilterCallback) (const struct iNewtonBody* body, const float* hitNormal, int collisionID, void* userData, float intersectParam);
typedef int (*iNewtonOnAABBOverlap) (const struct iNewtonMaterial* material, const struct iNewtonBody* body0, const struct iNewtonBody* body1, int threadIndex);
typedef void (*iNewtonContactsProcess) (const struct iNewtonJoint* contact, float timestep, int threadIndex);
typedef void (*iNewtonBodyIterator) (const struct iNewtonBody* body, void* userData);
typedef void (*iNewtonJointIterator) (const struct iNewtonJoint* joint, void* userData);
typedef void (*iNewtonCollisionIterator) (void* userData, int vertexCount, const float* faceArray, int faceId);
typedef void (*iNewtonUserBilateralCallBack) (const struct iNewtonJoint* userJoint, float timestep, int threadIndex);
typedef void (*iNewtonUserBilateralGetInfoCallBack) (const struct iNewtonJoint* userJoint, struct iNewtonJointRecord* info);
typedef void (*iNewtonConstraintDestructor) (const struct iNewtonJoint* me);
typedef void (*iNewtonUserJointDestructorCallback) (const struct iNewtonUserJoint* me);
typedef void (*iNewtonUserJointSubmitConstraintCallback) (const struct iNewtonUserJoint* me, float timestep, int threadIndex);
typedef void (*iBlankJointGetInfo) (const struct iNewtonUserJoint* me, struct iNewtonJointRecord* info);
typedef void (*iDGRaycastVehicleTireTransformCallback) (struct iNewtonUserJoint* car);




@interface iNewton : NSObject
{
	void* m_world;
}

-(id) init;
-(id) initWithAllocator: (iNewtonAllocMemory) allocator: (iNewtonFreeMemory) deallocator;
-(void) dealloc;
-(int) WorldGetVersion;
-(int) WorldFloatSize;
-(int) GetMemoryUsed;
-(void) SetMemorySystem: (iNewtonAllocMemory) malloc: (iNewtonFreeMemory) mfree;
-(struct iNewtonWorld*) Create;
-(void) DestroyAllBodies;
-(void) Update: (float) timestep;
-(void) InvalidateCache;
-(void) CollisionUpdate;
-(void) SetSolverModel: (int) model;
-(void) SetPlatformArchitecture: (int) mode;
-(int) GetPlatformArchitecture: (char*) description;
-(void) SetMultiThreadSolverOnSingleIsland: (int) mode;
-(int) GetMultiThreadSolverOnSingleIsland;
-(void) SetPerformanceClock: (iNewtonGetTicksCountCallback) callback;
-(unsigned) ReadPerformanceTicks: (unsigned) performanceEntry;
-(unsigned) ReadThreadPerformanceTicks: (unsigned) threadIndex;
-(void) WorldCriticalSectionLock;
-(void) WorldCriticalSectionUnlock;
-(void) SetThreadsCount: (int) threads;
-(int) GetThreadsCount;
-(int) GetMaxThreadsCount;
-(void) SetFrictionModel: (int) model;
-(void) SetMinimumFrameRate: (float) frameRate;
-(void) SetBodyLeaveWorldEvent: (iNewtonBodyLeaveWorld) callback;
-(void) SetWorldSize: (const float*) minPoint: (const float*) maxPoint;
-(void) SetIslandUpdateEvent: (iNewtonIslandUpdate) islandUpdate;
-(void) SetCollisionDestructor: (iNewtonCollisionDestructor) callback;
-(void) SetDestroyBodyByExeciveForce: (iNewtonDestroyBodyByExeciveForce) callback;
-(void) WorldForEachJointDo: (iNewtonJointIterator) callback: (void*) userData;
-(void) WorldForEachBodyInAABBDo: (const float*) p0: (const float*) p1: (iNewtonBodyIterator) callback: (void*) userData;
-(void) WorldSetUserData: (void*) userData;
-(void*) WorldGetUserData;
-(void) WorldSetDestructorCallBack: (iNewtonDestroyWorld) destructor;
-(iNewtonDestroyWorld) WorldGetDestructorCallBack;
-(void) WorldRayCast: (const float*) p0: (const float*) p1: (iNewtonWorldRayFilterCallback) filter: (void*) userData: (iNewtonWorldRayPrefilterCallback) prefilter;
-(int) WorldConvexCast: (const float*) matrix: (const float*) target: (const struct iNewtonCollision*) shape: (float*) hitParam: (void*) userData: (iNewtonWorldRayPrefilterCallback) prefilter: (struct iNewtonWorldConvexCastReturnInfo*) info: (int) maxContactsCount: (int) threadIndex;
-(int) WorldGetBodyCount;
-(int) WorldGetConstraintCount;
-(struct iNewtonBody*) IslandGetBody: (const void*) island: (int) bodyIndex;
-(void) IslandGetBodyAABB: (const void*) island: (int) bodyIndex: (const float*) p0: (const float*) p1;
-(int) MaterialCreateGroupID;
-(int) MaterialGetDefaultGroupID;
-(void) MaterialDestroyAllGroupID;
-(void*) MaterialGetUserData: (int) id0: (int) id1;
-(void) MaterialSetSurfaceThickness: (int) id0: (int) id1: (float) thickness;
-(void) MaterialSetContinuousCollisionMode: (int) id0: (int) id1: (int) state;
-(void) MaterialSetCollisionCallback: (int) id0: (int) id1: (void*) userData: (iNewtonOnAABBOverlap) aabbOverlap: (iNewtonContactsProcess) process;
-(void) MaterialSetDefaultSoftness: (int) id0: (int) id1: (float) value;
-(void) MaterialSetDefaultElasticity: (int) id0: (int) id1: (float) elasticCoef;
-(void) MaterialSetDefaultCollidable: (int) id0: (int) id1: (int) state;
-(void) MaterialSetDefaultFriction: (int) id0: (int) id1: (float) staticFriction: (float) kineticFriction;
-(struct iNewtonMaterial*) WorldGetFirstMaterial;
-(struct iNewtonMaterial*) WorldGetNextMaterial: (const struct iNewtonMaterial*) material;
-(struct iNewtonBody*) WorldGetFirstBody;
-(struct iNewtonBody*) WorldGetNextBody: (const struct iNewtonBody*) curBody;
-(void*) MaterialGetMaterialPairUserData: (const struct iNewtonMaterial*) material;
-(unsigned) MaterialGetContactFaceAttribute: (const struct iNewtonMaterial*) material;
-(unsigned) MaterialGetBodyCollisionID: (const struct iNewtonMaterial*) material: (const struct iNewtonBody*) body;
-(float) MaterialGetContactNormalSpeed: (const struct iNewtonMaterial*) material;
-(void) MaterialGetContactForce: (const struct iNewtonMaterial*) material: (float*) force;
-(void) MaterialGetContactPositionAndNormal: (const struct iNewtonMaterial*) material: (float*) posit: (float*) normal;
-(void) MaterialGetContactTangentDirections: (const struct iNewtonMaterial*) material: (float*) dir0: (float*) dir1;
-(float) MaterialGetContactTangentSpeed: (const struct iNewtonMaterial*) material: (int) index;
-(void) MaterialSetContactSoftness: (const struct iNewtonMaterial*) material: (float) softness;
-(void) MaterialSetContactElasticity: (const struct iNewtonMaterial*) material: (float) restitution;
-(void) MaterialSetContactFrictionState: (const struct iNewtonMaterial*) material: (int) state: (int) index;
-(void) MaterialSetContactFrictionCoef: (const struct iNewtonMaterial*) material: (float) staticFrictionCoef: (float) kineticFrictionCoef: (int) index;
-(void) MaterialSetContactNormalAcceleration: (const struct iNewtonMaterial*) material: (float) accel;
-(void) MaterialSetContactNormalDirection: (const struct iNewtonMaterial*) material: (const float*) directionVector;
-(void) MaterialSetContactTangentAcceleration: (const struct iNewtonMaterial*) material: (float) accel: (int) index;
-(void) MaterialContactRotateTangentDirections: (const struct iNewtonMaterial*) material: (const float*) directionVector;
-(struct iNewtonCollision*) CreateNull;
-(struct iNewtonCollision*) CreateSphere: (float) radiusX: (float) radiusY: (float) radiusZ: (int) shapeID: (const float*) offsetMatrix;
-(struct iNewtonCollision*) CreateBox: (float) dx: (float) dy: (float) dz: (int) shapeID: (const float*) offsetMatrix;
-(struct iNewtonCollision*) CreateCone: (float) radius: (float) height: (int) shapeID: (const float*) offsetMatrix;
-(struct iNewtonCollision*) CreateCapsule: (float) radius: (float) height: (int) shapeID: (const float*) offsetMatrix;
-(struct iNewtonCollision*) CreateCylinder: (float) radius: (float) height: (int) shapeID: (const float*) offsetMatrix;
-(struct iNewtonCollision*) CreateChamferCylinder: (float) radius: (float) height: (int) shapeID: (const float*) offsetMatrix;
-(struct iNewtonCollision*) CreateConvexHull: (int) count: (const float*) vertexCloud: (int) strideInBytes: (float) tolerance: (int) shapeID: (const float*) offsetMatrix;
-(struct iNewtonCollision*) CreateConvexHullFromMesh: (const struct iNewtonMesh*) mesh: (float) tolerance: (int) shapeID;
-(struct iNewtonCollision*) CreateConvexHullModifier: (const struct iNewtonCollision*) convexHullCollision: (int) shapeID;
-(void) ConvexHullModifierGetMatrix: (const struct iNewtonCollision*) convexHullCollision: (float*) matrix;
-(void) ConvexHullModifierSetMatrix: (const struct iNewtonCollision*) convexHullCollision: (const float*) matrix;
-(int) CollisionIsTriggerVolume: (const struct iNewtonCollision*) convexCollision;
-(void) CollisionSetAsTriggerVolume: (const struct iNewtonCollision*) convexCollision: (int) trigger;
-(void) CollisionSetMaxBreakImpactImpulse: (const struct iNewtonCollision*) convexHullCollision: (float) maxImpactImpulse;
-(float) CollisionGetMaxBreakImpactImpulse: (const struct iNewtonCollision*) convexHullCollision;
-(void) CollisionSetUserID: (const struct iNewtonCollision*) convexCollision: (unsigned) id;
-(unsigned) CollisionGetUserID: (const struct iNewtonCollision*) convexCollision;
-(int) ConvexHullGetFaceIndices: (const struct iNewtonCollision*) convexHullCollision: (int) face: (int*) faceIndices;
-(float) ConvexCollisionCalculateVolume: (const struct iNewtonCollision*) convexCollision;
-(void) ConvexCollisionCalculateInertialMatrix: (const struct iNewtonCollision*) convexCollision: (float*) inertia: (float*) origin;
-(void) CollisionMakeUnique: (const struct iNewtonCollision*) collision;
-(void) ReleaseCollision: (const struct iNewtonCollision*) collision;
-(int) AddCollisionReference: (const struct iNewtonCollision*) collision;
-(struct iNewtonCollision*) CreateCompoundCollision: (int) count: (struct iNewtonCollision* const*) collisionPrimitiveArray: (int) shapeID;
-(struct iNewtonCollision*) CreateCompoundCollisionFromMesh: (const struct iNewtonMesh*) mesh: (int) maxSubShapesCount: (int) shapeID: (int) subShapeID;
-(struct iNewtonCollision*) CreateCompoundBreakable: (int) meshCount: (const struct iNewtonMesh* const*) solids: (const int* const) shapeIDArray: (const float* const) densities: (const int* const) internalFaceMaterial: (int) shapeID: (int) debriID: (float) debriSeparationGap;
-(void) CompoundBreakableResetAnchoredPieces: (const struct iNewtonCollision*) compoundBreakable;
-(void) CompoundBreakableSetAnchoredPieces: (const struct iNewtonCollision*) compoundBreakable: (int) fixShapesCount: (float* const) matrixPallete: (struct iNewtonCollision**) fixedShapesArray;
-(int) CompoundBreakableGetVertexCount: (const struct iNewtonCollision*) compoundBreakable;
-(void) CompoundBreakableGetVertexStreams: (const struct iNewtonCollision*) compoundBreakable: (int) vertexStrideInByte: (float*) vertex: (int) normalStrideInByte: (float*) normal: (int) uvStrideInByte: (float*) uv;
-(struct iNewtonbreakableComponentMesh*) BreakableGetMainMesh: (const struct iNewtonCollision*) compoundBreakable;
-(struct iNewtonbreakableComponentMesh*) BreakableGetFirstComponent: (const struct iNewtonCollision*) compoundBreakable;
-(struct iNewtonbreakableComponentMesh*) BreakableGetNextComponent: (const struct iNewtonbreakableComponentMesh*) component;
-(void) BreakableBeginDelete: (const struct iNewtonCollision*) compoundBreakable;
-(struct iNewtonBody*) BreakableCreateDebrieBody: (const struct iNewtonCollision*) compoundBreakable: (const struct iNewtonbreakableComponentMesh*) component;
-(void) BreakableDeleteComponent: (const struct iNewtonCollision*) compoundBreakable: (const struct iNewtonbreakableComponentMesh*) component;
-(void) BreakableEndDelete: (const struct iNewtonCollision*) compoundBreakable;
-(int) BreakableGetComponentsInRadius: (const struct iNewtonCollision*) compoundBreakable: (const float*) position: (float) radius: (struct iNewtonbreakableComponentMesh**) segments: (int) maxCount;
-(void*) BreakableGetFirstSegment: (const struct iNewtonbreakableComponentMesh*) breakableComponent;
-(void*) BreakableGetNextSegment: (const void*) segment;
-(int) BreakableSegmentGetMaterial: (const void*) segment;
-(int) BreakableSegmentGetIndexCount: (const void*) segment;
-(int) BreakableSegmentGetIndexStream: (const struct iNewtonCollision*) compoundBreakable: (const struct iNewtonbreakableComponentMesh*) meshOwner: (const void*) segment: (int*) index;
-(int) BreakableSegmentGetIndexStreamShort: (const struct iNewtonCollision*) compoundBreakable: (const struct iNewtonbreakableComponentMesh*) meshOwner: (const void*) segment: (short int*) index;
-(struct iNewtonCollision*) CreateUserMeshCollision: (const float*) minBox: (const float*) maxBox: (void*) userData: (iNewtonUserMeshCollisionCollideCallback) collideCallback: (iNewtonUserMeshCollisionRayHitCallback) rayHitCallback: (iNewtonUserMeshCollisionDestroyCallback) destroyCallback: (iNewtonUserMeshCollisionGetCollisionInfo) getInfoCallback: (iNewtonUserMeshCollisionGetFacesInAABB) facesInAABBCallback: (int) shapeID;
-(struct iNewtonCollision*) CreateSceneCollision: (int) shapeID;
-(struct iNewtonSceneProxy*) SceneCollisionCreateProxy: (struct iNewtonCollision*) scene: (struct iNewtonCollision*) collision;
-(void) SceneCollisionDestroyProxy: (struct iNewtonCollision*) scene: (struct iNewtonSceneProxy*) Proxy;
-(void) SceneProxySetMatrix: (struct iNewtonSceneProxy*) proxy: (const float*) matrix;
-(void) SceneProxyGetMatrix: (struct iNewtonSceneProxy*) proxy: (float*) matrix;
-(void) SceneCollisionOptimize: (struct iNewtonCollision*) scene;
-(struct iNewtonCollision*) CreateCollisionFromSerialization: (iNewtonDeserialize) deserializeFunction: (void*) serializeHandle;
-(void) CollisionSerialize: (const struct iNewtonCollision*) collision: (iNewtonSerialize) serializeFunction: (void*) serializeHandle;
-(void) CollisionGetInfo: (const struct iNewtonCollision*) collision: (struct iNewtonCollisionInfoRecord*) collisionInfo;
-(struct iNewtonCollision*) CreateHeightFieldCollision: (int) width: (int) height: (int) gridsDiagonals: (unsigned short*) elevationMap: (char*) attributeMap: (float) horizontalScale: (float) verticalScale: (int) shapeID;
-(void) HeightFieldSetUserRayCastCallback: (const struct iNewtonCollision*) treeCollision: (iNewtonHeightFieldRayCastCallback) rayHitCallback;
-(struct iNewtonCollision*) CreateTreeCollision: (int) shapeID;
-(void) TreeCollisionSetUserRayCastCallback: (const struct iNewtonCollision*) treeCollision: (iNewtonCollisionTreeRayCastCallback) rayHitCallback;
-(void) TreeCollisionBeginBuild: (const struct iNewtonCollision*) treeCollision;
-(void) TreeCollisionAddFace: (const struct iNewtonCollision*) treeCollision: (int) vertexCount: (const float*) vertexPtr: (int) strideInBytes: (int) faceAttribute;
-(void) TreeCollisionEndBuild: (const struct iNewtonCollision*) treeCollision: (int) optimize;
-(int) TreeCollisionGetFaceAtribute: (const struct iNewtonCollision*) treeCollision: (const int*) faceIndexArray;
-(void) TreeCollisionSetFaceAtribute: (const struct iNewtonCollision*) treeCollision: (const int*) faceIndexArray: (int) attribute;
-(int) TreeCollisionGetVertexListIndexListInAABB: (const struct iNewtonCollision*) treeCollision: (const float*) p0: (const float*) p1: (const float**) vertexArray: (int*) vertexCount: (int*) vertexStrideInBytes: (const int*) indexList: (int) maxIndexCount: (const int*) faceAttribute;
-(void) StaticCollisionSetDebugCallback: (const struct iNewtonCollision*) staticCollision: (iNewtonTreeCollisionCallback) userCallback;
-(int) CollisionPointDistance: (const float*) point: (const struct iNewtonCollision*) collision: (const float*) matrix: (float*) contact: (float*) normal: (int) threadIndex;
-(int) CollisionClosestPoint: (const struct iNewtonCollision*) collisionA: (const float*) matrixA: (const struct iNewtonCollision*) collisionB: (const float*) matrixB: (float*) contactA: (float*) contactB: (float*) normalAB: (int) threadIndex;
-(int) CollisionCollide: (int) maxSize: (const struct iNewtonCollision*) collisionA: (const float*) matrixA: (const struct iNewtonCollision*) collisionB: (const float*) matrixB: (float*) contacts: (float*) normals: (float*) penetration: (int) threadIndex;
-(int) CollisionCollideContinue: (int) maxSize: (const float) timestep: (const struct iNewtonCollision*) collisionA: (const float*) matrixA: (const float*) velocA: (const float*) omegaA: (const struct iNewtonCollision*) collisionB: (const float*) matrixB: (const float*) velocB: (const float*) omegaB: (float*) timeOfImpact: (float*) contacts: (float*) normals: (float*) penetration: (int) threadIndex;
-(void) CollisionSupportVertex: (const struct iNewtonCollision*) collision: (const float*) dir: (float*) vertex;
-(float) CollisionRayCast: (const struct iNewtonCollision*) collision: (const float*) p0: (const float*) p1: (float*) normals: (int*) attribute;
-(void) CollisionCalculateAABB: (const struct iNewtonCollision*) collision: (const float*) matrix: (float*) p0: (float*) p1;
-(void) CollisionForEachPolygonDo: (const struct iNewtonCollision*) collision: (const float*) matrix: (iNewtonCollisionIterator) callback: (void*) userData;
-(void) GetEulerAngle: (const float*) matrix: (float*) eulersAngles;
-(void) SetEulerAngle: (const float*) eulersAngles: (float*) matrix;
-(float) CalculateSpringDamperAcceleration: (float) dt: (float) ks: (float) x: (float) kd: (float) s;
-(struct iNewtonBody*) CreateBody: (const struct iNewtonCollision*) collision: (const float*) matrix;
-(void) DestroyBody: (const struct iNewtonBody*) body;
-(void) BodyAddForce: (const struct iNewtonBody*) body: (const float*) force;
-(void) BodyAddTorque: (const struct iNewtonBody*) body: (const float*) torque;
-(void) BodyCalculateInverseDynamicsForce: (const struct iNewtonBody*) body: (float) timestep: (const float*) desiredVeloc: (float*) forceOut;
-(void) BodySetMatrix: (const struct iNewtonBody*) body: (const float*) matrix;
-(void) BodySetMatrixRecursive: (const struct iNewtonBody*) body: (const float*) matrix;
-(void) BodySetMassMatrix: (const struct iNewtonBody*) body: (float) mass: (float) Ixx: (float) Iyy: (float) Izz;
-(void) BodySetMaterialGroupID: (const struct iNewtonBody*) body: (int) id;
-(void) BodySetContinuousCollisionMode: (const struct iNewtonBody*) body: (unsigned) state;
-(void) BodySetJointRecursiveCollision: (const struct iNewtonBody*) body: (unsigned) state;
-(void) BodySetOmega: (const struct iNewtonBody*) body: (const float*) omega;
-(void) BodySetVelocity: (const struct iNewtonBody*) body: (const float*) velocity;
-(void) BodySetForce: (const struct iNewtonBody*) body: (const float*) force;
-(void) BodySetTorque: (const struct iNewtonBody*) body: (const float*) torque;
-(void) BodySetCentreOfMass: (const struct iNewtonBody*) body: (const float*) com;
-(void) BodySetLinearDamping: (const struct iNewtonBody*) body: (float) linearDamp;
-(void) BodySetAngularDamping: (const struct iNewtonBody*) body: (const float*) angularDamp;
-(void) BodySetUserData: (const struct iNewtonBody*) body: (void*) userData;
-(void) BodySetCollision: (const struct iNewtonBody*) body: (const struct iNewtonCollision*) collision;
-(int) BodyGetSleepState: (const struct iNewtonBody*) body;
-(int) BodyGetAutoSleep: (const struct iNewtonBody*) body;
-(void) BodySetAutoSleep: (const struct iNewtonBody*) body: (int) state;
-(int) BodyGetFreezeState: (const struct iNewtonBody*) body;
-(void) BodySetFreezeState: (const struct iNewtonBody*) body: (int) state;
-(void) BodySetDestructorCallback: (const struct iNewtonBody*) body: (iNewtonBodyDestructor) callback;
-(void) BodySetTransformCallback: (const struct iNewtonBody*) body: (iNewtonSetTransform) callback;
-(iNewtonSetTransform) BodyGetTransformCallback: (const struct iNewtonBody*) body;
-(void) BodySetForceAndTorqueCallback: (const struct iNewtonBody*) body: (iNewtonApplyForceAndTorque) callback;
-(iNewtonApplyForceAndTorque) BodyGetForceAndTorqueCallback: (const struct iNewtonBody*) body;
-(void*) BodyGetUserData: (const struct iNewtonBody*) body;
-(struct iNewtonWorld*) BodyGetWorld: (const struct iNewtonBody*) body;
-(struct iNewtonCollision*) BodyGetCollision: (const struct iNewtonBody*) body;
-(int) BodyGetMaterialGroupID: (const struct iNewtonBody*) body;
-(int) BodyGetContinuousCollisionMode: (const struct iNewtonBody*) body;
-(int) BodyGetJointRecursiveCollision: (const struct iNewtonBody*) body;
-(void) BodyGetMatrix: (const struct iNewtonBody*) body: (float*) matrix;
-(void) BodyGetRotation: (const struct iNewtonBody*) body: (float*) rotation;
-(void) BodyGetMassMatrix: (const struct iNewtonBody*) body: (float*) mass: (float*) Ixx: (float*) Iyy: (float*) Izz;
-(void) BodyGetInvMass: (const struct iNewtonBody*) body: (float*) invMass: (float*) invIxx: (float*) invIyy: (float*) invIzz;
-(void) BodyGetOmega: (const struct iNewtonBody*) body: (float*) vector;
-(void) BodyGetVelocity: (const struct iNewtonBody*) body: (float*) vector;
-(void) BodyGetForce: (const struct iNewtonBody*) body: (float*) vector;
-(void) BodyGetTorque: (const struct iNewtonBody*) body: (float*) vector;
-(void) BodyGetForceAcc: (const struct iNewtonBody*) body: (float*) vector;
-(void) BodyGetTorqueAcc: (const struct iNewtonBody*) body: (float*) vector;
-(void) BodyGetCentreOfMass: (const struct iNewtonBody*) body: (float*) com;
-(float) BodyGetLinearDamping: (const struct iNewtonBody*) body;
-(void) BodyGetAngularDamping: (const struct iNewtonBody*) body: (float*) vector;
-(void) BodyGetAABB: (const struct iNewtonBody*) body: (float*) p0: (float*) p1;
-(struct iNewtonJoint*) BodyGetFirstJoint: (const struct iNewtonBody*) body;
-(struct iNewtonJoint*) BodyGetNextJoint: (const struct iNewtonBody*) body: (const struct iNewtonJoint*) joint;
-(struct iNewtonJoint*) BodyGetFirstContactJoint: (const struct iNewtonBody*) body;
-(struct iNewtonJoint*) BodyGetNextContactJoint: (const struct iNewtonBody*) body: (const struct iNewtonJoint*) contactJoint;
-(void*) ContactJointGetFirstContact: (const struct iNewtonJoint*) contactJoint;
-(void*) ContactJointGetNextContact: (const struct iNewtonJoint*) contactJoint: (void*) contact;
-(int) ContactJointGetContactCount: (const struct iNewtonJoint*) contactJoint;
-(void) ContactJointRemoveContact: (const struct iNewtonJoint*) contactJoint: (void*) contact;
-(struct iNewtonMaterial*) ContactGetMaterial: (const void*) contact;
-(void) BodyAddBuoyancyForce: (const struct iNewtonBody*) body: (float) fluidDensity: (float) fluidLinearViscosity: (float) fluidAngularViscosity: (const float*) gravityVector: (iNewtonGetBuoyancyPlane) buoyancyPlane: (void*) context;
-(void) BodyAddImpulse: (const struct iNewtonBody*) body: (const float*) pointDeltaVeloc: (const float*) pointPosit;
-(void*) JointGetUserData: (const struct iNewtonJoint*) joint;
-(void) JointSetUserData: (const struct iNewtonJoint*) joint: (void*) userData;
-(struct iNewtonBody*) JointGetBody0: (const struct iNewtonJoint*) joint;
-(struct iNewtonBody*) JointGetBody1: (const struct iNewtonJoint*) joint;
-(void) JointGetInfo: (const struct iNewtonJoint*) joint: (struct iNewtonJointRecord*) info;
-(int) JointGetCollisionState: (const struct iNewtonJoint*) joint;
-(void) JointSetCollisionState: (const struct iNewtonJoint*) joint: (int) state;
-(float) JointGetStiffness: (const struct iNewtonJoint*) joint;
-(void) JointSetStiffness: (const struct iNewtonJoint*) joint: (float) state;
-(void) DestroyJoint: (const struct iNewtonJoint*) joint;
-(void) JointSetDestructor: (const struct iNewtonJoint*) joint: (iNewtonConstraintDestructor) destructor;
-(struct iNewtonJoint*) ConstraintCreateUpVector: (const float*) pinDir: (const struct iNewtonBody*) body;
-(void) UpVectorGetPin: (const struct iNewtonJoint*) upVector: (float*) pin;
-(void) UpVectorSetPin: (const struct iNewtonJoint*) upVector: (const float*) pin;
-(struct iNewtonJoint*) ConstraintCreateUserJoint: (int) maxDOF: (iNewtonUserBilateralCallBack) callback: (iNewtonUserBilateralGetInfoCallBack) getInfo: (const struct iNewtonBody*) childBody: (const struct iNewtonBody*) parentBody;
-(void) UserJointSetFeedbackCollectorCallback: (const struct iNewtonJoint*) joint: (iNewtonUserBilateralCallBack) getFeedback;
-(void) UserJointAddLinearRow: (const struct iNewtonJoint*) joint: (const float*) pivot0: (const float*) pivot1: (const float*) dir;
-(void) UserJointAddAngularRow: (const struct iNewtonJoint*) joint: (float) relativeAngle: (const float*) dir;
-(void) UserJointAddGeneralRow: (const struct iNewtonJoint*) joint: (const float*) jacobian0: (const float*) jacobian1;
-(void) UserJointSetRowMinimumFriction: (const struct iNewtonJoint*) joint: (float) friction;
-(void) UserJointSetRowMaximumFriction: (const struct iNewtonJoint*) joint: (float) friction;
-(void) UserJointSetRowAcceleration: (const struct iNewtonJoint*) joint: (float) acceleration;
-(void) UserJointSetRowSpringDamperAcceleration: (const struct iNewtonJoint*) joint: (float) springK: (float) springD;
-(void) UserJointSetRowStiffness: (const struct iNewtonJoint*) joint: (float) stiffness;
-(float) UserJointGetRowForce: (const struct iNewtonJoint*) joint: (int) row;
-(struct iNewtonMesh*) MeshCreate;
-(struct iNewtonMesh*) MeshCreateFromMesh: (const struct iNewtonMesh*) mesh;
-(struct iNewtonMesh*) MeshCreateFromCollision: (const struct iNewtonCollision*) collision;
-(struct iNewtonMesh*) MeshConvexHull: (int) count: (const float*) vertexCloud: (int) strideInBytes: (float) tolerance;
-(struct iNewtonMesh*) MeshCreatePlane: (const float*) locationMatrix: (float) witdth: (float) breadth: (int) material: (const float*) textureMatrix0: (const float*) textureMatrix1;
-(void) MeshDestroy: (const struct iNewtonMesh*) mesh;
-(void) MeshCalculateOOBB: (const struct iNewtonMesh*) mesh: (float*) matrix: (float*) x: (float*) y: (float*) z;
-(void) MeshCalculateVertexNormals: (const struct iNewtonMesh*) mesh: (float) angleInRadians;
-(void) MeshApplySphericalMapping: (const struct iNewtonMesh*) mesh: (int) material;
-(void) MeshApplyBoxMapping: (const struct iNewtonMesh*) mesh: (int) front: (int) side: (int) top;
-(void) MeshApplyCylindricalMapping: (const struct iNewtonMesh*) mesh: (int) cylinderMaterial: (int) capMaterial;
-(int) MeshIsOpenMesh: (const struct iNewtonMesh*) mesh;
-(void) MeshFixTJoints: (const struct iNewtonMesh*) mesh;
-(void) MeshPolygonize: (const struct iNewtonMesh*) mesh;
-(void) MeshTriangulate: (const struct iNewtonMesh*) mesh;
-(struct iNewtonMesh*) MeshUnion: (const struct iNewtonMesh*) mesh: (const struct iNewtonMesh*) clipper: (const float*) clipperMatrix;
-(struct iNewtonMesh*) MeshDifference: (const struct iNewtonMesh*) mesh: (const struct iNewtonMesh*) clipper: (const float*) clipperMatrix;
-(struct iNewtonMesh*) MeshIntersection: (const struct iNewtonMesh*) mesh: (const struct iNewtonMesh*) clipper: (const float*) clipperMatrix;
-(void) MeshClip: (const struct iNewtonMesh*) mesh: (const struct iNewtonMesh*) clipper: (const float*) clipperMatrix: (struct iNewtonMesh**) topMesh: (struct iNewtonMesh**) bottomMesh;
-(void) RemoveUnusedVertices: (const struct iNewtonMesh*) mesh: (int*) vertexRemapTable;
-(void) MeshBeginFace: (const struct iNewtonMesh* const) mesh;
-(void) MeshAddFace: (const struct iNewtonMesh* const) mesh: (int) vertexCount: (const float*) vertex: (int) strideInBytes: (int) materialIndex;
-(void) MeshEndFace: (const struct iNewtonMesh* const) mesh;
-(void) MeshBuildFromVertexListIndexList: (const struct iNewtonMesh* const) mesh: (int) faceCount: (const int* const) faceIndexCount: (const int* const) faceMaterialIndex: (const float* const) vertex: (int) vertexStrideInBytes: (const int* const) vertexIndex: (const float* const) normal: (int) normalStrideInBytes: (const int* const) normalIndex: (const float* const) uv0: (int) uv0StrideInBytes: (const int* const) uv0Index: (const float* const) uv1: (int) uv1StrideInBytes: (const int* const) uv1Index;
-(void) MeshGetVertexStreams: (const struct iNewtonMesh* const) mesh: (int) vertexStrideInByte: (float*) vertex: (int) normalStrideInByte: (float*) normal: (int) uvStrideInByte0: (float*) uv0: (int) uvStrideInByte1: (float*) uv1;
-(void) MeshGetIndirectVertexStreams: (const struct iNewtonMesh* const) mesh: (int) vertexStrideInByte: (float*) vertex: (int*) vertexIndices: (int*) vertexCount: (int) normalStrideInByte: (float*) normal: (int*) normalIndices: (int*) normalCount: (int) uvStrideInByte0: (float*) uv0: (int*) uvIndices0: (int*) uvCount0: (int) uvStrideInByte1: (float*) uv1: (int*) uvIndices1: (int*) uvCount1;
-(void*) MeshBeginHandle: (const struct iNewtonMesh* const) mesh;
-(void) MeshEndHandle: (const struct iNewtonMesh* const) mesh: (void* const) handle;
-(int) MeshFirstMaterial: (const struct iNewtonMesh* const) mesh: (void* const) handle;
-(int) MeshNextMaterial: (const struct iNewtonMesh* const) mesh: (void* const) handle: (int) materialId;
-(int) MeshMaterialGetMaterial: (const struct iNewtonMesh* const) mesh: (void* const) handle: (int) materialId;
-(int) MeshMaterialGetIndexCount: (const struct iNewtonMesh* const) mesh: (void* const) handle: (int) materialId;
-(void) MeshMaterialGetIndexStream: (const struct iNewtonMesh* const) mesh: (void* const) handle: (int) materialId: (int*) index;
-(void) MeshMaterialGetIndexStreamShort: (const struct iNewtonMesh* const) mesh: (void* const) handle: (int) materialId: (short int*) index;
-(struct iNewtonMesh*) MeshCreateFirstSingleSegment: (const struct iNewtonMesh* const) mesh;
-(struct iNewtonMesh*) MeshCreateNextSingleSegment: (const struct iNewtonMesh* const) mesh: (const struct iNewtonMesh*) segment;
-(int) MeshGetTotalFaceCount: (const struct iNewtonMesh* const) mesh;
-(int) MeshGetTotalIndexCount: (const struct iNewtonMesh* const) mesh;
-(void) MeshGetFaces: (const struct iNewtonMesh* const) mesh: (int* const) faceIndexCount: (int* const) faceMaterial: (void** const) faceIndices;
-(int) MeshGetPointCount: (const struct iNewtonMesh* const) mesh;
-(int) MeshGetPointStrideInByte: (const struct iNewtonMesh* const) mesh;
-(float*) MeshGetPointArray: (const struct iNewtonMesh* const) mesh;
-(float*) MeshGetNormalArray: (const struct iNewtonMesh* const) mesh;
-(float*) MeshGetUV0Array: (const struct iNewtonMesh* const) mesh;
-(float*) MeshGetUV1Array: (const struct iNewtonMesh* const) mesh;
-(int) MeshGetVertexCount: (const struct iNewtonMesh* const) mesh;
-(int) MeshGetVertexStrideInByte: (const struct iNewtonMesh* const) mesh;
-(float*) MeshGetVertexArray: (const struct iNewtonMesh* const) mesh;
-(void*) MeshGetFirstVertex: (const struct iNewtonMesh* const) mesh;
-(void*) MeshGetNextVertex: (const struct iNewtonMesh* const) mesh: (const void*) vertex;
-(int) MeshGetVertexIndex: (const struct iNewtonMesh* const) mesh: (const void*) vertex;
-(void*) MeshGetFirstPoint: (const struct iNewtonMesh* const) mesh;
-(void*) MeshGetNextPoint: (const struct iNewtonMesh* const) mesh: (const void*) point;
-(int) MeshGetPointIndex: (const struct iNewtonMesh* const) mesh: (const void*) point;
-(int) MeshGetVertexIndexFromPoint: (const struct iNewtonMesh* const) mesh: (const void*) point;
-(void*) MeshGetFirstEdge: (const struct iNewtonMesh* const) mesh;
-(void*) MeshGetNextEdge: (const struct iNewtonMesh* const) mesh: (const void*) edge;
-(void) MeshGetEdgeIndices: (const struct iNewtonMesh* const) mesh: (const void*) edge: (int*) v0: (int*) v1;
-(void*) MeshGetFirstFace: (const struct iNewtonMesh* const) mesh;
-(void*) MeshGetNextFace: (const struct iNewtonMesh* const) mesh: (const void*) face;
-(int) MeshIsFaceOpen: (const struct iNewtonMesh* const) mesh: (const void*) face;
-(int) MeshGetFaceMaterial: (const struct iNewtonMesh* const) mesh: (const void*) face;
-(int) MeshGetFaceIndexCount: (const struct iNewtonMesh* const) mesh: (const void*) face;
-(void) MeshGetFaceIndices: (const struct iNewtonMesh* const) mesh: (const void*) face: (int*) indices;
-(void) MeshGetFacePointIndices: (const struct iNewtonMesh* const) mesh: (const void*) face: (int*) indices;
-(void) CustomDestroyJoint: (const struct iNewtonUserJoint*) joint;
-(struct iNewtonJoint*) CustomGetNewtonJoint: (const struct iNewtonUserJoint*) joint;
-(int) CustomGetJointID: (const struct iNewtonUserJoint*) joint;
-(void) CustomSetJointID: (const struct iNewtonUserJoint*) joint: (int) rttI;
-(const struct iNewtonBody*) CustomGetBody0: (const struct iNewtonUserJoint*) joint;
-(const struct iNewtonBody*) CustomGetBody1: (const struct iNewtonUserJoint*) joint;
-(int) CustomGetBodiesCollisionState: (const struct iNewtonUserJoint*) joint;
-(void) CustomSetBodiesCollisionState: (const struct iNewtonUserJoint*) joint: (int) state;
-(void*) CustomGetUserData: (const struct iNewtonUserJoint*) joint;
-(void) CustomSetUserData: (const struct iNewtonUserJoint*) joint: (void*) userData;
-(void) CustomSetDestructorCallback: (const struct iNewtonUserJoint*) joint: (iNewtonUserJointDestructorCallback) callback;
-(void) CustomSetSubmitContraintCallback: (const struct iNewtonUserJoint*) joint: (iNewtonUserJointSubmitConstraintCallback) callback;
-(struct iNewtonUserJoint*) CustomCreateBlankJoint: (int) maxDof: (const struct iNewtonBody*) body0: (const struct iNewtonBody*) body1: (iBlankJointGetInfo) info;
-(struct iNewtonUserJoint*) CreateCustomKinematicController: (const struct iNewtonBody*) targetBody: (float*) attachmentPointInGlobalSpace;
-(void) CustomKinematicControllerSetPickMode: (const struct iNewtonUserJoint*) pick: (int) mode;
-(void) CustomKinematicControllerSetMaxLinearFriction: (const struct iNewtonUserJoint*) pick: (float) accel;
-(void) CustomKinematicControllerSetMaxAngularFriction: (const struct iNewtonUserJoint*) pick: (float) alpha;
-(void) CustomKinematicControllerSetTargetPosit: (const struct iNewtonUserJoint*) pick: (float*) posit;
-(void) CustomKinematicControllerSetTargetRotation: (const struct iNewtonUserJoint*) pick: (float*) rotation;
-(void) CustomKinematicControllerSetTargetMatrix: (const struct iNewtonUserJoint*) pick: (float*) matrix;
-(void) CustomKinematicControllerGetTargetMatrix: (const struct iNewtonUserJoint*) pick: (float*) matrix;
-(struct iNewtonUserJoint*) CreateCustomJoint6DOF: (const float*) pinsAndPivotChildFrame: (const float*) pinsAndPivotParentFrame: (const struct iNewtonBody*) child: (const struct iNewtonBody*) parent;
-(void) CustomJoint6DOF_SetLinearLimits: (struct iNewtonUserJoint*) customJoint6DOF: (const float*) minLinearLimits: (const float*) maxLinearLimits;
-(void) CustomJoint6DOF_SetAngularLimits: (struct iNewtonUserJoint*) customJoint6DOF: (const float*) minAngularLimits: (const float*) maxAngularLimits;
-(void) CustomJoint6DOF_GetLinearLimits: (struct iNewtonUserJoint*) customJoint6DOF: (float*) minLinearLimits: (float*) maxLinearLimits;
-(void) CustomJoint6DOF_GetAngularLimits: (struct iNewtonUserJoint*) customJoint6DOF: (float*) minAngularLimits: (float*) maxAngularLimits;
-(void) CustomJoint6DOF_SetReverseUniversal: (struct iNewtonUserJoint*) customJoint6DOF: (int) order;
-(struct iNewtonUserJoint*) CreateCustomBallAndSocket: (const float*) pinsAndPivotChildFrame: (const struct iNewtonBody*) child: (const struct iNewtonBody*) parent;
-(void) BallAndSocketSetConeAngle: (struct iNewtonUserJoint*) ballJoint: (float) angle;
-(void) BallAndSocketSetTwistAngle: (struct iNewtonUserJoint*) ballJoint: (float) minAngle: (float) maxAngle;
-(struct iNewtonUserJoint*) CreateCustomHinge: (const float*) pinsAndPivotChildFrame: (const struct iNewtonBody*) child: (const struct iNewtonBody*) parent;
-(void) HingeEnableLimits: (struct iNewtonUserJoint*) hingeJoint: (int) state;
-(void) HingeSetLimits: (struct iNewtonUserJoint*) hingeJoint: (float) minAngle: (float) maxAngle;
-(float) HingeGetJointAngle: (const struct iNewtonUserJoint*) hingeJoint;
-(void) HingeGetPinAxis: (const struct iNewtonUserJoint*) hingeJoint: (float*) pin;
-(float) HingeCalculateJointOmega: (const struct iNewtonUserJoint*) hingeJoint;
-(struct iNewtonUserJoint*) CreateCustomSlider: (const float*) pinsAndPivotChildFrame: (const struct iNewtonBody*) child: (const struct iNewtonBody*) parent;
-(void) SliderEnableLimits: (struct iNewtonUserJoint*) sliderJoint: (int) state;
-(void) SliderSetLimits: (struct iNewtonUserJoint*) sliderJoint: (float) mindist: (float) maxdist;
-(struct iNewtonUserJoint*) CreateCustomPlayerController: (const float*) pins: (const struct iNewtonBody*) player: (float) maxStairStepFactor: (float) cushion;
-(void) CustomPlayerControllerSetVelocity: (const struct iNewtonUserJoint*) playerController: (float) forwardSpeed: (float) sideSpeed: (float) heading;
-(void) CustomPlayerControllerGetVisualMaTrix: (const struct iNewtonUserJoint*) playerController: (float*) matrix;
-(float) CustomPlayerControllerGetMaxSlope: (const struct iNewtonUserJoint*) playerController;
-(void) CustomPlayerControllerSetMaxSlope: (const struct iNewtonUserJoint*) playerController: (float) maxSlopeAngleIndRadian;
-(const struct iNewtonCollision*) CustomPlayerControllerGetSensorShape: (const struct iNewtonUserJoint*) playerController;
-(struct iNewtonUserJoint*) DGRaycastVehicleCreate: (int) maxTireCount: (const float*) cordenateSytemInLocalSpace: (struct iNewtonBody*) carBody;
-(void) DGRaycastVehicleAddTire: (struct iNewtonUserJoint*) car: (void*) userData: (const float*) localPosition: (float) mass: (float) radius: (float) width: (float) friction: (float) suspensionLength: (float) springConst: (float) springDamper: (int) castMode;
-(void) DGRaycastVehicleSetTireTransformCallback: (struct iNewtonUserJoint*) car: (iDGRaycastVehicleTireTransformCallback) callback;
-(int) DGRaycastVehicleGetTiresCount: (struct iNewtonUserJoint*) car;
-(void*) DGRaycastVehicleGetTiresUserData: (struct iNewtonUserJoint*) car: (int) tireIndex;
-(void) DGRaycastVehicleGetTireMatrix: (struct iNewtonUserJoint*) car: (int) tireIndex: (float*) tireMatrix;
-(void) DGRaycastVehicleInitNormalizeTireLateralForce: (struct iNewtonUserJoint*) car: (int) pointsCount: (float* const) piceSizeStepAxis: (float* const) normalizedForceValue;
-(void) DGRaycastVehicleInitNormalizeTireLongitudinalForce: (struct iNewtonUserJoint*) car: (int) pointsCount: (float* const) piceSizeStepAxis: (float* const) normalizedForceValue;


@end
