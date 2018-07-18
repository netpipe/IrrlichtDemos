
#include "wxIrrHello.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
//#include <wx/gtk/win_gtk.h>
#include <gdk/gdkx.h>
typedef WXWidget     HWND;
using namespace irr;

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
// Tecan Disabled to work under linux

	GtkWidget* widget = window->GetHandle();
	gtk_widget_realize( widget );
XID xHandle = GDK_WINDOW_XWINDOW(widget->window);

   param.WindowId = reinterpret_cast<void*>(xHandle);
///   param.WindowId = reinterpret_cast<void *>( (HWND)window->GetHandle() );
   param.DriverType = type;

   irrDevice = irr::createDeviceEx(param);
   window->SetDevice( this );
   if ( irrDevice )
      irrDevice->setResizeAble( bResizeable );
}

CIrrDevice::~CIrrDevice()
{
   if ( irrDevice )
   {
      irrDevice->closeDevice();
      irrDevice->drop();
   }
}

wxIrrCamera *CIrrDevice::AddCamera(wxIrrNode *parent, core::vector3df position, core::vector3df lookat, wxIrrS32 id)
{
   wxIrrSceneManager *irrSceneMgr=GetSceneManager();

   if (irrSceneMgr)
      return (wxIrrCamera *)irrSceneMgr->addCameraSceneNode( parent, position, lookat );
   else
      return NULL;
}

///////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(CIrrWindow, wxWindow)
   EVT_PAINT(CIrrWindow::OnPaint)
   EVT_SIZE(CIrrWindow::OnSize)
   EVT_TIMER( wxID_ANY, CIrrWindow::OnTimer)
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
   wxPaintDC dc(wxWindow);

   if ( irrDevice )
   {
      wxIrrVideoDriver *driver=irrDevice->GetVideoDriver();
      wxIrrSceneManager *scenemgr=irrDevice->GetSceneManager();

      if ( driver )
      {
         driver->beginScene(true, true, wxIrrColor(110,110,0,0) );
         if ( scenemgr )
            scenemgr->drawAll();
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
      wxIrrVideoDriver *driver=irrDevice->GetVideoDriver();

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
   wxIrrSceneManager *scene=GetSceneManager();

   if ( scene )
   {
      wxIrrNode *cube=scene->getSceneNodeFromId( MyCube );

      if (cube)
      {
         core::vector3df yaw=cube->getRotation();

         cube->setRotation( core::vector3df(0.0f, yaw.Y+1.0f, 0.0f) );
      }
   }
}

wxIrrCamera *CIrrWindow::AddCamera(wxIrrNode *parent, core::vector3df position, core::vector3df lookat, wxIrrS32 id, bool bsetcurrent)
{
   wxIrrCamera *newcamera=NULL;

   if (irrDevice)
   {
      newcamera = irrDevice->AddCamera(parent, position, lookat, id);
      if ( bsetcurrent )
         irrCameraCurrent = newcamera;
   }
   return newcamera;
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

   SetMenuBar( menuBar );

   CreateStatusBar();
   SetStatusText( _T("Welcome to wxIrrlich!") );

   device3D = new CIrrDevice( window3D );
   window3D->AddCamera();

   wxIrrNode *cube=window3D->GetSceneManager()->addCubeSceneNode( 1.0f, 0, MyCube );

   cube->setMaterialTexture(0, window3D->GetVideoDriver()->getTexture("./crate.jpg"));
   cube->setMaterialFlag( irr::video::EMF_LIGHTING, false );
   cube->setPosition( wxIrrVector3DFloat(0.0f, 0.0f, 3.0f) );
}

void CIrrFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
   Close(TRUE);
}

void CIrrFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
   wxMessageBox( _T("This is a wxIrrlicht Hello world sample"),   _T("About Hello World"), wxOK|wxICON_INFORMATION, this );
}
