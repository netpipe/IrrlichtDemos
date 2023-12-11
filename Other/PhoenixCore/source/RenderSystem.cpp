/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "config.h"
#include "glew/GL/glew.h"
#include "RenderSystem.h"
#include "DroidSansMono.h"
#include "BitmapFont.h"
#include "BMFontLoader.h"
#include "GLFWWindowManager.h"
#include "soil/SOIL.h"

using namespace phoenix;

////////////////////////////////////////////////////////////////////////////////
//Static Members
////////////////////////////////////////////////////////////////////////////////
int RenderSystem::src_blend = GL_SRC_ALPHA;
int RenderSystem::dst_blend = GL_ONE_MINUS_SRC_ALPHA;

////////////////////////////////////////////////////////////////////////////////
//Construct & Destruct
////////////////////////////////////////////////////////////////////////////////



void RenderSystem::initialize( const Vector2d& _sz , bool _fs, bool _resize, bool _reint )
{
	if( _reint ){
		// Re-initialization requires us to dump everything.
		resources.clear();
		if( console ) console = boost::shared_ptr<DebugConsole>();
		if( font ) font = FontPtr();
		try{
			WindowManager::Instance()->close(); // close any open window.
		}catch( WindowManager::BadInstance ){
		}
	}

	WindowManagerPtr windowManager;

	// Does a window manager already exist? if so, use it
	try {
		windowManager = WindowManager::Instance();
	} 
	// If not, attempt to create one.
	catch( WindowManager::BadInstance ){

		#if PH_USE_GLFW

		// GLFW Window Manager
		windowManager = GLFWWindowManager::Instance();  
  
		#endif //PH_USE_GLFW

	}

    if(!windowManager) {
        throw std::runtime_error("Phoenix has no Window Manager. This is bad.");
    }

	// Create our window
	if( !windowManager->open( _sz, _fs, _resize ) ) { throw std::runtime_error("Failed to open a window."); }

	// Initialize GLEW
	if( glewInit() != GLEW_OK ){
		throw std::runtime_error("GLEW Failed to init, what are you trying to run me on?");
	}

	// Listen to events.
	event_connection = windowManager->listen( boost::bind( &RenderSystem::onWindowEvent, this, _1 ) );

    // viewport the same as the window size.
    renderer.getView().setSize();

    // Orthogonal projection.
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    glOrtho(0.0f, _sz.getX(), _sz.getY(), 0.0f, 1000.0f, -1000.0f);

    // load up identity for the modelview matrix.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up depth buffer
    #ifdef DISABLE_DEPTHBUFFER
        // No need for depth buffer.
        glDisable(GL_DEPTH_TEST);
    #else
        // Enable depth testing and set the function
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    #endif

    // Smoooth shading, Nicest Hinting.
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Enable blending and set our blending mode.
    glEnable(GL_BLEND);
    setBlendMode(); // Default is for 2d transluceny with RGBA textures.

    //! Default material is white.
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Material mode
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable(GL_COLOR_MATERIAL);

    //load our default font
	BitmapFontPtr bmfont = new BitmapFont( *this );
	BMFontLoader ldr( *this, bmfont );
	ldr.loadFromString( get_droid_sans_mono_fnt_file() );

	const unsigned char* data = get_droid_sans_mono_file_data();
	
	bmfont->setPage( 0, loadTexture(
		get_droid_sans_mono_file_data(),
		get_droid_sans_mono_file_size(),
        std::string("Droid Sans Mono")
	) );

	font = bmfont;

	//! Make a console
	console = boost::shared_ptr<DebugConsole>( new DebugConsole( *this ) );

	//! Report version information
	(*console)<<"PhoenixCore initialized.\n"<<"Using GLEW "<<glewGetString(GLEW_VERSION)<<"\n";

	if( GLEW_VERSION_2_0 ){
		(*console)<<"OpenGL Version 2.0 available\n";
	}
	if( GLEW_VERSION_3_0 ){
		(*console)<<"OpenGL Version 3.0 available\n";
	}

    // Clear the screen to black 
	renderer.setClearing(true);
    renderer.clearScreen( Color(0,0,0) );

    //!start the timer
    fpstimer.start();

}


RenderSystem::~RenderSystem()
{
	event_connection.disconnect();
	WindowManager::Instance()->close();
}


////////////////////////////////////////////////////////////////////////////////
//run function, should be called once every loop, it makes things ready for
//a render.
////////////////////////////////////////////////////////////////////////////////

bool RenderSystem::run()
{

    //Render the Debug Console.
    console->draw();

    //Call our own draw function
    renderer.draw();

    //flip the screen (this also polls events).
	WindowManager::Instance()->update();

    //Clean resources
    resources.clean();

    //store the new framerate
    double newframerate = 1.0f / fpstimer.getTime();
    framerate = (0.6f * newframerate) + (0.4f * framerate);

    //Start our fps timer
    fpstimer.reset();

    return !_quit; // Quit is set to true when the window manager has signaled to close.
}

////////////////////////////////////////////////////////////////////////////////
// Window Events
////////////////////////////////////////////////////////////////////////////////

void RenderSystem::onWindowEvent( const WindowEvent& e )
{
	switch( e.type )
	{
	case WET_CLOSE:
		_quit = true;
		break;
	case WET_KEY:
		if( e.bool_data == true && e.int_data == PHK_ESC ){
			_quit = true;
		}
		break;
	case WET_RESIZE:

		switch( resize_behavior )
		{
		case RZB_EXPAND:
			renderer.getView().setSize( e.vector_data );
			glMatrixMode(GL_PROJECTION); 
			glLoadIdentity();
			glOrtho(0.0f, e.vector_data.getX(), e.vector_data.getY(), 0.0f, 1000.0f, -1000.0f);
			break;

		case RZB_SCALE:
			renderer.getView().setSize( e.vector_data );
			break;

		case RZB_REVERT:
			WindowManager::Instance()->setWindowSize( renderer.getView().getSize() );
			break;

		case RZB_NOTHING:
		default:
			break;
		}

		break;
	default:
		break;
	};
}

////////////////////////////////////////////////////////////////////////////////
//Load texture function
////////////////////////////////////////////////////////////////////////////////

TexturePtr RenderSystem::loadTexture( const std::string& _fn, bool _l )
{

	//This is the class that will hold our texture
	TexturePtr ctext = new Texture( resources );

	int width=0,height=0;

	GLuint newtextid = SOIL_load_OGL_texture
        (
                _fn.c_str(),
                SOIL_LOAD_RGBA,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_TEXTURE_REPEATS,
                &width, &height
        );

	if( newtextid != 0 )
	{

        //Load the texture
        glBindTexture(GL_TEXTURE_2D, newtextid);

        //use linear filtering
        if ( _l == true)
        {

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        }
        else
        {

            //Use nearest filter
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        }

        //Set up the Texture class
        ctext->setTextureId(newtextid);
        ctext->setWidth(width);
        ctext->setHeight(height);
        ctext->setName( _fn );

        //Return our texture
        return ctext;

    }
    else
    {

        TexturePtr ctext = new Texture( resources );
        ctext->setTextureId(0);
        ctext->setWidth(0);
        ctext->setHeight(0);
        ctext->setName("FAILED TO LOAD");

        //Return our texture
        return ctext;

    }

    //should never happen
    return TexturePtr();
    assert(true);

}

// Load texture from memory.
TexturePtr RenderSystem::loadTexture( const unsigned char* const _d, const unsigned int _len, const std::string& _name, bool _lin )
{

	//This is the class that will hold our texture
	TexturePtr ctext = new Texture( resources );

	GLuint newtextid = SOIL_load_OGL_texture_from_memory(
		_d,
		_len,
		SOIL_LOAD_RGBA,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS
	);

	if( newtextid != 0 )
	{
        //Load the texture
        glBindTexture(GL_TEXTURE_2D, newtextid);

        //use linear filtering
        if ( _lin == true)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else
        {
            //Use nearest filter
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        //Set up the Texture class
        ctext->setTextureId(newtextid);

        //Get the size from the buffer
        int _w = 0; int _h = 0; int _c = 4;
        unsigned char* tmp = SOIL_load_image_from_memory(_d, _len, &_w, &_h, &_c , SOIL_LOAD_AUTO); 

        //Set the sizes in the Texture object
        ctext->setWidth( _w );
        ctext->setHeight( _h );
        if(!_name.size())
            ctext->setName( "Loaded From Memory" );
        else 
            ctext->setName( _name );

        //Return our texture
        return ctext;
    }
    else
    {
        TexturePtr ctext = new Texture( resources );
        ctext->setTextureId(0);
        ctext->setWidth(0);
        ctext->setHeight(0);
        ctext->setName("FAILED TO LOAD");

        //Return our texture
        return ctext;
    }

    //should never happen
    return TexturePtr();
    assert(true);

}


////////////////////////////////////////////////////////////////////////////////
// Find texture functions.
////////////////////////////////////////////////////////////////////////////////

//! Find texture.
TexturePtr RenderSystem::findTexture(const std::string& _i)
{
    ResourcePtr findtexture = resources.find(_i);
    if( findtexture )
    {
        return findtexture->grab< Texture >();
    }
    else
    {
        return TexturePtr();
    }

}

//! Find texture.
TexturePtr RenderSystem::findTexture(const GLuint& _n)
{
	boost::recursive_mutex::scoped_lock( resources.getMutex() );
    for (unsigned int i=0;i<resources.count();i++)
    {
        if( resources.get(i)->getType() == ERT_TEXTURE )
        {
            if ( resources.get(i)->grab<Texture>()->getTextureId() == _n )
            {
                return resources.get(i)->grab<Texture>();
            }
        }
    }
    return TexturePtr();
}


////////////////////////////////////////////////////////////////////////////////
//Draw text on the screen
////////////////////////////////////////////////////////////////////////////////

BatchGeometryPtr RenderSystem::drawText( const std::string& s, const Vector2d& p, const Color& _c, const Vector2d& _scale )
{
    font->setDepth( factory.getDepth() );
    font->setGroup( factory.getGroup() );
	return font->drawText( s, p, _c, _scale );
}

