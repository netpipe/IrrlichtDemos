/*
===============================================================================
	File:	Environment.h
	Desc:	
===============================================================================
*/

#ifndef __MX_ENVIRONMENT_H__
#define __MX_ENVIRONMENT_H__

namespace mix {

// Forward declarations.
class mxEntity;
class mxDestructionManager;

//
//	mxEnvironment - represents a physics (collision) environment.
//
//	There can be multiple independent environments.
//	Environments can be connected through portals.
//
class mxEnvironment {
public:
			mxEnvironment();
			~mxEnvironment();

					// Initialize the level.
	virtual void	Spawn();

					// Free alloced resources, clean up, etc.
	virtual void	Shutdown();

					// NOTE: Accepts responsibility for deleting the given entity.
	void			AddEntity( mxEntity* entity );
	void			RemoveEntity( mxEntity* entity );	

					// Advance by the given time.
	virtual void	Tick( const REAL fElapsedTime );

	//--- Physics ---------------------------------------------------------------------------

	//--- Destruction -----------------------------------------------------------------------
	mxDestructionManager &	GetDestructionManager();

					// This function is called when an object goes beyond the scope of this environment.
	virtual void	ExceededBounds( mxEntity* entity ) {}


	//---------------------------------------------------------------------------------------
	// Testing & Debugging.

					// Should return 'true' if hit anything.
	virtual bool	Debug_TraceLine( const Vec3D& start, const Vec3D& end, Vec3D &OutResult, void* pUserPtr );

private:
	TArray< mxEntity* >				m_spawnedEntities;

	mxDestructionManager *			m_pDestructionManager;

private:
	// Delete all spawned entities on this level.
	void	Clear();
};

}//end of namespace mix

#endif /* ! __MX_ENVIRONMENT_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
