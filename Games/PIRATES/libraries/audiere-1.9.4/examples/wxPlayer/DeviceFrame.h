#ifndef ADR_DEVICE_FRAME_H
#define ADR_DEVICE_FRAME_H


#include <audiere.h>
#include "wx.h"


class DeviceFrame : public wxMDIParentFrame {
public:
  DeviceFrame(audiere::AudioDevicePtr device);
  
  void OnDeviceNewDevice(wxCommandEvent&);
  void OnDeviceNewCDDevice(wxCommandEvent&);
  void OnDeviceNewMIDIDevice(wxCommandEvent&);
  wxString GetSoundFile();
  void OnDeviceOpenStream(wxCommandEvent&);
  void OnDeviceOpenSound(wxCommandEvent&);
  void OnDeviceCreateTone(wxCommandEvent&);
  void OnDeviceCreateSquareWave(wxCommandEvent&);
  void OnDeviceCreateWhiteNoise(wxCommandEvent&);
  void OnDeviceCreatePinkNoise(wxCommandEvent&);
  void OnDeviceOpenSingleEffect(wxCommandEvent&);
  void OnDeviceOpenMultipleEffect(wxCommandEvent&);
  void DoOpenEffect(audiere::SoundEffectType type, wxString typestring);
  void OnDeviceCloseCurrentWindow(wxCommandEvent&);
  void OnDeviceClose(wxCommandEvent&);
  void OnHelpAbout(wxCommandEvent&);

private:
  audiere::AudioDevicePtr m_device;

  DECLARE_EVENT_TABLE()
};


#endif
