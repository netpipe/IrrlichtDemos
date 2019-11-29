/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/


#include "Phoenix.h"

using namespace phoenix;

class TristripTest
{
    public:

        TristripTest() : 
			system()
        {
        }

        virtual ~TristripTest()
        {
        }

        int run()
        {

            //! Geometry to hold the tristrip
			BatchGeometryPtr strip = new BatchGeometry( system.getBatchRenderer(), GL_TRIANGLE_STRIP );
			BatchGeometryPtr strip2 = new BatchGeometry( system.getBatchRenderer(), GL_TRIANGLE_STRIP );

			//! Add some stuff to it
			Vector2d origin( 50.0f, 100.0f );

			for( int i = 0; i < 100; ++i ){
				float drift = sin((float(i)/10.0f)*3.14159265f)*20.0f;
				strip->addVertex( Vertex( origin + Vector2d( float(i) * 5.0f, -drift ), Color(255,255-(i*(255/100)),255) ) );
				strip->addVertex( Vertex( origin + Vector2d( float(i) * 5.0f, 50.0f+drift ), Color(255,255,255-(i*(255/100)) ) ) );
			}

			origin += Vector2d( 0.0f, 150.0f );

			for( int i = 0; i < 15; ++i ){
				strip2->addVertex( Vertex( origin + Vector2d( float(i) * 25.0f, 0.0f ), Color(255-(i*(255/15)),255,255) ) );
				strip2->addVertex( Vertex( origin + Vector2d( float(i) * 25.0f, 50.0f ), Color(255,255-(i*(255/15)),255-(i*(255/15)) ) ) );
			}

            //! Now just draw some stuff.
            while( system.run() )
            {

                //! Draw some info.
                system.drawText( "Triangle Strip Test", Vector2d(16,16) );

            }

            return 0;

        }// Run

    protected:
        RenderSystem system;
    private:
};


