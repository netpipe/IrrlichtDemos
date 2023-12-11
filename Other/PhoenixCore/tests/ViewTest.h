/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "Phoenix.h"

using namespace phoenix;

class ViewTest
{
    public:

        ViewTest() : system()
		{
        }

        virtual ~ViewTest()
        {
        }

        /*!
            This actually runs this test. It verifies:
            *) That a view can translate
            *) That a view can rotate.
            *) That a view can scale.
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

                //! Move the view with the arrow keys
                if(EventReceiver::Instance()->getKey( PHK_LEFT ) )
                    system.getView().setPosition( system.getView().getPosition()+Vector2d( -5,0 ) );
                if(EventReceiver::Instance()->getKey( PHK_RIGHT ) )
                    system.getView().setPosition( system.getView().getPosition()+Vector2d( 5,0 ) );
                if(EventReceiver::Instance()->getKey( PHK_UP ) )
                    system.getView().setPosition( system.getView().getPosition()+Vector2d( 0,-5 ) );
                if(EventReceiver::Instance()->getKey( PHK_DOWN ) )
                    system.getView().setPosition( system.getView().getPosition()+Vector2d( 0,5 ) );

                //! Rotate with q and w
                if(EventReceiver::Instance()->getKey( PHK_W ) )
                    system.getView().setRotation( system.getView().getRotation() + 10.0f );
                if(EventReceiver::Instance()->getKey( PHK_Q ) )
                    system.getView().setRotation( system.getView().getRotation() - 10.0f );

                //! Scale with a and s
                if(EventReceiver::Instance()->getKey( PHK_A ) )
                    system.getView().setScale( system.getView().getScale() * 0.99f );
                if(EventReceiver::Instance()->getKey( PHK_S ) )
                    system.getView().setScale( system.getView().getScale() * 1.01f );

                //! Draw some lines
                system.drawRay( Vector2d(0,0), Vector2d(640,480) );
                system.drawRay( Vector2d(0,0), Vector2d(640,-480) );
                system.drawRay( Vector2d(0,0), Vector2d(-640,-480));
                system.drawRay( Vector2d(0,0), Vector2d(-640,480));

                //! Draw some markers.
                system.drawText("100%", Vector2d(640,480) );
                system.drawText("100%", Vector2d(640,-480) );
                system.drawText("100%", Vector2d(-640,-480) );
                system.drawText("100%", Vector2d(-640,480) );

                //! Draw some more markers.
                system.drawText("50%", Vector2d(640,480)/2 );
                system.drawText("50%", Vector2d(640,-480)/2 );
                system.drawText("50%", Vector2d(-640,-480)/2 );
                system.drawText("50%", Vector2d(-640,480)/2 );


                //! Draw some info.
                system.drawText( "View Test", Vector2d(0,0) );
                system.drawText( "", Vector2d(16,48) );

            }

            return 0;

        }// Run

    protected:
        RenderSystem system;
    private:
};

