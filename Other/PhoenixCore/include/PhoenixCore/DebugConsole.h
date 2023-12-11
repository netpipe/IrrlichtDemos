#ifndef __PH_DEBUG_CONSOLE_H__
#define __PH_DEBUG_CONSOLE_H__

#include <string>
#include "config.h"
#include "2dGraphicsFactory.h"
#include "WindowManager.h"
#include "Color.h"
#include "View.h"

namespace phoenix
{

class RenderSystem;
class Font;

//! Debug Console
/*!
    The debug console provides an easy and familiar way to debug graphical applications in phoenix. Any
	class that can be written to an ostream can be used in the write() function. There is usually only
	one DebugConsole and it is accessible through RenderSystem::getDebugConsole();
*/
class DebugConsole
    : public virtual GraphicsFactory2d
{
public:

	//! Constructor
    /*!
        Makes a new debug console. Requires a reference to the render system.
    */
    DebugConsole( RenderSystem& _r );

    //! Destructor
    virtual ~DebugConsole() {
		event_connection.disconnect();
	}

	//! Listens for Window Events ( To Open/Close the console ).
	void onWindowEvent( const WindowEvent& e );

    //! Get the font.
    boost::intrusive_ptr<Font> getFont() { return font; }

    //! Set the font.
    void setFont( boost::intrusive_ptr<Font> _f ) { font = _f; }

    //! Set the background color.
    void setBackgroundColor( const Color& _c = Color(0,0,0,200) ) { backcolor = _c; }

    //! Get the background color.
    const Color& getBackgroundColor() { return backcolor; }

    //! Set the color used to draw text.
    void setFontColor( const Color& _c = Color(127,127,255) ) { fontcolor = _c; }

    //! Get the text color.
    const Color& getFontColor() { return fontcolor; }

    //! Write a string to the console.
    /*!
        Newline characters '\n' are automatically split.
    */
    virtual void write( std::string _s );

    //! Templated write function.
    /*!
        Used by the stream operator to convert types into
        strings. Supports any type that stringstreams do.
    */
    template < typename T >
    void write( T _v )
    {
        std::ostringstream stream;
        stream << _v;
        write( stream.str() );
    }

    //! Clear the debug console.
    void clear()
    {
        lines.clear();
    }

    //! Prints the console's content to cout.
    void dump()
    {
        for( std::deque< std::string >::iterator i = lines.begin(); i != lines.end(); ++i )
        {
            std::cout<<(*i)<<std::endl;
        }
    }

    //! Update the line limit based on the size of the window.
    void updateLineLimit()
    {
		try{
			linelimit = (unsigned int)(((WindowManager::Instance())->getWindowSize().getY())/20.0f) - 2;
		} catch ( WindowManager::BadInstance ){
			linelimit = 10;
		}
    }

    //! Removes old lines that are beyond the limit of displayable lines.
    void limitLines()
    {
        unsigned int size = lines.size();
        if( size > linelimit )
            lines.erase( lines.begin(), lines.begin()+( size-linelimit ) );
    }

    //! Stream operator.
    /*!
        Uses the templated write() function to stream types into the console.
    */
    template < typename T >
    DebugConsole& operator << ( T _s )
    {
        write<T>( _s );
        return *this;
    }

    //! Draw the console.
    /*!
        Runs through all the lines and draws them. Automatically called on the
		WET_UPDATE event.
    */
    void draw( );
    
	//! Get whether or not the console is enabled
	inline const bool getEnabled() { return enabled; }

	//! Set whether or not the console is enabled
	inline void setEnabled( const bool &_b) { enabled = _b; }

protected:

	//! Connection to Window Manager
	boost::signals2::connection event_connection;

	//! Font
    boost::intrusive_ptr<Font> font;

	//! Enabled or not.
    bool enabled;

	//! List of lines.
    std::deque< std::string > lines;

	//! The most lines that can be displayed on the screen.
    unsigned int linelimit;

	//! Background Color.
    Color backcolor;

	//! Foreground Color.
    Color fontcolor;

	//! Debug Console Group State Manager.
	/*!
		Changes views so user transformations don't
		effect the console.
	*/
	class ConsoleGroupState
		:public GroupState
	{
		virtual void begin( BatchRenderer& );
		virtual void end( BatchRenderer& );
	};

};

} //namespace phoenix;

#include "RenderSystem.h"
#include "Font.h"

#endif // __PH_DEBUG_CONSOLE_H__