/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "Phoenix.h"

using namespace phoenix;

class ChangeTest
{
    public:

        ChangeTest() : system(), changed(false)
        {
        }

        virtual ~ChangeTest()
        {
        }

        int run()
        {

			system = boost::shared_ptr<RenderSystem>( new RenderSystem );


            //! Now just draw some stuff.
            while( system->run() )
            {

                //! Draw some info.
				if( !changed )
					system->drawText( "Press space to change the rendersystem.", Vector2d(16,16) );
				else
					system->drawText( "Ta-da!", Vector2d(16,16) );

				if( !changed && EventReceiver::Instance()->getKeyPressed( PHK_SPACE ) ){
					try{

						system = boost::shared_ptr<RenderSystem>();
						system = boost::shared_ptr<RenderSystem>( new RenderSystem(Vector2d( 800,600 )) );
						std::cout<< system->getFont()->getTexture()->getName() << system->getFont()->getTexture()->getTextureId() << std::endl;

					}catch( std::exception e ){
						std::cout<< e.what() << std::endl;
						throw;
					}catch( ... ){
						std::cout<<"Something went terribly wrong."<<std::endl;
						throw;
					}
				}


            }

            return 0;

        }// Run

    protected:
		boost::shared_ptr<RenderSystem> system;
		bool changed;
    private:
};