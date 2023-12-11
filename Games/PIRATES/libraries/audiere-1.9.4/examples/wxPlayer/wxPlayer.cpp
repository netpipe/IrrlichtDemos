#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif


#include <audiere.h>
#include "CDDeviceDialog.h"
#include "CDDeviceFrame.h"
#include "DeviceFrame.h"
#include "MIDIDeviceDialog.h"
#include "MIDIDeviceFrame.h"
#include "NewDeviceDialog.h"
#include "wxPlayer.h"


IMPLEMENT_APP(wxPlayer)


bool wxPlayer::OnInit() {
  bool result = (TryDevice("autodetect") || TryDevice("null"));
  if (!result) {
    wxMessageBox(wxT("Could not open output device"), wxT("wxPlayer"));
  }
  return result;
}


bool wxPlayer::TryDevice(const std::string& name, const std::string& parameters) {
  if (audiere::AudioDevicePtr device = audiere::OpenDevice(name.c_str(), parameters.c_str())) {
    (new DeviceFrame(device.get()))->Show(true);
    return true;
  } else {
    return false;
  }
}


void wxPlayer::OnNewDevice(wxWindow* parent) {
  NewDeviceDialog dialog(parent);
  if (dialog.ShowModal() == wxID_OK) {
    bool result = TryDevice(
      dialog.getDevice().c_str(),
      dialog.getParameters().c_str());
    if (!result) {
      wxMessageBox(wxT("Error opening new device"), wxT("New Device"),
                   wxOK | wxICON_ERROR, parent);
    }
  }
}


void wxPlayer::OnNewCDDevice(wxWindow* parent) {
  CDDeviceDialog dialog(parent);
  if (dialog.ShowModal() == wxID_OK) {
    audiere::CDDevicePtr device = audiere::OpenCDDevice(dialog.getName().c_str());
    if (!device) {
      wxMessageBox(wxT("Error opening CD device"), wxT("New CD Device"),
                   wxOK | wxICON_ERROR, parent);
      return;
    }

    (new CDDeviceFrame(device))->Show(true);
  }
}


void wxPlayer::OnNewMIDIDevice(wxWindow* parent) {
  MIDIDeviceDialog dialog(parent);
  if (dialog.ShowModal() == wxID_OK) {
    audiere::MIDIDevicePtr device = audiere::OpenMIDIDevice(dialog.getName().c_str());
    if (!device) {
      wxMessageBox(wxT("Error opening MIDI device"), wxT("New MIDI Device"),
                   wxOK | wxICON_ERROR, parent);
      return;
    }

    (new MIDIDeviceFrame(device))->Show(true);
  }
}


void wxPlayer::ShowAboutDialog(wxWindow* parent) {
  wxString message =
    wxT("wxPlayer\n")
    wxT("Copyright (c) Chad Austin 2002-2004\n\n")
    wxT("Audiere Version: ");
  message += CStr2wxString(audiere::GetVersion());
  wxMessageBox(message, wxT("About wxPlayer"), wxOK | wxCENTRE, parent);
}
