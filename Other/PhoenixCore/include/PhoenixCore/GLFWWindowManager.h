/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHGLFWWINDOWMANAGER_H__
#define __PHGLFWWINDOWMANAGER_H__


#include "config.h"
#if PH_USE_GLFW

#include <boost/function.hpp>
#include "WindowManager.h"
#include "EventReceiver.h"
#include "Vector2d.h"

namespace phoenix
{

//! GLFW Window Manager Implementation
/*!
	This class implements the GLFW window manager and provides translations
	for Events.
*/
class GLFWWindowManager
	: public WindowManager
{
public:

	//! Instance.
	static boost::shared_ptr<WindowManager> Instance( ){
		if( !instance ){
			instance = WindowManagerPtr( new GLFWWindowManager );
		}
		return instance;
	}

	//! Destructor
	virtual ~GLFWWindowManager(){
		glfwTerminate();
	}

	/*! Open Command.
		Creates a new window and initializes OpenGL. Also sets up callbacks for
		key events.
		\param _sz The size of the window.
		\param _f Fullscreen.
		\param _resize Whether or not the window is natively resizable at all.
	*/
	virtual bool open( const Vector2d& _sz = Vector2d( 640, 480 ), const bool _f = false, const bool _resize = true );

	/*! Close Command.
		Closes the open window (if any).
	*/
	inline virtual void close() { glfwCloseWindow(); }

	//! Gets the culmative running time in seconds.
	virtual double getTime() {
		return glfwGetTime();
	}

	//! Set window caption.
    inline virtual void setWindowTitle(const std::string& _str) { glfwSetWindowTitle(_str.c_str()); }

	//! Set window size
	inline virtual void setWindowSize( const Vector2d& _sz ) { 
		glfwSetWindowSize( (unsigned int) _sz.getX(), (unsigned int) _sz.getY() ); 
		WindowManager::setWindowSize(_sz);
	}

	//! Iconify
	inline virtual void iconify() { glfwIconifyWindow(); }

	//! Restore
	inline virtual void restore() { glfwRestoreWindow(); }

    /*! Set Cursor Visibility.
        Can hide or show the mouse cursor. Hiding it has three effects: The mouse cursor is invisible, the mouse is confined to the screen,
        and coordinates are no longer limited to the window size. By default, the mouse is hidden in fullscreen, but otherwise visible.
    */
    inline virtual void setCursorVisible(const bool _v) { 
		_v ? glfwEnable( GLFW_MOUSE_CURSOR ) : glfwDisable( GLFW_MOUSE_CURSOR ); 
	}

	//! Swap Buffers and Update Events.
	virtual void update();

protected:

	GLFWWindowManager()
		: WindowManager()
	{}

    //! Window Resize callback (from GLFW).
    static void glfwWindowResizeCallback( int width, int height );

	//! Keyboard callback (from GLFW).
    static void glfwKeyboardCallback( int key, int action );

	//! Character callback (from GLFW).
	static void glfwCharacterCallback( int key, int action );

    //! Mouse position callback (from GLFW).
    static void glfwMousePosCallback( int x, int y );

    //! Window callback (from GLFW).
    static int glfwWindowCloseCallback();

    //! Mouse wheel callback (from GLFW).
    static void glfwMouseWheelPosCallback( int pos );
    
#ifdef _GLFW_WM_MOVE_HACK
	//! Window move callback ( from GLFW ).
	static void glfwWindowMoveCallback( int state );
#endif //_GLFW_WM_MOVE_HACK
};

}

#endif //PH_USE_GLFW
#endif //__PHGLFWWINDOWMANAGER_H__

