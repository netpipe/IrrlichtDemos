#ifndef ADR_CD_DEVICE_FRAME_H
#define ADR_CD_DEVICE_FRAME_H


#include <audiere.h>
#include "wx.h"


class CDDeviceFrame : public wxFrame {
public:
  CDDeviceFrame(audiere::CDDevicePtr device);

private:
  void OnDeviceNewDevice(wxCommandEvent&);
  void OnDeviceNewCDDevice(wxCommandEvent&);
  void OnDeviceNewMIDIDevice(wxCommandEvent&);
  void OnHelpAbout(wxCommandEvent&);

  void OnPlay(wxCommandEvent&);
  void OnStop(wxCommandEvent&);
  void OnPause(wxCommandEvent&);
  void OnResume(wxCommandEvent&);
  void OnOpenDoor(wxCommandEvent&);
  void OnCloseDoor(wxCommandEvent&);
  void OnCheckStatus(wxCommandEvent&);

  audiere::CDDevicePtr m_device;

  wxTextCtrl* m_track;

  DECLARE_EVENT_TABLE()
};


#endif
