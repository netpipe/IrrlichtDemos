/*
===============================================================================
	File:	Weapon.cpp
	Desc:.
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

namespace mix {

/*================================
	class mxGameWeapon
================================*/

mxGameWeapon::mxGameWeapon()
	: m_owner( NULL )
	, m_iFireDelay( 350 )
{
//	m_eStatus = EStatus::Ready;

	m_iLastAttackTime = 0;

	//m_sounds[ OnEquipping ] = GSoundWorld->CacheSound( PATH_TO_MEDIA "Sounds/Weapons/Equip_Rifle.ogg" );
}

mxGameWeapon::~mxGameWeapon()
{
}

void mxGameWeapon::SetOwner( mxActor* newOwner )
{
	m_owner = newOwner;
}

mxActor* mxGameWeapon::GetOwner()
{
	return m_owner;
}

const WString & mxGameWeapon::GetName() const {
	return m_name;
}

void mxGameWeapon::Equip()
{
	//GSoundWorld->Play2D( m_sounds[ OnEquipping ] );
//	GSoundWorld->Play2D( PATH_TO_MEDIA "Sounds/Weapons/Equip_Rifle.ogg" );
}

bool mxGameWeapon::HasAnyAmmo() const
{
	return false;
}

void mxGameWeapon::PrimaryFire()
{
	Unreachable();
}

void mxGameWeapon::SecondaryFire()
{
	Unreachable();
}

void mxGameWeapon::AlternateFire( u32 mode )
{}

void mxGameWeapon::CeaseFire()
{

}

void mxGameWeapon::DryFire()
{}

void mxGameWeapon::EndAttack()
{
	// Derived classes can override CeaseFire().
	CeaseFire();

	//if ( ::irrklang::ISound * sound = m_sounds[ Single ] )
	//{
	//	sound->stop();
	//}
//	m_eStatus = EStatus::Ready;
}

void mxGameWeapon::Reload()
{}

void mxGameWeapon::BeginAttack()
{
	//if ( m_eStatus == Ready ) {
		m_iLastAttackTime = GWorld->GetTime();
	//}
//	m_eStatus = EStatus::Firing;
}

/*================================
	class Weapon_Automatic
================================*/

Weapon_Automatic::Weapon_Automatic()
	: m_fireRate( 1 )
{}

void Weapon_Automatic::Think( const REAL fElapsedTime )
{
	if ( 1 )
	{
		// See if it's time for the next shot
		u32 currTime = GWorld->GetTime();

		if ( currTime - m_iLastShotTime >= (1000 / m_fireRate) )
		{
		//	GSoundWorld->Play2D( m_sounds[ Single ] );
			m_iLastShotTime = currTime;

			// Do actual shooting.
			PrimaryFire();
		}
	}
}

//===========================================
namespace {

//
//	Weapon_Null
//
class Weapon_Null : public mxGameWeapon {
public:
				Weapon_Null()
				{}
				~Weapon_Null()
				{}

	// overrides ( mxGameWeapon )
	void	PrimaryFire() {}
	void	SecondaryFire() {}
};

static Weapon_Null *	nullWeapon = NULL;

}//end of anonymous namespace

mxGameWeapon * mxGameWeapon::GetNullWeapon()
{
	if ( ! nullWeapon ) {
		nullWeapon = new Weapon_Null();
		GWorld->Environment->AddEntity( nullWeapon );
	}
	return nullWeapon;
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
