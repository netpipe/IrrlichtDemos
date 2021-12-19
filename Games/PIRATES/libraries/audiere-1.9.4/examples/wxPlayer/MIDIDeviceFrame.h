#ifndef ADR_MIDI_DEVICE_FRAME_H
#define ADR_MIDI_DEVICE_FRAME_H


#include <audiere.h>
#include "wx.h"


class MIDIDeviceFrame : public wxMDIParentFrame {
public:
  MIDIDeviceFrame(audiere::MIDIDevicePtr device);

  void OnDeviceNewDevice(wxCommandEvent&);
  void OnDeviceNewCDDevice(wxCommandEvent&);
  void OnDeviceNewMIDIDevice(wxCommandEvent&);
  void OnDeviceOpenSong(wxCommandEvent&);
  void OnDeviceCloseCurrentWindow(wxCommandEvent&);
  void OnDeviceClose(wxCommandEvent&);
  void OnHelpAbout(wxCommandEvent&);

private:
  audiere::MIDIDevicePtr m_device;

  DECLARE_EVENT_TABLE()
};


#endif
