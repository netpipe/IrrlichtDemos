#include "DebugConsole.h"
#include "BitmapFont.h"

#include <iostream>

using namespace phoenix;

//! Constructor
DebugConsole::DebugConsole( RenderSystem& _r )
	: GraphicsFactory2d( _r.getBatchRenderer() ), font( _r.getFont() ), enabled( false ), lines(), linelimit(0), backcolor(0,0,0,200), fontcolor(255,201,201)
{
	event_connection = WindowManager::Instance()->listen( boost::bind( &DebugConsole::onWindowEvent, this, _1 ) );
    setDepth( 999.0f );
    setGroup( -10 );
	 _r.getBatchRenderer().addGroupState( -10, GroupStatePtr( new ConsoleGroupState() ) );
    updateLineLimit();
}

//! Listens for Window Events ( To Open/Close the console ).
void DebugConsole::onWindowEvent( const WindowEvent& e )
{
	switch( e.type ){
		case WET_KEY:
			if( e.bool_data == true && e.int_data == PHK_BACKTICK ){
				enabled = !enabled;
			}
			break;
		case WET_RESIZE:
			updateLineLimit();
			break;
		default:
			break;
	}
}

/*
	Write String
*/
void DebugConsole::write( std::string _s )
{
    //stream and temporary storage.
    std::istringstream stream( _s );        
    char temp[256];

    // The first line in the string, even if it's empty, will be added to the current line.
    if( ! lines.empty() )
    {
        stream.getline( temp, 256 );
        lines.back() += std::string( temp );
    }

    // The remaining lines in the string will be addeded as completely new lines.
    for( int i = 0; !stream.eof(); ++i )
    {
        stream.getline( temp, 256 );
        lines.push_back( std::string( temp ) );
    }

    // Limit the number of lines.
    limitLines();
}


/*
	Draw the console
*/
void DebugConsole::draw( )
{
    if( enabled )
    {
		// we want the font's line height to be 16, so adjust the scale accordingly
		float scale = 1.0f;
		
		if( font->getType() == ERT_BITMAP_FONT ) {
			BitmapFontPtr bmfont = boost::dynamic_pointer_cast<BitmapFont>(font);
			scale = 1.0f/( bmfont->getLineHeight()/20.0f );
		}

        // Draw our overlay rectangle at depth- 0.01;
        BatchGeometryPtr geom = drawRectangle( Rectangle( Vector2d(0,0), (WindowManager::Instance())->getWindowSize() ), backcolor, backcolor, backcolor, backcolor);
        setDepth( getDepth() - 0.01f );
        apply( geom );
        setDepth( getDepth() + 0.01f );
        
		//output the text
		std::string output;
		output += std::string("-= ") + std::string( PHOENIXCORE_VERSION ) + std::string(" Debug Console =-");

        unsigned int count = 1;
        for( std::deque< std::string >::iterator i = lines.begin(); i != lines.end(); ++i, ++count )
        {
            output += "\n" + *i;
        }

		geom = font->drawText( output, Vector2d( 8.0f, 8.0f ), fontcolor, Vector2d(scale,scale) );
		apply( geom, EFF_ALL - EFF_TEXTURE );
    }
}

/*
	Group Begin ( sets view ).
*/
void DebugConsole::ConsoleGroupState::begin( BatchRenderer& r ){
    View view;
    view.activate();
}

/*
	Group End (restores view).
*/
void DebugConsole::ConsoleGroupState::end( BatchRenderer& r ){
    r.getView().activate();
}