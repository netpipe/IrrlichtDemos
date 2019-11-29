/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "Phoenix.h"
#include "BMFontLoader.h"

using namespace phoenix;

class FontTest
{
    public:

        FontTest() : system(Vector2d(800,600))
        {
        }

        virtual ~FontTest()
        {
        }

        int run()
        {

			//! Test
			FontPtr font = system.getFont();

			//! Rotating Text
			BatchGeometryPtr hellotext = font->drawText( "Hello, \nWorld!", Vector2d( 50,50 ), Color(127,127,255) );
			hellotext->setImmediate( false );
			hellotext->setDepth(400.3f);
			hellotext->update();

			//! Draw some text.
			font->setScale( Vector2d( 0.75f, 0.75f ) );
				
			std::string test_text = "\"Would you tell me, please, which way I ought to go from here?\"\n"\
						"\"That depends a good deal on where you want to get to,\" said the Cat.\n"\
						"\"I don't much care where -\" said Alice.\n"\
						"\"Then it doesn't matter which way you go,\" said the Cat";

			Vector2d dim = font->getTextDimensions(test_text);

			font->drawText( test_text, Vector2d(16,16), Color(127,127,255), Vector2d(0,0), T_ALIGN_LEFT )->setImmediate( false );
			font->drawText( test_text, Vector2d(16,16 + dim.getY() + 10 ), Color(127,255,127), Vector2d(0,0), T_ALIGN_CENTER )->setImmediate( false );
            font->drawText( test_text, Vector2d(16,16 + dim.getY()*2 + 20 ), Color(255,127,127), Vector2d(0,0), T_ALIGN_RIGHT )->setImmediate( false );

			BatchGeometryPtr g = system.drawRectangle( phoenix::Rectangle( Vector2d(16,16), dim ) );
				g->colorize( Color(127,127,127,120) );
				g->setImmediate( false );

			std::stringstream strm;
			strm<<"Width: "<<dim.getX()<<"\nHeight: "<<dim.getY();

			font->drawText( strm.str(), Vector2d(16,500) )->setImmediate( false );

            //! Now just draw some stuff.
            while( system.run() )
            {

				hellotext->rotate( 0.001f );

            }

            return 0;

        }// Run

    protected:
        RenderSystem system;
    private:
};