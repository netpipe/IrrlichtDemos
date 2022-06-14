/*
===============================================================================
	File:	Portal.h
	Desc:
===============================================================================
*/

#ifndef __MX_PORTAL_H__
#define __MX_PORTAL_H__

namespace mix {

// Forward declarations.
class mxEntity;
class mxEnvironment;

//
//	mxPortal - a Prey-like portal.
//
class mxPortal : public mxRenderEntity
{
	friend class mxRenderWorld;

	TCircularList< mxRenderEntity >		PortalListNode;	// For being linked into the portals list.

public:
			mxPortal( const Vec3D& position );
			~mxPortal();

			// overrides ( mxRenderEntity )
	void	RegisterForRendering();

			// overrides ( mxThinkingEntity )
	void	Think( const REAL fElapsedTime );

					// Link this portal to another portal.
	virtual void	Link( mxPortal* portal ) {}

	const mxView &	GetView() const;

	::irr::scene::ICameraSceneNode *	GetCamera();

protected:
	mutable mxView		m_view;

private:
	::irr::scene::ICameraSceneNode *	m_pCamera;
};

inline ::irr::scene::ICameraSceneNode * mxPortal::GetCamera() {
	return m_pCamera;
}

//
//	mxPortalExitPoint
//
//	Not a real portal, it just serves as an exit point of another portal.
//
class mxPortalExitPoint : public mxPortal {
public:
	mxPortalExitPoint( const Vec3D& position );
	~mxPortalExitPoint();

			// overrides ( mxRenderEntity )
	void	Render( const mxView& view );

			// overrides ( mxPortal )
	void	Link( mxPortal* portal );
};

//
//	mxSphericalOneWayPortal
// UNDONE:
//
class mxSphericalOneWayPortal : public mxPortal {
public:
			mxSphericalOneWayPortal( f32 radius, const Vec3D& center = Vec3D( 0,0,0 ) );
			~mxSphericalOneWayPortal();

			// overrides ( mxRenderEntity )
	void	RegisterForRendering();
	bool	IsVisible( const mxView& view );
	void	Render( const mxView& view );

			// overrides ( mxPortal )
			// Set the exit portal for this portal.
	void	Link( mxPortal* other );

private:
	f32		m_fRadius;

	mxPortal *	m_pExitPortal;

	::irr::scene::ISceneNode * m_pMesh;	// Mesh for use with stencil buffer.

	bool	m_bShouldBeRendered;	// For avoiding recursion.
};

//
//	mxRectangularPortal_RTT
//
class mxRectangularPortal_RTT : public mxPortal {
public:
			mxRectangularPortal_RTT( const Vec3D& dimensions,
				const Vec3D& position = Vec3D( 0,0,0 ), const Vec3D& rotation = Vec3D( 0,0,0 ) );

			~mxRectangularPortal_RTT();

			// overrides ( mxRenderEntity )
	void	RegisterForRendering();
	bool	IsVisible( const mxView& view );
	void	Render( const mxView& view );

			// overrides ( mxPortal )
			// Set the exit portal for this portal.
	void	Link( mxPortal* other );

private:
	Vec3D		m_dimensions;
	mxPortal *	m_pBuddy;

	// Debugging.
	::irr::scene::ISceneNode *	m_pCube;

	bool	m_bShouldBeRendered;

	// RTT specific.
	::irr::video::ITexture *	m_pRenderTarget;
};

}//end of namespace mix

#endif /* ! __MX_PORTAL_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
