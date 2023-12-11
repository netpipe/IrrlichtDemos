/*! Texture Example.
    This example shows how to load and draw a simple texture.
*/

#include "Phoenix.h"

/*!
    This time we will use the phoenix namespace and the boost namespace.
    Phoenix uses boost a lot internally and sometimes it's useful to 
    use both.
*/
using namespace phoenix;
using namespace boost;

int main()
{

    RenderSystem system;

    /*!
        Now that we have a system up, we can go ahead and load a texture.
        The system provides that functionality. We'll keep a shared_ptr
        to the loaded texture. Phoenix uses shared_ptr for all dynamically
        allocated resources.
    */
	TexturePtr feather = system.loadTexture( std::string("feather.png") );

    while( system.run() )
    {
        /*!
            Now we draw it. Just as in the text and shape examples, this is fairly
            easy. For fun, we'll also rotate the texture 45 degrees and scale it
            by 150%. We could also colorize and flip the texture.
        */
        system.drawTexture( feather, Vector2d( 320,240 ), DegreesToRadians( 45 ), Vector2d( 1.5f, 1.5f ) );

    }

    return 0;

}
