
#ifndef WXEDITOR_H
#define WXEDITOR_H

#include <irrlicht.h>
#include <wx/wx.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

typedef irr::SIrrlichtCreationParameters   wxIrrCreateParam;
typedef irr::video::E_DRIVER_TYPE         wxIrrDriverType;
typedef irr::video::SColor               wxIrrColor;
typedef irr::core::dimension2d<irr::u32>   wxIrrDimension2D;
typedef irr::core::vector3df            wxIrrVector3DFloat;
typedef irr::s32                     wxIrrS32;
typedef irr::video::E_MATERIAL_FLAG         wxIrrMaterialFlag;

class CIrrApp;
class CIrrDevice;
class CIrrWindow;
class CIrrFrame;

enum
{
   ID_Quit = 1,
   ID_About,
};

class CIrrApp : public wxApp
{
   public:
      virtual bool OnInit(void);
};

class CIrrDevice
{
   public:
      CIrrDevice(CIrrWindow *window, wxIrrDriverType type=irr::video::EDT_SOFTWARE, bool bResizeable=false);
      ~CIrrDevice();

      inline irr::IrrlichtDevice *GetIrrDevice(void) { return irrDevice; }
      inline scene::ISceneManager *GetSceneManager(void) { return irrDevice ? irrDevice->getSceneManager() : NULL; }
      inline video::IVideoDriver *GetVideoDriver(void) { return irrDevice ? irrDevice->getVideoDriver() : NULL; }

   protected:
      IrrlichtDevice *irrDevice;
};

class CIrrWindow : public wxWindow
{
   public:
      CIrrWindow(wxWindow *parent, wxWindowID id, long style=wxTAB_TRAVERSAL);
      ~CIrrWindow();

      void OnPaint(wxPaintEvent &event);
      void OnSize(wxSizeEvent &event);
      void OnTimer(wxTimerEvent &event);
      void OnMouseMove(wxMouseEvent &event);
      void OnMouseLeftDown(wxMouseEvent &event);
      void OnMouseLeftUp(wxMouseEvent &event);
      void OnMouseMiddleDown(wxMouseEvent &event);
      void OnMouseMiddleUp(wxMouseEvent &event);
      void OnMouseRightDown(wxMouseEvent &event);
      void OnMouseRightUp(wxMouseEvent &event);


      void OnSceneUpdate(void);

      inline scene::ISceneManager *GetSceneManager(void) { return irrDevice ? irrDevice->GetSceneManager() : NULL; }
      inline video::IVideoDriver *GetVideoDriver(void) { return irrDevice ? irrDevice->GetVideoDriver() : NULL; }

      inline void SetDevice(CIrrDevice *irrdevice) { irrDevice = irrdevice; }

   DECLARE_EVENT_TABLE()

   protected:
      CIrrDevice *irrDevice;
      scene::ICameraSceneNode *irrCameraCurrent;

   private:
      float fRatio;
      wxTimer m_Timer;
};

class CIrrFrame : public wxFrame
{
   public:
      CIrrFrame(const wxString &title, const wxPoint &pos, const wxSize &size, long style=wxDEFAULT_FRAME_STYLE);

      void OnQuit(wxCommandEvent &event);
      void OnAbout(wxCommandEvent &event);

      DECLARE_EVENT_TABLE()

   protected:
      CIrrDevice *device3D;
      scene::ICameraSceneNode *irrCam;
      CIrrWindow *window3D;
};

#endif //WXEDITOR_H
