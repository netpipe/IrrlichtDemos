/*! Hello World Example
    This example shows the most basic of tasks with Phoenix: Saying hello.
*/

/*!
    Include our header.
*/
#include "Phoenix.h"

/*!
    We'll use the phoenix namespace so that we don't
    have to type phoenix:: in front of everything.
*/
using namespace phoenix;

/*!
    Your standard main() function. 
*/
int main()
{

    /*!
        We'll create a RenderSystem. This opens up a window and gets everything
        ready for us to draw. We could optionally set a different screen size
        (the default is 640x480) and tell it to be fullscreen.
    */
	RenderSystem system;

    /*! The Loop.
        The loop is the main feature of most games. The loop runs until the user
        decides to exit ( by closing the window or pressing the escape key ).
        We'll call the system's run() function. The function does some magic 
        behind the scenes to swap buffers, update events, calcuate frame per
        second, and other things.
    */
    while( system.run() )
    {
        /*!
            Now we say hello. System has a method called drawText that can do just that.
            The first parameter is obviously the text we'd like to draw. The second is a
            Vector2d that tells the system where to draw it. And we'd also like the color
            of that text to be red.
        */
        system.drawText( "Hello, World!", Vector2d(50,50), Color( 255,0,0 ) );

    }

    return 0;

}
