
#include "IrrlichtWidget.h"

#include <gdk/gdkx.h>

irr::video::ITexture* texture = 0;

IrrlichtWidget::IrrlichtWidget() : Gtk::DrawingArea()
{
	context = 0;
	window = 0;
	glxwin = 0;
	createdwindow = false;
	
	signal_expose_event().connect(sigc::mem_fun(*this, &IrrlichtWidget::OnExpose));
	signal_configure_event().connect(sigc::mem_fun(*this, &IrrlichtWidget::OnConfigure));
}
IrrlichtWidget::~IrrlichtWidget()
{
	if (createdwindow)
	{
		// Destroy window
		glXDestroyContext((Display*)device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display, context);
		glXDestroyWindow((Display*)device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display, glxwin);
		XDestroyWindow((Display*)device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display, window);
	}
	
	usecount--;
	if (!usecount)
	{
		device->drop();
		device = 0;
		irrcontext = 0;
	}
}

void IrrlichtWidget::beginScene(void)
{
	irr::video::SColor color (255,0,0,255);
	device->getVideoDriver()->beginScene(true, true, color);
}
void IrrlichtWidget::endScene(void)
{
	glFlush();
	glXSwapBuffers((Display*)device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display, glXGetCurrentDrawable());
}

bool IrrlichtWidget::OnExpose(GdkEventExpose *event)
{
	printf("OnExpose\n");
	if (!context)
	{
		GLXWindow parent = GDK_WINDOW_XWINDOW(get_window()->gobj());
		// Load context
		if (!device)
		{
			// Open irrlicht
			irr::SIrrlichtCreationParameters params;
			params.DriverType = irr::video::EDT_OPENGL;
			params.WindowId = (void*)parent;
			params.WindowSize.Width = get_width();
			params.WindowSize.Height = get_height();
			params.EventReceiver = 0;
			device = irr::createDeviceEx(params);
			context = glXGetCurrentContext();
			irrcontext = context;
			usecount++;
			texture = device->getVideoDriver()->getTexture("tests/test1/materials/wood.png");
		}
		else
		{
			// Create window
			Display *display = (Display*)device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display;
			irr::SIrrlichtCreationParameters params;
			params.DriverType = irr::video::EDT_OPENGL;
			params.WindowId = (void*)glxwin;
			params.WindowSize.Width = get_width();
			params.WindowSize.Height = get_height();
			params.EventReceiver = 0;
			
			const int MAX_SAMPLES = 16;
			int visualAttrBuffer[] =
			{
				GLX_RENDER_TYPE, GLX_RGBA_BIT,
				GLX_RED_SIZE, 4,
				GLX_GREEN_SIZE, 4,
				GLX_BLUE_SIZE, 4,
				GLX_ALPHA_SIZE, params.WithAlphaChannel?1:0,
				GLX_DEPTH_SIZE, params.ZBufferBits,
				GLX_DOUBLEBUFFER, GL_TRUE,
				GLX_STENCIL_SIZE, 1,
				GLX_SAMPLE_BUFFERS_ARB, 1,
				GLX_SAMPLES_ARB, MAX_SAMPLES,
				None
			};


			int nitems = 0;
			GLXFBConfig *configList = glXGetFBConfigs(display, 0, &nitems);

			XVisualInfo* visual;
			XSetWindowAttributes attributes;
			visual = glXGetVisualFromFBConfig(display,*configList);

			// create color map
			Colormap colormap;
			colormap = XCreateColormap(display,
				RootWindow(display, visual->screen),
				visual->visual, AllocNone);

			attributes.colormap = colormap;
			attributes.border_pixel = 0;
			attributes.event_mask = StructureNotifyMask | FocusChangeMask;

			window = XCreateWindow(display,
				parent,
				0, 0, get_width(), get_height(), 0, visual->depth,
				InputOutput, visual->visual,
				CWBorderPixel | CWColormap | CWEventMask,
				&attributes);
			XMapRaised(display, window);
			glxwin = glXCreateWindow(display, *configList, window, NULL);
			context = glXCreateContext(display, visual, irrcontext, True);
			XFree(configList);
 
			glXMakeContextCurrent(display, glxwin, glxwin, context);
			
			createdwindow = true;
			usecount++;
			texture = device->getVideoDriver()->getTexture("tests/test1/materials/wood.png");
		}
	}
	else
	{
		// Make context current
		glXMakeCurrent((Display*)device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display, glxwin, context); 
	}
	
	if (window)
	{
		XWindowChanges changes;
		changes.width = get_width();
		changes.height = get_height();
		XConfigureWindow((Display*)device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display, window, CWWidth | CWHeight, &changes);
		XMapRaised((Display*)device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display, window);
	}
	else if (device)
	{
		XWindowChanges changes;
		changes.width = get_width();
		changes.height = get_height();
		XConfigureWindow((Display*)device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display, device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Window, CWWidth | CWHeight, &changes);
	}
	
	// Render here!
	beginScene();
	device->getSceneManager()->drawAll();
	if (texture)
		device->getVideoDriver()->draw2DImage(texture, irr::core::position2d<irr::s32>(50,50));
	device->getGUIEnvironment()->drawAll();
	endScene();
	
	if (!glxwin)
	{
		glxwin = glXGetCurrentDrawable();
	}
	
	return true;
}

bool IrrlichtWidget::OnConfigure(GdkEventConfigure *event)
{
	printf("OnConfigure\n");
	if (window)
	{
		XWindowChanges changes;
		changes.width = get_width();
		changes.height = get_height();
		XConfigureWindow((Display*)device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display, window, CWWidth | CWHeight, &changes);
	}
	else if (device)
	{
		XWindowChanges changes;
		changes.width = get_width();
		changes.height = get_height();
		XConfigureWindow((Display*)device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display, device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Window, CWWidth | CWHeight, &changes);
		/*irr::core::dimension2d<int> size;
		size.Width = get_width();
		size.Height = get_height();
		device->getVideoDriver()->OnResize(size);*/
	}

}

irr::IrrlichtDevice *IrrlichtWidget::device = 0;
int IrrlichtWidget::usecount = 0;
GLXContext IrrlichtWidget::irrcontext = 0;

