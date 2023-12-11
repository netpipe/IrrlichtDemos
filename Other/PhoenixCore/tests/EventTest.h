/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "Phoenix.h"

using namespace phoenix;

class EventTest
{
    public:

        EventTest() : system()
        {
			system.getFont()->setScale(Vector2d(0.75f,0.75f));
        }

        virtual ~EventTest()
        {
        }

        /*!
            This actually runs this test. It verifies:
            *) That the event receiver is handling keyboard events.
            *) That the event receiver is handling mouse events.
        */
        int run()
        {

            //! Now just draw some stuff.
            while( true )
            {

                //! limit to 30fps
                if( system.getTime() < 1.0f/30.0f ) continue;

                // Run the system
                if( ! system.run() )
                    break;

                //! For every possible one of the 256 keys, we'll draw a little icon if they've been pressed or not
                for( unsigned char i = 0; i < 16; ++i)
                {
                    for( unsigned char j = 0; j <16; ++j)
                    {
                        Color textcolor;

                        //! If it's down, we'll set the color to blue
                        if(EventReceiver::Instance()->getKey( Key((j*16)+i) ) ) textcolor = Color(200,200,255);
                        //! If it's up, we'll set the color to gray
                        if( !EventReceiver::Instance()->getKey( Key((j*16)+i) ) ) textcolor = Color(255,255,255,150);
                        //! If it was just pressed, make it white
                        if(EventReceiver::Instance()->getKeyPressed( Key((j*16)+i) ) ) textcolor = Color(255,255,255);
                        //! If it was just released, make it red.
                        if(EventReceiver::Instance()->getKeyReleased( Key((j*16)+i) ) ) textcolor = Color(255,200,200);

                        //! Draw it
                        system.drawText( "O", Vector2d(75,75)+Vector2d( (float) i*16, (float) j*16 ), textcolor);
                    }
                }

                //! Draw an @ symbol where the mouse is, and once again use our custom font to do it.
                Color textcolor = Color(255,0,0);
                if(EventReceiver::Instance()->getMouseButton( PHK_MB_LEFT ) ) textcolor = Color(0,255,0);
                if(EventReceiver::Instance()->getMouseButton( PHK_MB_RIGHT ) ) textcolor = Color(0,0,255);
                if(EventReceiver::Instance()->getMouseButton( PHK_MB_MIDDLE ) ) textcolor = Color(255,255,255);
                system.drawText( "@",EventReceiver::Instance()->getMousePosition(), textcolor);

                //! Draw some info.
                system.drawText( "Event Test: Below is an ASCII Table", Vector2d(16,16) );
                system.drawText( "Not all will change, but you should be able", Vector2d(16,32) );
                system.drawText( "make some pretty colors by mashing keys.", Vector2d(16,48) );

				//! Draw the keyboard string.
				system.drawText( std::string("Keyboard String: ") + EventReceiver::Instance()->getKeyboardString(), Vector2d( 16, 448 ) ); 

			    system.getDebugConsole()<<"\nGeoms "<<system.getBatchRenderer().count()<<", FPS: "<<system.getFPS();

            }

            return 0;

        }// Run

    protected:
        RenderSystem system;
    private:
};
