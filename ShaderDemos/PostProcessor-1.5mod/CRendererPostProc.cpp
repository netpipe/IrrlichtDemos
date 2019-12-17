#include "CRendererPostProc.h"

CRendererPostProc::CRendererPostProc( ISceneManager* sm, dimension2di sOut, bool clearBackBuffer, bool clearZBuffer, SColor color ) : IPostProc( sm, sOut ) {
	cbb = clearBackBuffer;
	czb = clearZBuffer;
	c = color;
}
void CRendererPostProc::renderEffect( ITexture* target ) {
	IVideoDriver *driver = smgr->getVideoDriver( );

	if( enabled ) {
		if( supported ) {
			driver->setRenderTarget( target, false, false, c );
			driver->beginScene( cbb, czb, c );
		} else {
			if( !cbb )
				driver->beginScene( false, czb, c );
			else {
				if( driver->getViewPort( ) == rect<int>( position2di( 0, 0 ), driver->getCurrentRenderTargetSize( ) ) )
					driver->beginScene( true, czb, c );
				else {
					driver->draw2DRectangle( c, rect<int>( position2di( 0, 0 ), driver->getCurrentRenderTargetSize( ) ) );
					driver->beginScene( false, czb, c );
				}
			}
		}
		smgr->drawAll( );
		if( target != NULL )
			driver->endScene( );
	}
}
