// WorldObjectMgr.cpp: implementation of the CWorldObjectMgr class.
//Manages world objects (add / remove etc.)

#include "WorldObjectMgr.h"
#include "utils.h"


CWorldObjectMgr::CWorldObjectMgr()
{
	m_WorldObjectArray.clear();
}

CWorldObjectMgr::~CWorldObjectMgr() {
	//problem here is the engine may have cleaned up before this destructor
	// is called, so can't touch the SceneNodes.
	//This destructor will only be called once on application close so no
	// need to remove() scenenodes anyway
	for (std::vector<CWorldObject*>::iterator iter = m_WorldObjectArray.begin(); iter != m_WorldObjectArray.end(); iter++) {
		delete *iter;
	}
}
//remove one object completely
void CWorldObjectMgr::Remove(CWorldObject* pObj) 
{
	for (std::vector<CWorldObject*>::iterator iter = m_WorldObjectArray.begin(); iter != m_WorldObjectArray.end(); iter++) {
		if (pObj ==  *iter) {
			pObj->Cleanup();
			delete pObj;
			m_WorldObjectArray.erase(iter);		
			break;
		}
	}
}

//remove all world objects
void CWorldObjectMgr::RemoveAll() 
{
	for (std::vector<CWorldObject*>::iterator iter = m_WorldObjectArray.begin(); iter != m_WorldObjectArray.end(); iter++) {
		(*iter)->Cleanup();
		delete *iter;
	}
	m_WorldObjectArray.clear();
}


void CWorldObjectMgr::Add(CWorldObject* pObj) 
{
	m_WorldObjectArray.push_back(pObj);
}

CWorldObject* CWorldObjectMgr::GetFirst() 
{
	m_Current = m_WorldObjectArray.begin();
	return (m_Current != m_WorldObjectArray.end()) ? *m_Current : 0;
}
CWorldObject*  CWorldObjectMgr::GetNext() 
{
	return (++m_Current != m_WorldObjectArray.end()) ? *m_Current : 0;
}

