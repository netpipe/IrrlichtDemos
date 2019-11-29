/*! Persistence Example.
    This example introduces the concept of persistence and elaborates on
    geometry factories. 
*/

#include "Phoenix.h"

using namespace phoenix;
using namespace boost;

int main()
{

    RenderSystem system;

    /*!
        The way that phoenix draws everything is by buffering all drawing calls
        and organizing them in the most effecient way. In the prevous examples,
        we would draw things every frame. While this is usually fine for small
        amounts of drawing calls, there is a much faster way to handle things
        that will be drawn for more than one frame ( or 'persistent' things ).
        
        We must first touch on the concept of geometry factories. Everything in 
        phoenix that draws things is a geometry factory. Geometry factories
        simply make a BatchGeometry object from more simple calls such as
        drawRectangle. The most important of the factories the the RenderSystem
        ( which actually gets all of its functionality from GraphicsFactory2d ).
        
        Every factory function returns a geometry object. You can ignore this
        object and it will only appear for one frame unless you draw it again
        the next frame, or you can grab this object and manipulate it. In this
        example we will manipulate a geometry object to make it persistent.
    */

    /*!
        Here we have the drawRectangle call just as we do in the shapes example,
        but this time we are keeping the return value. BatchGeometry is dynamically
        allocated, so it is a pointer.
    */
    BatchGeometryPtr rectgeom = system.drawRectangle( Rectangle( 200,200, 200,200 ) );

    /*!
        Now we can manipulate our geometry. The first thing we want to do is make
        it persistent.
    */
    rectgeom->setImmediate( false );

    /*!
        Now we'll demonstrate some of the other neat manipulations you can do
        to geometry. We'll rotate and colorize the rectangle. We could also
        scale, translate, add vertices, and many other things.
    */
    rectgeom->rotate( DegreesToRadians( 30 ) );
    rectgeom->colorize( Color( 255,200,127 ) );

    /*!
        Remember, everything that is drawn is turned into a BatchGeometry object.
        This means that you can manipulate everything in this way, even text!
    */

    while( system.run() )
    {
        /*!
            You'll notice now that we no longer have any draw calls in our
            main loop. This is the beauty of persistent geometry.
        */
    }

    return 0;

}
