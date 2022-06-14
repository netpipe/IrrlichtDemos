/*
===============================================================================
	File:	Player.h
	Desc:	
===============================================================================
*/

#ifndef __MX_GAME_PLAYER_H__
#define __MX_GAME_PLAYER_H__


// Forward declarations.
namespace irr {
namespace scene {
	class mxPlayerController;
}//end of namespace scene
}//end of namespace irr


namespace mix {

// Forward declarations.
class mxPlayerHUD;
class mxGameWeapon;

//
//	mxGamePlayer
//
class mxGamePlayer : public mxSpectator {
public:
			mxGamePlayer();
			~mxGamePlayer();

	// overrides ( mxSpectator )
	void	Spawn();

	void	RunPhysics();

	// overrides ( mxActor )
	mxGamePlayer *	IsGamePlayer() { return this; }

	// overrides ( Specator )
	void	SetOrigin( const Vec3D& newOrigin );

	::irr::scene::mxPlayerController *	GetController()		{ return m_pController; }

	// Returns the radius of a sphere enclosing the player.
	static f32		GetPlayerRadius() { return 1.75f; }
	static f32		GetPlayerMass() { return 90.0f; }

public:
	//--- Weapons ----------------------------------------
	mxGameWeapon &	GetCurrentWeapon();

	void	SelectWeapon( u32 weaponId );
	void	SelectNextWeapon();
	void	SelectPrevWeapon();

	// The player accepts responsibility for deleting the weapon.
	void	GiveWeapon( mxGameWeapon* newWeapon );

private:
	// Initialization
	void	CreateHUD();
	void	CreateCrosshair();

private:
	void	UpdateHUD();

private:
	::irr::scene::mxPlayerController *	m_pController;

	::irr::core::array< mxGameWeapon* >	m_weapons;
	u32		m_iSelectedWeaponIndex;	// for weapon selection

	mxGameWeapon *		m_pCurrentWeapon;

	mxPlayerHUD *		m_pHUD;
};

//
//	mxPlayerHUD
//
class mxPlayerHUD : public mxClass {
public:
			mxPlayerHUD();
			~mxPlayerHUD();

	void	SetWeaponText( const wchar_t* text );

private:
	::irr::gui::IGUIStaticText *	m_weaponText;
};

}//end of namespace mix

#endif /* ! __MX_GAME_PLAYER_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//

