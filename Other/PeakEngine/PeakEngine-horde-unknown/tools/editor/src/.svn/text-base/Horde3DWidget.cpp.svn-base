
/*
Copyright (C) 2008   Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Horde3DWidget.h"
#include "InputReceiver.h"

#include <Horde3D.h>
#include <Horde3DUtils.h>

Horde3DWidget::Horde3DWidget() : Gtk::GL::DrawingArea()
{
	Glib::RefPtr<Gdk::GL::Config> glconfig;
	glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH | Gdk::GL::MODE_DOUBLE);
	add_events(Gdk::ALL_EVENTS_MASK);
	if (firstwidget)
	{
		set_gl_capability(glconfig, (const Glib::RefPtr<const Gdk::GL::Context>)firstwidget->get_gl_context());
	}
	else
	{
		set_gl_capability(glconfig);
		firstwidget = this;
	}
	//Glib::signal_timeout().connect( sigc::mem_fun(*this, &cHordeWidget::on_timeout), 10);
	
	camera = 0;
	input = 0;
}
Horde3DWidget::~Horde3DWidget()
{
}

void Horde3DWidget::setCamera(int camera)
{
	this->camera = camera;
}

void Horde3DWidget::setRootDirectory(std::string rootdir)
{
	Horde3DWidget::rootdir = rootdir;
}
void Horde3DWidget::loadResources(void)
{
	Horde3DUtils::loadResourcesFromDisk(rootdir.c_str());
}

void Horde3DWidget::update(void)
{
	Gtk::Widget::queue_draw();
}

void Horde3DWidget::on_realize(void)
{
	Gtk::GL::DrawingArea::on_realize();
	printf("Realize.\n");
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
	if (!glwindow->gl_begin(get_gl_context()))
	{
		printf("Rendering failed.\n");
		return false;
	}
	
	glClearColor(1.0, 0.0, 0.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	if (camera)
	{
		Horde3D::resize(0,0,get_width(),get_height());
		Horde3D::render(camera);
		Horde3DUtils::dumpMessages();
		printf("Rendered.\n");
	}
	if (glwindow->is_double_buffered())
		glwindow->swap_buffers();
	else
		glFlush();

	glwindow->gl_end();
	// Draw again instantly
	//Gtk::Widget::queue_draw();
	return true;
}
bool Horde3DWidget::on_motion_notify_event(GdkEventMotion* event)
{
	if (input) input->injectMouseMovement(event->x, event->y, event->state);
	state = event->state;
	return true;
}
bool Horde3DWidget::on_drag_motion(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time)
{
	if (input) input->injectMouseMovement(x, y, state);
	return true;
}
bool Horde3DWidget::on_button_press_event(GdkEventButton *button)
{
	return true;
}

bool Horde3DWidget::initialize(void)
{
	Horde3D::init();
	Horde3D::resize(0,0,get_width(),get_height());
	Horde3DUtils::setResourcePath(ResourceTypes::SceneGraph, "models");
	Horde3DUtils::setResourcePath(ResourceTypes::Geometry, "models");
	Horde3DUtils::setResourcePath(ResourceTypes::Animation, "models");
	Horde3DUtils::setResourcePath(ResourceTypes::Material, "materials");
	Horde3DUtils::setResourcePath(ResourceTypes::Code, "shaders");
	Horde3DUtils::setResourcePath(ResourceTypes::Shader, "shaders");
	Horde3DUtils::setResourcePath(ResourceTypes::Texture2D, "textures");
	Horde3DUtils::setResourcePath(ResourceTypes::TextureCube, "textures");
	Horde3DUtils::setResourcePath(ResourceTypes::Effect, "effects");
	Horde3DUtils::setResourcePath(ResourceTypes::Pipeline, "pipelines");

	printf("Initializing.\n");

	initialized = true;
	return true;
}

volatile bool Horde3DWidget::initialized = false;
Horde3DWidget *Horde3DWidget::firstwidget = 0;
std::string Horde3DWidget::rootdir;

