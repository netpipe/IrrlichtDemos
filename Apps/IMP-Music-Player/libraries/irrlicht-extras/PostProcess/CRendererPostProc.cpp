#include <irrlicht.h>
#include "CRendererPostProc.h"

CRendererPostProc::CRendererPostProc( irr::scene::ISceneManager* sm, DIMENSION2D sOut, bool clearBackBuffer, bool clearZBuffer, irr::video::SColor color ) : IPostProc( sm, sOut ) {
	cbb = clearBackBuffer;
	czb = clearZBuffer;
	setColor( color );
}
void CRendererPostProc::renderEffect( irr::video::ITexture* target ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	
	if( enabled ) {
		if( supported )
			driver->setRenderTarget( target, cbb, czb, c );
		else {
			if( cbb ) {
				if( driver->getViewPort( ).UpperLeftCorner.X <= 0 && driver->getViewPort( ).UpperLeftCorner.Y <= 0 && driver->getViewPort( ).LowerRightCorner.X >= (irr::s32) driver->getCurrentRenderTargetSize( ).Width && driver->getViewPort( ).LowerRightCorner.Y >= (irr::s32) driver->getCurrentRenderTargetSize( ).Height )
					driver->beginScene( true, false, c );
				else
					driver->draw2DRectangle( c, irr::core::rect<irr::s32>( irr::core::position2d<irr::s32>( 0, 0 ), (irr::core::dimension2d<irr::s32>) driver->getCurrentRenderTargetSize( ) ) );
			}
			if( czb )
				driver->clearZBuffer( );
		}
		smgr->drawAll( );
	}
}
