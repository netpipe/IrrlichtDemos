
#include "Horde3DWidget.h"

#include <Horde3D.h>
#include <Horde3DUtils.h>

Horde3DWidget::Horde3DWidget() : Gtk::GL::DrawingArea()
{
	Glib::RefPtr<Gdk::GL::Config> glconfig;
	glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH | Gdk::GL::MODE_DOUBLE);
	add_events(Gdk::POINTER_MOTION_MASK);
	if (firstwidget)
	{
		set_gl_capability(glconfig, (const Glib::RefPtr<const Gdk::GL::Context>)firstwidget->get_gl_context());
	}
	else
	{
		set_gl_capability(glconfig);
	}
	//Glib::signal_timeout().connect( sigc::mem_fun(*this, &cHordeWidget::on_timeout), 10);
}
Horde3DWidget::~Horde3DWidget()
{
}

void Horde3DWidget::on_realize(void)
{
	Gtk::GL::DrawingArea::on_realize();
	if (!initialized)
	{
		Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
		glwindow->gl_begin(get_gl_context());
		initialize();
		glwindow->gl_end();
	}
}
bool Horde3DWidget::on_configure_event(GdkEventConfigure* event)
{
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	glwindow->gl_begin(get_gl_context());
	
	if (!initialized) initialize();
	
	glwindow->gl_end();
	return true;
}
bool Horde3DWidget::on_expose_event(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	glwindow->gl_begin(get_gl_context());
	
	glwindow->swap_buffers();
	glwindow->gl_end();
	return true;
}
bool Horde3DWidget::on_motion_notify_event(GdkEventMotion* event)
{
}

bool Horde3DWidget::initialize(void)
{
	Horde3D::init();
	Horde3D::resize(0,0,get_width(),get_height());
	//Horde3D::setupCameraView(cam,45.0f,(float)get_width()/get_height(),0.1f,1000.0f);
	printf("Initializing.\n");

	initialized = true;
	return true;
}

volatile bool Horde3DWidget::initialized = false;
Horde3DWidget *Horde3DWidget::firstwidget = 0;

