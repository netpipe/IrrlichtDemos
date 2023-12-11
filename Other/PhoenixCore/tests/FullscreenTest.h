/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "Phoenix.h"

using namespace phoenix;

class FullscreenTest
{
    public:

		FullscreenTest() : system()
        {
			system = boost::shared_ptr<RenderSystem>(
				new RenderSystem(Vector2d(640,480),true)
			);
        }

        virtual ~FullscreenTest()
        {
        }

        int run()
        {

            //! Now just draw some stuff.
            while( system->run() )
            {
                //! Draw some info.
				system->drawText( "Hooray fullscreen", Vector2d(16,16) );

            }

            return 0;

        }// Run

    protected:
		boost::shared_ptr<RenderSystem> system;
    private:
};