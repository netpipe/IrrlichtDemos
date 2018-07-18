#include "renderWindow.h"
#include <irrlicht.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
//#include <wx/gtk/win_gtk.h>
#include "win_gtk.h"
#include <gdk/gdkx.h>
#include <wx/window.h>
BEGIN_EVENT_TABLE(renderWindow, wxGLCanvas)
EVT_PAINT(renderWindow::OnPaint)
EVT_SIZE(renderWindow::OnResize)
END_EVENT_TABLE()

renderWindow::renderWindow(wxFrame *parent, const wxPoint &point, const wxSize &size)
: wxGLCanvas(parent, wxID_ANY, NULL, point, size)
{		wxString msg;
	//	msg.Printf(wxT("Could not open the irrlicht device"));
//    		wxMessageBox(msg, wxT("Error"), wxOK | wxICON_ERROR, parent);
	SIrrlichtCreationParameters params;

		GtkWidget* widget = this->GetHandle();
	gtk_widget_realize( widget );
XID xHandle = GDK_WINDOW_XWINDOW(widget->window);

   params.WindowId = reinterpret_cast<void*>(xHandle);
	params.AntiAlias = false;
	params.Bits = 32;
	params.DriverType = EDT_SOFTWARE;
	params.Fullscreen = false;
	params.Stencilbuffer = false;
	params.Vsync = false;
	params.WindowSize = dimension2du(size.x, size.y);
//	params.WindowId = this->GetHandle();

	device = createDeviceEx(params);
//	if (device == NULL)
//	{
//		wxString msg;
//		msg.Printf(wxT("Could not open the irrlicht device"));
//
//		wxMessageBox(msg, wxT("Error"), wxOK | wxICON_ERROR, parent);
//
//		deviceActive = false;
//	}
//	else
//	{
//		device->setResizeAble(true);

		driver = device->getVideoDriver();
		smgr = device->getSceneManager();
		guienv = device->getGUIEnvironment();

		cam = smgr->addCameraSceneNode(0, vector3df(10, 10, 10), vector3df(0, 0, 0));

		sceneNodes.push_back(smgr->addCubeSceneNode());

		deviceActive = true;
//	}
}

renderWindow::~renderWindow()
{
	device->drop();
}

void renderWindow::OnResize(wxSizeEvent &event)
{
//	int w, h;
//	this->GetSize(&w, &h);
//	cam->setAspectRatio(float(w)/h);
//	driver->OnResize(dimension2d<s32>(w, h));
}

void renderWindow::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);

    SetCurrent();

	if (deviceActive)
	{
		driver->beginScene(true, true, SColor(0, 0, 0, 0));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}
}

void renderWindow::saveScene(const std::wstring &filename)
{
	std::string temp(filename.begin(), filename.end());
	smgr->saveScene(temp.c_str());
}

void renderWindow::loadScene(const std::wstring &filename)
{
	std::string temp(filename.begin(), filename.end());
	smgr->loadScene(temp.c_str());
}
