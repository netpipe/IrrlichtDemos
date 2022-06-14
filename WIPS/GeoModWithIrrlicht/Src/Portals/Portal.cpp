/*
===============================================================================
	File:	Portal.cpp
	Desc:	
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

namespace mix {

/*================================
	class mxPortal
================================*/

mxPortal::mxPortal( const Vec3D& position )
	: m_pCamera( NULL )
{
	mxRenderEntity::SetPosition( position );

	m_pCamera = GRenderWorld->SceneManager->
		addCameraSceneNode( NULL, position );
		//addCameraSceneNodeFPS();
	//m_pCamera->setPosition( position );

	GRenderWorld->AddPortal( this );
}

mxPortal::~mxPortal()
{
	m_pCamera->remove();

	GRenderWorld->RemovePortal( this );
}

void mxPortal::RegisterForRendering()
{}

void mxPortal::Think( const REAL fElapsedTime )
{
//	m_pCamera->setRotation( g_pPlayerCamera->getRotation() );
}

const mxView & mxPortal::GetView() const
{
	MemCopy( &(m_view.Frustum), m_pCamera->getViewFrustum(), sizeof( TViewFrustum ) );
	return m_view;
}

/*================================
	class mxPortalExitPoint
================================*/

mxPortalExitPoint::mxPortalExitPoint( const Vec3D& position )
	: mxPortal( position )
{
}

mxPortalExitPoint::~mxPortalExitPoint()
{
}

void mxPortalExitPoint::Link( mxPortal* portal )
{
	GCommon->DebugPrint( "Warning: attempt to link portal exit point to another portal!" );
}

void mxPortalExitPoint::Render( const mxView& view )
{
}

/*================================
	class mxSphericalOneWayPortal
================================*/

mxSphericalOneWayPortal::mxSphericalOneWayPortal( f32 radius, const Vec3D& center /* = Vec3D( 0,0,0 ) */ )
	: mxPortal( center )
	, m_fRadius( radius )
	, m_pExitPortal( NULL )
	, m_pMesh( NULL )
	, m_bShouldBeRendered( true )
{
	m_pMesh = GRenderWorld->SceneManager->addSphereSceneNode( m_fRadius, 16 );
	m_pMesh->setMaterialFlag( ::irr::video::EMF_LIGHTING, false );
	m_pMesh->setVisible( false );
}

mxSphericalOneWayPortal::~mxSphericalOneWayPortal() {
	m_pMesh->remove();
}

void mxSphericalOneWayPortal::Link( mxPortal* other ) {
	BREAK_IF( ! other );
	m_pExitPortal = other;
}

bool mxSphericalOneWayPortal::IsVisible( const mxView& view ) {
//	return SphereIntersectsFrustum( GetPosition(), m_fRadius, view.Frustum )
	return 1
		&& m_bShouldBeRendered;
}

void mxSphericalOneWayPortal::RegisterForRendering()
{}

void mxSphericalOneWayPortal::Render( const mxView& view )
{
	if ( m_pExitPortal )
	{
		// Before rendering the cell through the portal,
		// mask the portal using the stencil buffer to ensure
		// that only pixels that lie on the portal can be rendered.

		//
		// Setup stencil buffer.
		//
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILENABLE,		TRUE );					// Enable stencil testing.
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILFUNC,     	D3DCMP_ALWAYS );		// Specify the stencil comparison function. (Always pass the test.)
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILREF,      	0x1 );					// Set the comparison reference value.
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILMASK,     	0xffffffff );			// Specify a stencil mask.
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILWRITEMASK,	0xffffffff );			// A write mask controls what is written. (Default value is 0xffffffff.)
		//GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP );	// D3DSTENCILOP to do if stencil test passes and z-test fails. (The default value is D3DSTENCILOP_KEEP.)
		//GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILFAIL, 		D3DSTENCILOP_KEEP );	// Specify when to write stencil data. (The default value is D3DSTENCILOP_KEEP.)
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILPASS, 		D3DSTENCILOP_REPLACE );	// D3DSTENCILOP to do if both stencil and z-tests pass. (D3DSTENCILOP_REPLACE - Replace the stencil-buffer entry with a reference value.)

		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,  FALSE );		// Disable writing to the depth buffer.


		// Render the mesh into the stencil buffer.
		m_pMesh->setVisible( true );
		m_pMesh->render();

		//
		// Render the cell through portal ( from the buddy's point of view ).
		//
		GRenderWorld->SceneManager->setActiveCamera(
			m_pExitPortal->GetCamera()
		);
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );				// Enable writing to the depth buffer.
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILFUNC,  D3DCMP_EQUAL );		// D3DCMP_EQUAL - accept the new pixel if its value equals the value of the current pixel.
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILPASS,  D3DSTENCILOP_KEEP );	// D3DSTENCILOP_KEEP - do not update the entry in the stencil buffer.
		GRenderWorld->pD3DDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0L, 1.0f, 0L );		// Clear the depth buffer.


		//m_bShouldBeRendered = false;

		// Render the zone connected to this portal.
		GRenderWorld->SceneManager->drawAll();
		//GRenderWorld->DrawEntities( GetView() );

		//m_bShouldBeRendered = true;

		//
		// Restore render states.
		//
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );	// Disable stencil testing.
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );	// Disable user-defined clipping planes.

		//GRenderWorld->SceneManager->setActiveCamera( g_pPlayerCamera );
	}
	else
	{
		m_pMesh->setVisible( true );
		m_pMesh->render();
	}
}

/*================================
	class mxRectangularPortal_RTT
================================*/

mxRectangularPortal_RTT::mxRectangularPortal_RTT(  const Vec3D& dimensions,
				const Vec3D& position /* = Vec3D( 0,0,0 ) */,
				const Vec3D& rotation /* = Vec3D( 0,0,0 ) */ )

	: mxPortal( position )
	, m_pBuddy( NULL )

	, m_pRenderTarget( NULL )
{
	m_pRenderTarget = GRenderWorld->Driver->addRenderTargetTexture(
		::irr::core::dimension2d< s32 >( 512, 512 ), "RTT1"
	);
	Assert( m_pRenderTarget );

//*******
	m_pCube = GRenderWorld->SceneManager->addCubeSceneNode( dimensions.X, NULL, -1, position );
	m_pCube->setMaterialTexture( 0, m_pRenderTarget ); // set material of cube to render target

	m_bShouldBeRendered = true;
}

mxRectangularPortal_RTT::~mxRectangularPortal_RTT() {
	m_pCube->remove();
}

void mxRectangularPortal_RTT::Link( mxPortal* other )
{	BREAK_IF( ! other );
	m_pBuddy = other;
}

bool mxRectangularPortal_RTT::IsVisible( const mxView& view ) {
	return m_bShouldBeRendered;
}

void mxRectangularPortal_RTT::RegisterForRendering()
{}

void mxRectangularPortal_RTT::Render( const mxView& view )
{
	if ( m_pBuddy )
	{
		::irr::scene::ICameraSceneNode * fpsCamera = GRenderWorld->SceneManager->getActiveCamera();

		// draw scene into render target

		// set render target texture
		GRenderWorld->Driver->setRenderTarget( m_pRenderTarget, true, true, ::irr::video::SColor(0,0,0,255) );

		// make cube invisible and set fixed camera as active camera
		m_pCube->setVisible( false );
		GRenderWorld->SceneManager->setActiveCamera( m_pBuddy->GetCamera() );

		// draw whole scene into render buffer
		m_bShouldBeRendered = false;
		GRenderWorld->DrawEntities( m_pBuddy->GetView() );
		m_bShouldBeRendered = true;

		// set back old render target
		// The buffer might have been distorted, so clear it
		GRenderWorld->Driver->setRenderTarget( NULL, true, true, 0 );

		// make the cube visible and set the user controlled camera as active one
		m_pCube->setVisible( true );
		GRenderWorld->SceneManager->setActiveCamera( fpsCamera );
	}
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
