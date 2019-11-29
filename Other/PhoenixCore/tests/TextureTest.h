/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "Phoenix.h"
#include "RenderTarget.h"

using namespace phoenix;
using namespace std;

class TextureTest
{
    public:

        TextureTest() : system(Vector2d(1024,768))
        {
			system.setResizeBehavior(RZB_EXPAND);
        }

        virtual ~TextureTest()
        {
        }

        /*!
            This actually runs this test. It verifies:
            *) That the default font is loaded and named correctly.
            *) That TextureManager (and ResourceManager) can find textures correctly.
            *) That textures can be manipulated.
            *) That the system can draw the texture.
        */
        int run()
        {

            //print out all the resources
            BOOST_FOREACH( ResourcePtr& resource, system.getResourceManager().getList() )
            {
                system.getDebugConsole()<<"\n Resource "<<resource.get()<<" with name '"<<resource->getName()<<"'";
            }

            //! Print out the font texture's name.
            system.getDebugConsole()<<"\nFont Name: "<<system.getFont()->getTexture()->getName()<<" Pointer: "<< system.getFont().get();

            //! Find the font texture in the resource manager.
            TexturePtr testfind = system.findTexture( system.getFont()->getTexture()->getName() );

            //! Print out if we actually found a texture.
            system.getDebugConsole()<<"\nTest find is valid: "<< testfind;

            //! If we did, print out the name ( should be deffont.png ).
            if(testfind)
            {
                system.getDebugConsole()<<"\nFound font name: "<<testfind->getName();
            }

            //! Now see if we can manipulate textures.
            testfind->lock();
            for( unsigned int i = 0; i < testfind->getSize().getX(); ++i )
            {
                for( unsigned int j = 0; j < testfind->getSize().getY(); ++j)
                {
                    if( testfind->getPixel( Vector2d( (float) i, (float) j) ).getAlpha() < 100 )
                    {
                        testfind->setPixel( Vector2d( (float) i, (float) j), Color(100,200,255,25) );
                    }
                }
            }
            testfind->unlock();

            //! Now see if we can create a custom texture.
            TexturePtr newtexture = new Texture( system.getResourceManager(), Vector2d(512,512) );

            //! Tell me something about it.
            system.getDebugConsole()<<"\nNew texture name: "<<newtexture->getName()<<"Size: "<<newtexture->getSize().getX()<<","<<newtexture->getSize().getY();

            //! Now lets put some color in it!
            newtexture->lock();
            for( unsigned int i = 0; i < newtexture->getSize().getX(); ++i )
            {
                for( unsigned int j = 0; j < newtexture->getSize().getY(); ++j)
                {
                    newtexture->setPixel( Vector2d( (float)i, (float)j ), Color( i,i*2, j) );
                }
            }
            newtexture->unlock();

            //! Now just for fun, how about a render texture?
			RenderTargetPtr rendertarget = new RenderTarget( system.getResourceManager(), Vector2d(256,256) );
			RenderTargetPtr rendertarget2 = new RenderTarget( system.getResourceManager(), Vector2d(640,480) );
			rendertarget->setViewResizeBehavior(TVB_RESIZE_BOTH);
			rendertarget2->setViewResizeBehavior(TVB_RESIZE_BOTH);

			//! Another texture for MRT
			TexturePtr rt2_copy_texture = new Texture( system.getResourceManager(), Vector2d(640,480) );
			rendertarget2->attach( rt2_copy_texture, GL_COLOR_ATTACHMENT1_EXT );
			GLenum bufs[] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT};
			rendertarget2->setDrawBuffers(2,bufs);

            BatchRenderer tbatch; // we need a separate renderer for it.
			GraphicsFactory2d tfactory( tbatch ); //and factory.

			tbatch.setRenderTarget( rendertarget );
			tbatch.setClearColor( Color(0,0,0,0) );
			tbatch.setClearing(true);
            
            system.getDebugConsole()<<"\nRender Target Size: "<<rendertarget->getTexture()->getSize().getX()<<","<<rendertarget->getTexture()->getSize().getY();
            system.getDebugConsole()<<"\nRender Target Texture ID: "<<rendertarget->getTexture()->getTextureId();


            //! Now just draw some stuff.
            while( system.run() )
            {   

                //! Draw some stuff to it.
				tfactory.drawTexture( rendertarget->getTexture(), Vector2d(0,0) );
				tfactory.drawRectangle( phoenix::Rectangle(0,0,50,50) )->colorize(Color(255,0,0));
				tfactory.drawRectangle( phoenix::Rectangle(256-50,0,50,50) )->colorize(Color(255,0,0));
				tfactory.drawRectangle( phoenix::Rectangle(256-50,256-50,50,50) )->colorize(Color(255,0,0));
				tfactory.drawRectangle( phoenix::Rectangle(0,256-50,50,50) )->colorize(Color(255,0,0));
				tbatch.draw();

                //! Draw our textures and some info
                system.drawTexture( newtexture, Vector2d(32,32), 0.0f, Vector2d(1,1), Color(255,255,255,255), ( EventReceiver::Instance()->getKey(PHK_Q) ? EGF_HFLIP : EGF_NONE )&( EventReceiver::Instance()->getKey(PHK_W) ? EGF_VFLIP : EGF_NONE ) );
                system.setDepth( 1.0f );
				system.drawTexture( rendertarget->getTexture(), Vector2d(200,200) );
                system.drawTexturePart( system.getFont()->grab<BitmapFont>()->getTexture() , Vector2d(32,32), phoenix::Rectangle( EventReceiver::Instance()->getMousePosition() , Vector2d( 256,256 ) ), 0.0f, Vector2d(1,1), Color(), ( EventReceiver::Instance()->getKey(PHK_Q) ? EGF_HFLIP : EGF_NONE )&( EventReceiver::Instance()->getKey(PHK_W) ? EGF_VFLIP : EGF_NONE ) );
                
				// Draw the mirrored textureds
				BatchGeometryPtr mirror_g = system.drawTexture( rendertarget2->getTexture(), EventReceiver::Instance()->getMousePosition(), 0.0f, Vector2d(1,1), Color(255,255,255), EGF_VFLIP );
                BatchGeometryPtr mirror_g2 = system.drawTexture( rt2_copy_texture, Vector2d(640,480), 0.0f, Vector2d(1,1), Color(255,255,255) );
                

                system.drawText( "Texture Test: you should see white text", Vector2d(16,16) );
                system.drawText( "with a semi-transparent blue background.", Vector2d(16,32) );
                system.drawText( "You should see a multicolored texture below.", Vector2d(16,48) );
                system.setDepth( 0.0f );


				system.getBatchRenderer().setRenderTarget(rendertarget2);

				mirror_g->setEnabled(false);
				mirror_g2->setEnabled(false);
				system.getBatchRenderer().draw(true);
				mirror_g->setEnabled(true);
				mirror_g2->setEnabled(true);

				system.getBatchRenderer().setRenderTarget();

            }

            return 0;

        }// Run

    protected:
        RenderSystem system;
    private:
};
