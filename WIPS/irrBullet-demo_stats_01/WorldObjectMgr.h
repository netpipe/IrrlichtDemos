// WorldObjectMgr.h: interface for the CWorldObjectMgr class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include "WorldObject.h"
#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;


class CWorldObjectMgr
{
private:
	std::vector<CWorldObject*>::iterator m_Current;
public:
	std::vector<CWorldObject*> m_WorldObjectArray;


public:
	CWorldObjectMgr();
	virtual ~CWorldObjectMgr();

	void Add(CWorldObject* pObj);

	//remove one object completely
	void Remove(CWorldObject* pObj);

	//remove all Objs
	void RemoveAll();

	CWorldObject* GetFirst();
	CWorldObject* GetNext();

};
extern CWorldObjectMgr* g_pWorldObjectMgr;
