/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/


#include "Phoenix.h"

using namespace phoenix;

class ShaderTest
{
    public:

        ShaderTest() : 
			system(), mouse(), tintColor()
        {
        }

        virtual ~ShaderTest()
        {
        }

        void onWindowEvent( const WindowEvent& e ) {
            if(e.type == WET_MOUSE_POSITION) {
                mouse = e.vector_data;
            }
        }

        void onSetUniforms( ShaderGroupState& gs ) {
            //! Set the tint color inside the shader
            gs.getShader()->setUniform("tint", tintColor );
        }

        int run()
        {

			//! Load the picture
			TexturePtr picture = system.loadTexture("picture.jpg");

			//! Create a basic shader
			ShaderPtr regular_shader  = new Shader( system.getResourceManager() );
			regular_shader->load("vertex.glsl","fragment.glsl");
			
			if( !regular_shader->ready() ){
				std::cout<<regular_shader->getErrors();
			} else {
				system.getBatchRenderer().setShader( regular_shader );
			}

            //! Listen for mouse events
            WindowManager::Instance()->listen( boost::bind( &ShaderTest::onWindowEvent, this, _1 ) );

			//! Create a blur/desaturate shader, and assign it to a group
			ShaderPtr blur_shader  = new Shader( system.getResourceManager() );
			blur_shader->load("vertex.glsl","desaturate.glsl");
			
			if( !blur_shader->ready() ){
				std::cout<<blur_shader->getErrors();
			} else {
                ShaderGroupState* sgs = new ShaderGroupState(blur_shader);
                sgs->setBeginCallback( boost::bind( &ShaderTest::onSetUniforms, this, _1 ) );
                system.getBatchRenderer().addGroupState( 1, (GroupStatePtr) sgs );
			}

            float r = 0.0f;
            tintColor = Color( 255, 255, 255, 255);

            //! Now just draw some stuff.
            while( system.run() )
            {

                //! Draw some info.
                system.drawText( "Shader Test", Vector2d(16,16) )->colorize( Color(127,127,255,127) );
                
                //! Color is based on mouse.  
                r = mouse.getX() / system.getView().getSize().getX();
                tintColor = Color( r * 255, 127, r * 255, 255 );
                system.drawText( "Color Sent To Shader", Vector2d(16,38) )->colorize( tintColor );

                //! Draw the picture with the effect
				system.setGroup(1);                
				system.drawTexture( picture, (system.getView().getSize()/2) - (picture->getSize()/2) );
				system.setGroup();

                //! Draw the original, unaffected picture.
                system.setDepth(2);
                system.drawTexture( picture, (system.getView().getSize()/2) - (picture->getSize()/2), RotationMatrix(), Vector2d(0.4f,0.4f) );
                system.setDepth();

            }

            return 0;

        }// Run

    protected:
        RenderSystem system;
        Vector2d mouse;
        Color tintColor;

    private:
};


