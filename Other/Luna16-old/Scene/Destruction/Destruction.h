/*
===============================================================================
	File:	Destruction.h
	Desc:.
===============================================================================
*/

#ifndef __MX_DESTRUCTION__COMMON_SHARED_HEADER_H__
#define __MX_DESTRUCTION__COMMON_SHARED_HEADER_H__

#include "Common/Common.h"
#include "Common/Mesh.h"
#include "CSG/GeoMod.h"

namespace mix {

//
//	Defines
//

// Forward declarations.
class mxGeomoddableSolid;
class mxCrackPattern;
class mxRenderMesh;
class mxGeoModMesh;

//======================================================================

//
//	mxDestructionManager
//

class mxDestructionManager {
public:
	virtual ~mxDestructionManager() {}

	// CSG
	virtual mxGeomoddableSolid * CreateSolid() = 0;
	virtual void DeleteSolid( mxGeomoddableSolid* pointer ) = 0;
};

}//end of namespace mix

#endif /* ! __MX_DESTRUCTION__COMMON_SHARED_HEADER_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
