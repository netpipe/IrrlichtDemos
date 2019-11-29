/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/


#include "Phoenix.h"

using namespace phoenix;

class ResizeTest
{
    public:

        ResizeTest() : 
			system()
        {
        }

        virtual ~ResizeTest()
        {
        }

        int run()
        {

            //! Geometry to hold the tristrip
			BatchGeometryPtr lines = new BatchGeometry( system.getBatchRenderer(), GL_LINES );

			//! Add some stuff to it
			Vector2d origin( 0.0f, 0.0f );

			for( int i = 0; i < 100; ++i ){
				lines->addVertex( Vertex( origin + Vector2d( float(i) * 30.0f, 0 ), Color(255,255-(i*(255/100)),255,127) ) );
				lines->addVertex( Vertex( origin + Vector2d( float(i) * 30.0f, 1000 ), Color(255,255,255-(i*(255/100)),127 ) ) );

				lines->addVertex( Vertex( origin + Vector2d( 0, float(i) * 30.0f ), Color(255,255-(i*(255/100)),255,127) ) );
				lines->addVertex( Vertex( origin + Vector2d( 1000, float(i) * 30.0f ), Color(255,255,255-(i*(255/100)),127 ) ) );
			}

            //! Now just draw some stuff.
            while( system.run() )
            {

				//! Switch modes
				if( EventReceiver::Instance()->getKeyPressed(PHK_SPACE) ){
					system.setResizeBehavior( E_RESIZE_BEHAVIOR( ( system.getResizeBehavior() + 1 ) % 4 ) );
				}

                //! Draw some info.
                system.drawText( "Resize Test", Vector2d(16,16) );

				std::string resize_mode = "Nothing";
				switch( system.getResizeBehavior() ){
				case RZB_SCALE:
					resize_mode = "Scale"; break;
				case RZB_EXPAND:
					resize_mode = "Expand"; break;
				case RZB_REVERT:
					resize_mode = "Revert"; break;
				default: break;
				}


				system.drawText( "Mode " + resize_mode, Vector2d(16,32) );
            }

            return 0;

        }// Run

    protected:
        RenderSystem system;
    private:
};


