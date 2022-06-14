/*
===============================================================================
	File:	Player.cpp
	Desc:	
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

namespace mix {

/*================================
	class mxGamePlayer
================================*/

mxGamePlayer::mxGamePlayer()
	: m_pController( NULL )

	, m_iSelectedWeaponIndex( 0 )
	, m_pCurrentWeapon( NULL )
	, m_pHUD( NULL )
{
}

mxGamePlayer::~mxGamePlayer()
{
	if ( m_pController ) {
		m_pController->drop();
	}

	delete m_pHUD;
}

void mxGamePlayer::Spawn()
{
	m_pController = new ::irr::scene::mxPlayerController( *this );
	m_pCamera->addAnimator( m_pController );

	// Create physics.
	m_physics = GPhysicsWorld->CreatePlayerPhysics( GetOrigin(), 10 );

	CreateHUD();
	CreateCrosshair();

	UpdateHUD();

	m_pCurrentWeapon = mxGameWeapon::GetNullWeapon();
}

void mxGamePlayer::SetOrigin( const Vec3D& newOrigin )
{
//	if ( m_pController ) {
		m_pController->Warp( newOrigin );
//	} else {
//		mxSpectator::SetOrigin( newOrigin );
//	}
}

void mxGamePlayer::RunPhysics()
{
//	if ( m_pController ) {
		m_pController->Update();
//	}
}

void mxGamePlayer::CreateHUD()
{
	m_pHUD = new mxPlayerHUD();
}

void mxGamePlayer::CreateCrosshair()
{
	//::irr::scene::ISceneNode * crosshair = GRenderWorld->SceneManager->addBillboardSceneNode(
	//	m_pCamera,
	//	irr::core::dimension2df( 0.1f, 0.1f ), Vec3D ( 0.0f, 0.0f, 1.0f ) );

	//crosshair->setMaterialType( irr::video::EMT_TRANSPARENT_ADD_COLOR );
	//crosshair->setMaterialTexture( 0, GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/crosshair.bmp" ) );
	//crosshair->setMaterialFlag( irr::video::EMF_LIGHTING, false );
	//crosshair->setMaterialFlag( irr::video::EMF_ZBUFFER, false );
}

void mxGamePlayer::SelectNextWeapon()
{
	GetCurrentWeapon().EndAttack();

	++m_iSelectedWeaponIndex;
	if ( m_iSelectedWeaponIndex >= m_weapons.size() )
	{
		m_iSelectedWeaponIndex = 0;
	}
	m_pCurrentWeapon = m_weapons[ m_iSelectedWeaponIndex ];
	m_pCurrentWeapon->Equip();

	UpdateHUD();
}

void mxGamePlayer::SelectPrevWeapon()
{
	GetCurrentWeapon().EndAttack();

	if ( m_iSelectedWeaponIndex == 0 )
	{
		m_iSelectedWeaponIndex = m_weapons.size()-1;
	}
	else
	{
		--m_iSelectedWeaponIndex;
	}
	m_pCurrentWeapon = m_weapons[ m_iSelectedWeaponIndex ];
	m_pCurrentWeapon->Equip();

	UpdateHUD();
}

void mxGamePlayer::GiveWeapon( mxGameWeapon* newWeapon )
{
	Assert( newWeapon );
	newWeapon->SetOwner( this );
	m_weapons.push_back( newWeapon );

	m_pCurrentWeapon = newWeapon;
}

mxGameWeapon & mxGamePlayer::GetCurrentWeapon() {
	return *m_pCurrentWeapon;
}

void mxGamePlayer::UpdateHUD()
{
	if ( m_weapons.size() == 0 ) {
		return;
	}
	m_pHUD->SetWeaponText( m_weapons[ m_iSelectedWeaponIndex ]->GetName().c_str() );
}

/*================================
	class mxGamePlayer
================================*/

mxPlayerHUD::mxPlayerHUD()
	: m_weaponText( NULL )
{
	// Weapon stuff.
	m_weaponText = GRenderWorld->GUI->addStaticText(
		L"", ::irr::core::rect< s32 >( 450,  20,  620,  70 ),
		false,	// border
		true,	// wordWrap
		0, -1, true );

	m_weaponText->setTextAlignment( ::irr::gui::EGUIA_UPPERLEFT, irr::gui::EGUIA_CENTER );
	m_weaponText->setOverrideColor( ::irr::video::SColor(255, 0, 255, 255) );
}

mxPlayerHUD::~mxPlayerHUD()
{
}

void mxPlayerHUD::SetWeaponText( const wchar_t* text )
{
	m_weaponText->setText( text );
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
