//class to manage collision object data, so it can be shared
#include "CollisionObjectMgr.h"


CCollisionObjectMgr::~CCollisionObjectMgr(void)
{
	m_CollisionObjectArray.clear();
}


CCollisionObject* CCollisionObjectMgr::Get(void* pIrrMesh)
{
	for (std::vector<CCollisionObject*>::iterator iter = m_CollisionObjectArray.begin(); iter != m_CollisionObjectArray.end(); iter++)
	{
		CCollisionObject* pCurrentObj = *iter;
		//since irrlicht reuses meshes, the irr::scene::IAnimatedMesh pointer
		// will be the same for similar meshes
		//it would be betterto use Irrlicht's internal ref. counting,
		// but this is not exposed through the irr::IUnknown interface :(
		//so using an external ref. counting system instead
		if (pCurrentObj->m_pIrrMesh == pIrrMesh) {
			pCurrentObj->AddRef();
			return pCurrentObj;
		}
	}
	return 0;
}

CCollisionObject* CCollisionObjectMgr::Add(btCollisionShape* pShape, btTriangleMesh* pBtMesh, void* pIrrMesh)
{
	CCollisionObject* pNewObj = new CCollisionObject(pShape, pBtMesh, pIrrMesh);
	m_CollisionObjectArray.push_back(pNewObj);
	return pNewObj;
}

void CCollisionObjectMgr::Remove(CCollisionObject* pColObj)
{
	for (std::vector<CCollisionObject*>::iterator iter = m_CollisionObjectArray.begin(); iter != m_CollisionObjectArray.end(); iter++) {
		CCollisionObject* pCurrentObj = *iter;
		if (pCurrentObj==pColObj) {
			if (pColObj->Release()==0) {
				m_CollisionObjectArray.erase(iter);
				break;
			}
		}
	}
}
