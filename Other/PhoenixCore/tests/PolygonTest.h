/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "Phoenix.h"

using namespace phoenix;
using namespace std;

class PolygonTest
{
    public:

        PolygonTest() 
            : system( Vector2d(640,480), false, false )
        {
        }

        virtual ~PolygonTest()
        {
        }

        /*!
            This actually runs this test. It verifies:
            *) That polygons can be constructed
            *) That rectangles can be constructed.
            *) That polygons can be scaled.
            *) That polygons can be rotated.
            *) That polygons can be compared.
        */
        int run()
        {

            //! Let's make some polys.

            //! Poly 1 - Triangle. Basic constructor, addVertex
            phoenix::Polygon poly1;
            poly1.setPosition( Vector2d(50,150) );
            poly1.addVertex( Vector2d( 0,-30 ) );
            poly1.addVertex( Vector2d( 30,20 ) );
            poly1.addVertex( Vector2d( -30, 20 ) );

            //! Poly 2 - Square, Constructed from Rectangle.
            phoenix::Polygon poly2( phoenix::Rectangle(180,150,150,150) );

            //! Poly 3 - Rectangleangle, Basic constructor, addPoint
            phoenix::Polygon poly3( Vector2d(0,0) );
            poly3.addPoint( Vector2d(60,-30) );
            poly3.addPoint( Vector2d(-60,-30) );
            poly3.addPoint( Vector2d(-60,30) );
            poly3.addPoint( Vector2d(60,30) );

            //! Poly 4 is poly 1 scaled.
            phoenix::Polygon poly4( poly1 * 2.0f );
            poly4.setPosition( Vector2d( 350,250 ) );


            //! Some sanity comparisons
            phoenix::Polygon Poly1Copy = poly1;
            cout<<"Poly 1 Copy == Poly1 ? "<< ( poly1 == Poly1Copy? "True" : "False" )<< endl;
            cout<<"Poly 1 Copy * 10.0f == Poly1 * 10.0f ? "<< ( (poly1*10.0f) == (Poly1Copy*10.f)? "True" : "False" )<< endl;
            cout<<"Poly 1 == Poly2 ? "<< ( poly1 == poly2? "True" : "False" )<< endl;

            //! Sort test.
            poly1.sort();
            poly2.sort();
            poly3.sort();
            poly4.sort();

			//! timer.
			Timer deltatime;

            //! texture.
            TexturePtr feathertexture = system.loadTexture( "feather.png" );

            //! Now just draw some stuff.
            while( system.run() )
            {

                //! Rotate poly 1 with a scalar
                poly1.rotate( DegreesToRadians( 60.0f * (float)deltatime.getTime() ) );

                //! Rotate poly 2 with a rotation matrix
                poly2 *= RotationMatrix( DegreesToRadians( -80.f * (float)deltatime.getTime() ) );

                //! Move poly 3 to the mouse's position
                poly3.setPosition( EventReceiver::Instance()->getMousePosition() );

                //!Rotate 3 and four with mouse buttons.
                if( EventReceiver::Instance()->getMouseButton(PHK_MB_LEFT))
                {
                    poly3.rotate( DegreesToRadians(80.0f * (float)deltatime.getTime()) );
                    poly4.rotate( DegreesToRadians(-100.0f * (float)deltatime.getTime()) );
                }
                if( EventReceiver::Instance()->getMouseButton(PHK_MB_RIGHT))
                {
                    poly3.rotate( DegreesToRadians(-80.0f * (float)deltatime.getTime()) );
                    poly4.rotate( DegreesToRadians(100.0f * (float)deltatime.getTime()) );
                }

                //! if the spacebar is pressed, draw some bounding boxes.
                if( EventReceiver::Instance()->getKey( PHK_SPACE ) )
                {
                    system.setDepth( 6.0f );
                    system.drawRectangle( poly1 , Color( 255,255,255,127 ) );
                    system.drawRectangle( poly2 , Color( 0,255,255,127 ) );
                    system.drawRectangle( poly3 , Color( 255,0,255,127 ) );
                    system.drawRectangle( poly4 , Color( 255,255,0,127 ) );
                }

                //! Draw our polygons
                system.setDepth( 1.0f );
                system.drawPolygon( poly1, Color(255,200,200) );
                system.setDepth( 0.0f );

                phoenix::BatchGeometryPtr g = system.drawPolygon( poly2, Color(200,255,200) );
				g->setClipping(true);
				g->setClippingRectangle( phoenix::Rectangle(200,200,EventReceiver::Instance()->getMousePosition().getX()-200,EventReceiver::Instance()->getMousePosition().getY()-200) );


                system.setDepth( -6.0f );
                system.drawPolygon( poly4, Color(0,200,255) );
                system.setDepth( -3.0f );
                system.drawTexturedPolygon( poly3, feathertexture, Color(200,200,255,127), EventReceiver::Instance()->getKey( PHK_E ) ? true : false ) ;

				//! Draw a rectangle and a polygon to test the cross-constructors
				phoenix::Rectangle r( Vector2d(15,400), Vector2d(64,32) );
				system.drawRectangle( r )->colorize( Color(255,0,0,100) );
				system.drawPolygon( r, Color(0,255,0,100) );
				system.drawRectangle( phoenix::Polygon(r) )->colorize( Color(0,0,255,100) );

                //! Draw some info.
                system.drawText( "Polygon Test", Vector2d(16,16) );
                system.drawText( "Press Space to Show Bounding Boxes", Vector2d(16,32), Color(255,127,127) );

				//! Reset timer.
				deltatime.reset();

            }

            return 0;

        }// Run

    protected:
        RenderSystem system;
    private:
};

