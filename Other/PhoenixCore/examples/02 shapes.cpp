/*! Shapes example.
    This example shows you how to draw basic shapes with phoenix.
*/

#include "Phoenix.h"

using namespace phoenix;

int main()
{

    RenderSystem system;

    //! The Loop.
    while( system.run() )
    {

        /*!
            Let's start with rays and lines. Rays are exactly as they are geometry, a vector that extends from a point.
            Lines (line segments) are vectors that extend from one point to another.
        */
        system.drawRay( Vector2d( 320, 240 ), Vector2d( 64, 64 ), Color(255,0,0) );

        system.drawLine( Vector2d( 100,100 ), Vector2d( 540, 100 ), Color(0,255,0 ) );

        /*!
            Let's try a rectangle. Rectangles are fairly easy, you make one using
            Rectangle( position, size ) where position and size are Vector2ds. So we'd
            like to draw a small rectangle at the top left of our screen.
        */
        system.drawRectangle( Rectangle( Vector2d( 32,32 ), Vector2d( 64, 32 ) ) );

        /*!
            Now let's try a polygon. Polygons are a collection of vertices that extend
            from the centroid of the polygon.
        */
        Polygon poly( Vector2d( 200,200 ) ); // 200,200 is the centroid of the polygon.

        /*!
            Now let's add some vertices to make a simple triangle.
        */
        poly.addVertex( Vector2d( 0, -10 ) );
        poly.addVertex( Vector2d( 10, 0 ) );
        poly.addVertex( Vector2d( -10, 0 ) );

        /*!
            Finally, the call to draw it.
        */
        system.drawPolygon( poly );

    }

    return 0;

}
