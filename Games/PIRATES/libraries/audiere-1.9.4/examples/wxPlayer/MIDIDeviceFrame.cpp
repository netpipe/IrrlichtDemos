#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif


#include "Commands.h"
#include "MIDIDeviceFrame.h"
#include "MIDIStreamFrame.h"
#include "wxPlayer.h"


BEGIN_EVENT_TABLE(MIDIDeviceFrame, wxMDIParentFrame)
  EVT_MENU(DEVICE_NEW_DEVICE,     MIDIDeviceFrame::OnDeviceNewDevice)
  EVT_MENU(DEVICE_NEW_CDDEVICE,   MIDIDeviceFrame::OnDeviceNewCDDevice)
  EVT_MENU(DEVICE_NEW_MIDIDEVICE, MIDIDeviceFrame::OnDeviceNewMIDIDevice)
  EVT_MENU(DEVICE_OPEN_SONG,      MIDIDeviceFrame::OnDeviceOpenSong)
  EVT_MENU(DEVICE_CLOSE_WINDOW,   MIDIDeviceFrame::OnDeviceCloseCurrentWindow)
  EVT_MENU(DEVICE_CLOSE,          MIDIDeviceFrame::OnDeviceClose)
  EVT_MENU(HELP_ABOUT,            MIDIDeviceFrame::OnHelpAbout)
END_EVENT_TABLE()


MIDIDeviceFrame::MIDIDeviceFrame(audiere::MIDIDevicePtr device)
: wxMDIParentFrame(0, -1, wxT("MIDI Device - ") + CStr2wxString(device->getName()),
                   wxDefaultPosition, wxSize(400, 500))
{
  m_device = device;

  wxMenu* deviceMenu = new wxMenu;
  deviceMenu->Append(DEVICE_NEW_DEVICE,     wxT("&New Device..."));
  deviceMenu->Append(DEVICE_NEW_CDDEVICE,   wxT("New C&D Device..."));
  deviceMenu->Append(DEVICE_NEW_MIDIDEVICE, wxT("New &MIDI Device..."));
  deviceMenu->AppendSeparator();
  deviceMenu->Append(DEVICE_OPEN_SONG,      wxT("Open &Song..."));
  deviceMenu->AppendSeparator();
  deviceMenu->Append(DEVICE_CLOSE_WINDOW,   wxT("Close C&urrent Window"));
  deviceMenu->Append(DEVICE_CLOSE,          wxT("&Close Device"));
  
  wxMenu* helpMenu = new wxMenu;
  helpMenu->Append(HELP_ABOUT, wxT("&About..."));

  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append(deviceMenu, wxT("&Device"));
  menuBar->Append(helpMenu,   wxT("&Help"));
  SetMenuBar(menuBar);

  SetFocus();
}


void MIDIDeviceFrame::OnDeviceNewDevice(wxCommandEvent&) {
  wxGetApp().OnNewDevice(this);
}


void MIDIDeviceFrame::OnDeviceNewCDDevice(wxCommandEvent&) {
  wxGetApp().OnNewCDDevice(this);
}


void MIDIDeviceFrame::OnDeviceNewMIDIDevice(wxCommandEvent&) {
  wxGetApp().OnNewMIDIDevice(this);
}


void MIDIDeviceFrame::OnDeviceOpenSong(wxCommandEvent&) {
  wxString file = wxFileSelector(
    wxT("Select a MIDI file"), wxT(""), wxT(""), wxT(""),
   wxT( "MIDI Files (*.midi,*.mid,*.rmi)|*.midi;*.mid;*.rmi|")
    wxT("All Files (*.*)|*.*"),
    wxOPEN, this);
  if (!file.empty()) {
    audiere::MIDIStreamPtr stream = m_device->openStream(wxString2CStr(file));
    if (!stream) {
      wxMessageBox(
        wxT("Could not open MIDI file: ") + file,
        wxT("Open MIDI File"), wxOK | wxCENTRE, this);
    }

    new MIDIStreamFrame(this, wxT("MIDI File - ") + file, stream);
  }
}


void MIDIDeviceFrame::OnDeviceCloseCurrentWindow(wxCommandEvent&) {
  wxMDIChildFrame* frame = GetActiveChild();
  if (frame) {
    frame->Close();
  }
}


void MIDIDeviceFrame::OnDeviceClose(wxCommandEvent&) {
  Close();
}


void MIDIDeviceFrame::OnHelpAbout(wxCommandEvent&) {
  wxGetApp().ShowAboutDialog(this);
}
