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

extern ::irr::scene::ICameraSceneNode *		g_pPlayerCamera = NULL;

/*================================
	class mxPortal
================================*/

mxPortal::mxPortal( const Vec3D& position )
	: m_pBuddy( NULL )
	, m_pCamera( NULL )
{
	m_pCamera = GRenderWorld->SceneManager->addCameraSceneNode( NULL, position );
}

mxPortal::~mxPortal()
{
	m_pCamera->remove();
}

void mxPortal::RegisterForRendering()
{}

void mxPortal::Think( const REAL fElapsedTime )
{}

const mxView & mxPortal::GetView() const
{
	MemCopy( &(m_view.Frustum), m_pCamera->getViewFrustum(), sizeof( TViewFrustum ) );
	return m_view;
}


#if 0

/*================================
	class mxSphericalPortal
================================*/

mxSphericalPortal::mxSphericalPortal( f32 radius, const Vec3D& position /* = Vec3D( 0,0,0 ) */ )
	: mxPortal( position )
	, m_fRadius( radius )
	, m_pSceneNode( NULL )
{
	SetPosition( position );
	UpdateWorldTransform();

#if 1
	m_pSceneNode = GRenderWorld->SceneManager->addSphereSceneNode( radius, 16,
		NULL, -1,
		position
	);
	//m_pSceneNode->getMaterial( 0 ) == mxRenderWorld::DefaultSolidMaterial;
	m_pSceneNode->setMaterialFlag( ::irr::video::EMF_LIGHTING, false );
	//m_pSceneNode->setMaterialTexture( 0, GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Textures/Decals/hole_lg_mrk.jpg" ) );
	//m_pSceneNode->setVisible( false );
#else
	m_pSceneNode = GRenderWorld->SceneManager->addAnimatedMeshSceneNode(
		GRenderWorld->SceneManager->getMesh( PATH_TO_MEDIA "From Irrlicht SDK/earth.x" ),
		NULL, -1,
		position
	);
	m_pSceneNode->setMaterialFlag( ::irr::video::EMF_LIGHTING, false );
#endif
}

mxSphericalPortal::~mxSphericalPortal()
{
	if ( m_pSceneNode ) {
		m_pSceneNode->remove();
	}
}

bool mxSphericalPortal::IsVisible( const mxView& view )
{
	//return SphereIntersectsFrustum( m_pSceneNode->getPosition(), m_fRadius, view.Frustum )
	//return SphereIntersectsFrustum( GetPosition(), m_fRadius, view.Frustum )
	return (true)
		&& (m_timesRendered < m_maxNumRenderTimes)
		&& m_bShouldBeRendered;
}

void mxSphericalPortal::Render( const mxView& view )
{
	if ( ! IsVisible( view ) ) {
		return;
	}

	if ( m_pBuddy 
		// To avoid infinite recursion.
		&& m_pBuddy->GetNumTimesRendered() < 5 )
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
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILWRITEMASK,	0xffffffff );			// A write mask controls what is written.
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP );	// D3DSTENCILOP to do if stencil test passes and z-test fails.
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILFAIL, 		D3DSTENCILOP_KEEP );	// Specify when to write stencil data.
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILPASS, 		D3DSTENCILOP_REPLACE );	// D3DSTENCILOP to do if both stencil and z-tests pass.

		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,  FALSE );		// Disable writing to the depth buffer.


		// Render spherical mesh into the stencil buffer.
//		m_pSceneNode->setVisible( true );
		m_pSceneNode->render();
//		m_pSceneNode->setVisible( false );


		// HACK:
		++m_timesRendered;



		//
		// Render the cell through portal ( from the buddy's point of view ).
		//
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILFUNC,  D3DCMP_EQUAL );
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILPASS,  D3DSTENCILOP_KEEP );
		GRenderWorld->pD3DDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0L, 1.0f, 0L );		// Clear the depth buffer.


		this->m_bShouldBeRendered = false;
		m_pBuddy->m_bShouldBeRendered = false;


		m_pBuddy->m_pCamera->render();
		GRenderWorld->SceneManager->setActiveCamera( m_pBuddy->m_pCamera );
		GRenderWorld->DrawEntities( m_pBuddy->GetView() );


		//
		// Restore render states.
		//
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );	// Disable stencil testing.
		GRenderWorld->pD3DDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );


		g_pPlayerCamera->render();
		::mix::GRenderWorld->SceneManager->setActiveCamera( g_pPlayerCamera );
		GRenderWorld->DrawEntities( mxView() );



		this->m_bShouldBeRendered = true;
		m_pBuddy->m_bShouldBeRendered = true;
	}
}

#endif

/*================================
	class mxPortalExitPoint
================================*/

mxPortal::mxPortal( const Vec3D& position )
	: m_pBuddy( NULL )
	, m_pCamera( NULL )
{
	m_pCamera = GRenderWorld->SceneManager->addCameraSceneNode( NULL, position );
}

mxPortal::~mxPortal()
{
	m_pCamera->remove();
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
