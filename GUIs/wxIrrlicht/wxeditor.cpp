#include "wxeditor.h"
#include <irrlicht.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>
//#include <wx/gtk/win_gtk.h>
#include <gdk/gdkx.h>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif


typedef WXWidget     HWND;


IMPLEMENT_APP(CIrrApp)

bool CIrrApp::OnInit()
{
   CIrrFrame *frame = new CIrrFrame( _T("wxIrrlicht: Hello World"), wxPoint(50,50), wxSize(550,540) );
   frame->Show(TRUE);
   SetTopWindow(frame);
   return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

CIrrDevice::CIrrDevice(CIrrWindow *window, wxIrrDriverType type, bool bResizeable)
{
   wxIrrCreateParam param;

	GtkWidget* widget = window->GetHandle();
	gtk_widget_realize( widget );
XID xHandle = GDK_WINDOW_XWINDOW(widget->window);

   param.WindowId = reinterpret_cast<void*>(xHandle);
   param.DriverType = type;
   irrDevice = irr::createDeviceEx(param);
   window->SetDevice( this );
 if ( irrDevice )
      irrDevice->setResizable( bResizeable );

   window->CaptureMouse();

}

CIrrDevice::~CIrrDevice()
{
   if ( irrDevice )
   {
      irrDevice->closeDevice();
      irrDevice->drop();
   }
}


///////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(CIrrWindow, wxWindow)
   EVT_PAINT(CIrrWindow::OnPaint)
   EVT_SIZE(CIrrWindow::OnSize)
   EVT_TIMER( wxID_ANY, CIrrWindow::OnTimer)
   EVT_MOTION( CIrrWindow::OnMouseMove)
   EVT_LEFT_DOWN(CIrrWindow::OnMouseLeftDown)
   EVT_LEFT_UP(CIrrWindow::OnMouseLeftUp)
   EVT_MIDDLE_DOWN(CIrrWindow::OnMouseMiddleDown)
   EVT_MIDDLE_UP(CIrrWindow::OnMouseMiddleUp)
   EVT_RIGHT_DOWN(CIrrWindow::OnMouseRightDown)
   EVT_RIGHT_UP(CIrrWindow::OnMouseRightUp)
END_EVENT_TABLE()

CIrrWindow::CIrrWindow(wxWindow *parent, wxWindowID id, long style) : wxWindow( parent, id, wxDefaultPosition, wxDefaultSize, style ), m_Timer(this)
{
   irrDevice = NULL;
   irrCameraCurrent = NULL;
   m_Timer.Start(1000/60);
}

CIrrWindow::~CIrrWindow()
{
}

void CIrrWindow::OnPaint(wxPaintEvent &event)
{
   wxPaintDC dc(wxWindow);  //added wxWindow to this to build with wx 2.9

   if ( irrDevice )
   {
      video::IVideoDriver *driver=irrDevice->GetVideoDriver();
      scene::ISceneManager *smgr=irrDevice->GetSceneManager();

      if ( driver )
      {
         driver->beginScene(true, true, wxIrrColor(0,0,0,0) );
         if ( smgr )
            smgr->drawAll();
         driver->endScene();
      }
   }
   event.Skip();
}

void CIrrWindow::OnSize(wxSizeEvent &event)
{
   float w=GetClientSize().GetWidth();
   float h=GetClientSize().GetHeight();

   fRatio = (float)w/(float)h;
   if ( irrDevice )
   {
      video::IVideoDriver *driver=irrDevice->GetVideoDriver();

      if ( driver )
      {
         driver->OnResize( wxIrrDimension2D(w, h) );
         if ( irrCameraCurrent )
            irrCameraCurrent->setAspectRatio( fRatio );
         if ( !m_Timer.IsRunning() )
            Update();
      }
   }
   event.Skip();
}

void CIrrWindow::OnTimer(wxTimerEvent &event)
{
   OnSceneUpdate();
   Refresh( false );
   event.Skip();
}

//
// Do the update of your scene here
//
void CIrrWindow::OnSceneUpdate(void)
{
   if ( irrDevice )
   {
      video::IVideoDriver *driver=irrDevice->GetVideoDriver();
      scene::ISceneManager *smgr=irrDevice->GetSceneManager();

      if ( driver )
      {
         driver->beginScene(true, true, wxIrrColor(0,0,0,0) );
         if ( smgr )
            smgr->drawAll();
         driver->endScene();
      }
   }

}


void CIrrWindow::OnMouseMove(wxMouseEvent &event)
{
    irr::SEvent irrEvent;
    irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

    irrEvent.MouseInput.X = event.GetX();
    irrEvent.MouseInput.Y = event.GetY();
    irrEvent.MouseInput.Wheel = 0.0f; // Zero is better than undefined

    irrDevice->GetIrrDevice()->postEventFromUser( irrEvent );
}

void CIrrWindow::OnMouseLeftDown(wxMouseEvent &event)
{
    irr::SEvent irrEvent;
    irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

   irrEvent.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
    irrDevice->GetIrrDevice()->postEventFromUser( irrEvent );

}

void CIrrWindow::OnMouseLeftUp(wxMouseEvent &event)
{
    irr::SEvent irrEvent;
    irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

   irrEvent.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
    irrDevice->GetIrrDevice()->postEventFromUser( irrEvent );
}

void CIrrWindow::OnMouseMiddleDown(wxMouseEvent &event)
{
}

void CIrrWindow::OnMouseMiddleUp(wxMouseEvent &event)
{
}

void CIrrWindow::OnMouseRightDown(wxMouseEvent &event)
{
    irr::SEvent irrEvent;
    irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

   irrEvent.MouseInput.Event = irr::EMIE_RMOUSE_PRESSED_DOWN;
    irrDevice->GetIrrDevice()->postEventFromUser( irrEvent );

}

void CIrrWindow::OnMouseRightUp(wxMouseEvent &event)
{
    irr::SEvent irrEvent;
    irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

   irrEvent.MouseInput.Event = irr::EMIE_RMOUSE_LEFT_UP;
    irrDevice->GetIrrDevice()->postEventFromUser( irrEvent );
}


///////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(CIrrFrame, wxFrame)
   EVT_MENU(ID_Quit, CIrrFrame::OnQuit)
   EVT_MENU(ID_About, CIrrFrame::OnAbout)
END_EVENT_TABLE()

CIrrFrame::CIrrFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame((wxFrame *)NULL, -1, title, pos, size, style)
{
   this->SetSizeHints( wxDefaultSize, wxDefaultSize );

   wxBoxSizer* bSizer1;
   bSizer1 = new wxBoxSizer( wxVERTICAL );

   window3D = new CIrrWindow( this, wxID_ANY );
   bSizer1->Add( window3D, 1, wxEXPAND, 5 );

   this->SetSizer( bSizer1 );
   this->Layout();

   wxMenu *menuFile = new wxMenu;

   menuFile->Append( ID_About, _T("&About...") );
   menuFile->AppendSeparator();
   menuFile->Append( ID_Quit, _T("E&xit") );

   wxMenuBar *menuBar = new wxMenuBar;
   menuBar->Append( menuFile, _T("&File") );

//wxButton *button = new wxButton(window3D, 1, "Select", wxDefaultPosition);


   SetMenuBar( menuBar );

   CreateStatusBar();
   SetStatusText( _T("Welcome to wxIrrlich!") );

   device3D = new CIrrDevice( window3D );

   // setup a simple 3d scene
   scene::ISceneManager *smgr = window3D->GetSceneManager();
   video::IVideoDriver *driver = window3D->GetVideoDriver();

   //Set up test scene
   device3D->GetIrrDevice()->getFileSystem()->addZipFileArchive("map-20kdm2.pk3");

   scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
   if (mesh)
   {
      scene::ISceneNode* node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
      node->setPosition(core::vector3df(-1300,-144,-1249));


   scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS();
   camera->setPosition(core::vector3df(0,0,0));
   }

}

void CIrrFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
   Close(TRUE);
}

void CIrrFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
   wxMessageBox( _T("This is a wxIrrlicht Hello world sample"),   _T("About Hello World"), wxOK|wxICON_INFORMATION, this );
}

