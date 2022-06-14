/*
===============================================================================
	File:	Environment.cpp
	Desc:	
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

#include <Src/Destruction/Internal/CDestructionManager.h>

namespace mix {

/*================================
	class mxEnvironment
================================*/

mxEnvironment::mxEnvironment()
	: m_pDestructionManager( NULL )
{
}

mxEnvironment::~mxEnvironment()
{
}

void mxEnvironment::Spawn()
{
	m_pDestructionManager = new CDestructionManager();
}

void mxEnvironment::Shutdown()
{
	delete( m_pDestructionManager );
	m_pDestructionManager = NULL;

	Clear();
}

void mxEnvironment::AddEntity( mxEntity* entity )
{
	m_spawnedEntities.Append( entity );
}

void mxEnvironment::RemoveEntity( mxEntity* entity )
{
	GCommon->DebugPrint( "mxEnvironment::RemoveEntity() not implemented!\n" );
}

void mxEnvironment::Clear()
{
	// Delete all spawned entities.
	for ( u32 i = 0; i < m_spawnedEntities.Num(); ++i )
	{
		delete m_spawnedEntities[ i ];
	}
	m_spawnedEntities.Clear();
}

//TODO: check if some entities are out of world's bounds ( run these checks from time to time ).
void mxEnvironment::Tick( const REAL fElapsedTime )
{
	for ( u32 i = 0; i < m_spawnedEntities.Num(); ++i )
	{
		mxEntity * pEnt = m_spawnedEntities[ i ];

		pEnt->RunPhysics();
		pEnt->Think( fElapsedTime );
	}
}

mxDestructionManager& mxEnvironment::GetDestructionManager()
{	BREAK_IF( ! m_pDestructionManager );
	return *m_pDestructionManager;
}

bool mxEnvironment::Debug_TraceLine( const Vec3D& start, const Vec3D& end, Vec3D &OutResult, void* pUserPtr )
{
	return false;
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
