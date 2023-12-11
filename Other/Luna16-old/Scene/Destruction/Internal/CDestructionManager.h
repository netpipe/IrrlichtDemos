/*
===============================================================================
	File:	DestructionManager.h
	Desc:.
===============================================================================
*/

#ifndef __MX_DESTRUCTION_MANAGER_H__
#define __MX_DESTRUCTION_MANAGER_H__

namespace mix {

// Forward declarations;
class mxGeomoddableSolid;

//
//	CDestructionManager
//
class CDestructionManager : public mxDestructionManager {
public:
			CDestructionManager();
			~CDestructionManager();

	mxGeomoddableSolid *	CreateSolid();
	void					DeleteSolid( mxGeomoddableSolid* pointer );
};

}//end of namespace mix

#endif /* ! __MX_DESTRUCTION_MANAGER_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
