/*! Views & Timers Example
    This example shows how to manipulate views and how to
    use the built-in timer class.
*/

#include "Phoenix.h"

using namespace phoenix;
using namespace boost;

int main()
{

    RenderSystem system;

    /*!
        We need a timer to keep track of the difference in time
        between each frame. This just allows us to use the concept
        of delta time, which is beyond the scope of this example.
    */
    Timer timer;
    timer.start();

    while( system.run() )
    {
        /*!
            We'll draw some text at various locations so that you know where you're at.
        */
        system.drawText( "Top Left" , Vector2d( 0,0 ) );
        system.drawText( "Center", Vector2d( 320,240 ), Color( 255,127,127 ) );
        system.drawText( "Bottom Right", Vector2d( 640,480 ), Color( 255,0,0 ) );
        system.drawText( "Top Right", Vector2d( 640,0 ), Color( 0,255,0 ) );
        system.drawText( "Bottom Left", Vector2d( 0, 480 ), Color( 0,0,255 ) );

        /*!
            Now for view control. We'll use the time to compute how much we should move
            the view when the player holds down the key. You could also rotate and scale
            the view, but I'll leave that up to you!
        */
		if( EventReceiver::Instance()->getKey( PHK_LEFT ) ){
			system.getView().setPosition( 
				system.getView().getPosition() 
				+ ( Vector2d( -100.0f , 0.0f ) 
				* (float)timer.getTime()) 
			);
		}
		if( EventReceiver::Instance()->getKey( PHK_RIGHT ) ){ 
			system.getView().setPosition( 
				system.getView().getPosition() 
				+ ( Vector2d( 100.0f , 0.0f ) 
				* (float)timer.getTime() ) 
			);
		}
		if( EventReceiver::Instance()->getKey( PHK_UP ) ){
			system.getView().setPosition( 
				system.getView().getPosition() + 
				( Vector2d( 0.0f , -100.0f ) 
				* (float)timer.getTime() ) 
			);
		}
		if( EventReceiver::Instance()->getKey( PHK_DOWN ) ){
			system.getView().setPosition( 
				system.getView().getPosition() 
				+ ( Vector2d( 0.0f , 100.0f ) 
				* (float)timer.getTime() ) 
			);
		}

        /*!
            We reset the timer so we have an accurate measurement for the next frame.
        */
        timer.reset();
    }

    return 0;

}
