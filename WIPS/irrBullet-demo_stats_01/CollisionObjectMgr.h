#pragma once
#include "btBulletDynamicsCommon.h"

#include "utils.h"

#include <vector>

class CCollisionObject;

//since bullet collision shapes can be shared between similar geometry objects,
// the btCollisionShapes are stored in an array, as members of a CCollisionObject class
//This class uses reference counting to automatically delete the objects when no 
// longer referenced
class CCollisionObjectMgr
{
private:
	std::vector<CCollisionObject*> m_CollisionObjectArray;	

public:
	
	CCollisionObjectMgr(void) {};
	~CCollisionObjectMgr(void);
	CCollisionObject* Get(void* pIrrMesh);
	CCollisionObject* Add(btCollisionShape* pShape, btTriangleMesh* pBtMesh, void* pIrrMesh);
	void Remove(CCollisionObject* pColObj);
};

extern CCollisionObjectMgr* g_pCollisionObjectMgr;

class CCollisionObject
{
private:
	int m_RefCount;

public:
	void* m_pIrrMesh;
	btTriangleMesh* m_pTriangleMesh; //this is not used by all objects
	btCollisionShape* m_pShape;

	CCollisionObject(btCollisionShape* pShape, btTriangleMesh* pBtMesh, void* pIrrMesh)
		: m_pShape(pShape), m_pTriangleMesh(pBtMesh), m_pIrrMesh(pIrrMesh), m_RefCount(1) {		
	};
	~CCollisionObject() {
		SAFE_DELETE(m_pTriangleMesh);
		SAFE_DELETE(m_pShape);
	};
	int AddRef() { 
		return m_RefCount++; //return old value
	};
	int Release() { 
		if (--m_RefCount <= 0) {
			delete this;
			return 0;
		}
		return m_RefCount; //return new value
	};
};
