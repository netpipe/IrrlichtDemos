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

//HACK:
extern ::irr::scene::ICameraSceneNode *		g_pPlayerCamera;

//
//	mxPortal - a Prey-like portal.
//
class mxPortal : public mxRenderEntity {
public:
			mxPortal( const Vec3D& position );
			~mxPortal();

			// overrides ( mxRenderEntity )
	void	RegisterForRendering();

			// overrides ( mxThinkingEntity )
	void	Think( const REAL fElapsedTime );

	void	Link( mxPortal* portal ) = 0;

	const mxView &	GetView() const;

			// For avoiding infinite recursion.
	u32		GetNumTimesRendered() const { return m_timesRendered; }

private:
	mutable mxView	m_view;

	::irr::scene::ICameraSceneNode *	m_pCamera;
};

#if 0
//
//	mxRectangularPortal
//
//	TODO: -> clip to another, a smaller frustum.
//	Use portal edges for culling.
//
class mxRectangularPortal : public mxPortal {
public:
	mxRectangularPortal( f32 width, f32 height, f32 thickness,
		const Vec3D& position = Vec3D( 0,0,0 ),
		const Vec3D& rotation = Vec3D( 0,0,0 ) );

	~mxRectangularPortal();
};

//
//	mxSphericalPortal
//
//	TODO: -> clip to cone frustum.
//
class mxSphericalPortal : public mxPortal {
public:
			mxSphericalPortal( f32 radius, const Vec3D& position = Vec3D( 0,0,0 ) );
			~mxSphericalPortal();

			// overrides ( mxRenderEntity )
	bool	IsVisible( const mxView& view );
	void	Render( const mxView& view );

private:
	f32		m_fRadius;

	::irr::scene::ISceneNode * m_pSceneNode;
};

#endif

//
//	mxPortalExitPoint
//
class mxPortalExitPoint : public mxPortal {
public:
	mxPortalExitPoint( const Vec3D& position );
	~mxPortalExitPoint();
};

//
//	mxSphericalOneWayPortal
//
class mxSphericalOneWayPortal : public mxPortal {
public:
			mxSphericalPortal( f32 radius, const Vec3D& center = Vec3D( 0,0,0 ) );
			~mxSphericalPortal();

			// overrides ( mxRenderEntity )
	bool	IsVisible( const mxView& view );
	void	Render( const mxView& view );

private:
	f32		m_fRadius;

	::irr::scene::ISceneNode * m_pSceneNode;
};

}//end of namespace mix

#endif /* ! __MX_PORTAL_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
