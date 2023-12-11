
#include "config.h"

#if PH_USE_GLFW

#include "GLFWWindowManager.h"

using namespace phoenix;

/*!
    Open function.
*/
bool GLFWWindowManager::open( const Vector2d& _sz, const bool _f, const bool _resize )
{

    // Set our internal screen size variable.
    screensize = _sz;

	// No GLFW functions may be called before this
	if( !glfwInit() ) return false;

	// Important defines
	#define RED_BITS 8
	#define BLUE_BITS 8
	#define GREEN_BITS 8
	#define ALPHA_BITS 8
	#define STENCIL_BITS 0
	#ifdef DISABLE_DEPTHBUFFER
		#define DEPTH_BITS 0
	#else
		#define DEPTH_BITS 8
	#endif

	int mode = _f ? GLFW_FULLSCREEN : GLFW_WINDOW;
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, !_resize);

	if( !glfwOpenWindow(int(_sz.getX()), int(_sz.getY()), RED_BITS, BLUE_BITS, GREEN_BITS, ALPHA_BITS, DEPTH_BITS, STENCIL_BITS, mode) ) return false;


	// Disable vsync.
	glfwSwapInterval( 0 );

	// Set the window caption
	setWindowTitle( PHOENIXCORE_VERSION );

	// Set GLFW event callbacks
	glfwSetKeyCallback			( &GLFWWindowManager::glfwKeyboardCallback		);
	glfwSetCharCallback			( &GLFWWindowManager::glfwCharacterCallback		);
	glfwSetMouseButtonCallback  ( &GLFWWindowManager::glfwKeyboardCallback		);
	glfwSetMousePosCallback		( &GLFWWindowManager::glfwMousePosCallback		);
	glfwSetWindowCloseCallback	( &GLFWWindowManager::glfwWindowCloseCallback	);
	glfwSetMouseWheelCallback	( &GLFWWindowManager::glfwMouseWheelPosCallback	);
    glfwSetWindowSizeCallback	( &GLFWWindowManager::glfwWindowResizeCallback	);

#ifdef _GLFW_WM_MOVE_HACK
	glfwSetWindowMoveCallback( &GLFWWindowManager::glfwWindowMoveCallback );
#endif

	// Disable key repeat; event receiver manages that.
	glfwDisable(GLFW_KEY_REPEAT);

	return true;

}

/*
	Swap Buffers and Update Events.
*/
void GLFWWindowManager::update() 
{
	// send the update event. ( done first because glfwSwapBuffers sends new events ).
	WindowEvent e;
	e.type = WET_UPDATE;
	signal(e);

	glfwSwapBuffers(); 
}

//! Window Resize callback (from GLFW).
void GLFWWindowManager::glfwWindowResizeCallback( int width, int height )
{
	WindowManagerPtr wm = WindowManager::Instance();

	Vector2d size( (float) width, (float) height );
	wm->setWindowSize( size );

	WindowEvent e;
	e.type = WET_RESIZE;
	e.vector_data = size;
	wm->signal(e);
}

//! Keyboard callback (from GLFW).
void GLFWWindowManager::glfwKeyboardCallback( int key, int action )
{
	WindowEvent e;
	e.type = WET_KEY;
	e.int_data = key;
	e.bool_data = action == GLFW_PRESS ? true : false;
	Instance()->signal(e);
}

//! Character callback (from GLFW).
void GLFWWindowManager::glfwCharacterCallback( int key, int action )
{
		WindowEvent e;
		e.type = WET_CHAR;
		e.int_data = key;
		e.bool_data = action == GLFW_PRESS ? true : false;
		Instance()->signal(e);
}


//! Mouse position callback (from GLFW).
void GLFWWindowManager::glfwMousePosCallback( int x, int y )
{
	WindowEvent e;
	e.type = WET_MOUSE_POSITION;
	e.bool_data = true;
	e.vector_data = Vector2d( float(x), float(y) );
	Instance()->signal(e);
}

//! Window callback (from GLFW).
int GLFWWindowManager::glfwWindowCloseCallback()
{
	WindowEvent e;
	e.type = WET_CLOSE;
	e.bool_data = true;
	Instance()->signal(e);
	return false;
}

//! Mouse wheel callback (from GLFW).
void GLFWWindowManager::glfwMouseWheelPosCallback( int pos )
{
	WindowEvent e;
	e.type = WET_MOUSE_WHEEL;
	e.bool_data = true;
	e.int_data = pos;
	Instance()->signal(e);
}

#ifdef _GLFW_WM_MOVE_HACK
void GLFWWindowManager::glfwWindowMoveCallback( int state )
{
	WindowEvent e;
	e.type = WET_MOVE;
	e.bool_data = state == GLFW_MOVE_ENTER ? 1 : 0;
	Instance()->signal(e);
}

#endif //_GLFW_WM_MOVE_HACK
#endif //PH_USE_GLFW