/*
===============================================================================
	File:	CDestructionManager.cpp
	Desc:.
===============================================================================
*/

#include "../stdafx.h"
#pragma hdrstop
#include "../Include/IrrMix.h"


#define USE_OLD_BSP


#include "CDestructionManager.h"

#include "../CSG/BSP/BSP.h"
#include "../CSG/BSP_old/BSP.h"


namespace mix {

/*================================
	class CDestructionManager
================================*/

CDestructionManager::CDestructionManager()
{
#ifdef MX_DEBUG
	GCommon->DebugPrint( "sizeof( BspNode ) == %u\n", sizeof( BspNode ) );
	GCommon->DebugPrint( "sizeof( BspSolid ) == %u\n", sizeof( BspSolid ) );
#endif
}

CDestructionManager::~CDestructionManager()
{}

//============================================================================

#include "Solid.h"

//============================================================================

mxGeomoddableSolid* CDestructionManager::CreateSolid()
{
	return new CGeomoddableSolid_v0();
}
void CDestructionManager::DeleteSolid( mxGeomoddableSolid* pointer )
{
	delete pointer;
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
