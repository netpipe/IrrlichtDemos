#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif


#include <sstream>
#include "CDDeviceFrame.h"
#include "Commands.h"
#include "wxPlayer.h"


BEGIN_EVENT_TABLE(CDDeviceFrame, wxFrame)
  EVT_MENU(DEVICE_NEW_DEVICE,     CDDeviceFrame::OnDeviceNewDevice)
  EVT_MENU(DEVICE_NEW_CDDEVICE,   CDDeviceFrame::OnDeviceNewCDDevice)
  EVT_MENU(DEVICE_NEW_MIDIDEVICE, CDDeviceFrame::OnDeviceNewMIDIDevice)
  EVT_MENU(HELP_ABOUT,            CDDeviceFrame::OnHelpAbout)

  EVT_BUTTON(CD_PLAY,         CDDeviceFrame::OnPlay)
  EVT_BUTTON(CD_STOP,         CDDeviceFrame::OnStop)
  EVT_BUTTON(CD_PAUSE,        CDDeviceFrame::OnPause)
  EVT_BUTTON(CD_RESUME,       CDDeviceFrame::OnResume)
  EVT_BUTTON(CD_OPEN_DOOR,    CDDeviceFrame::OnOpenDoor)
  EVT_BUTTON(CD_CLOSE_DOOR,   CDDeviceFrame::OnCloseDoor)
  EVT_BUTTON(CD_CHECK_STATUS, CDDeviceFrame::OnCheckStatus)
END_EVENT_TABLE()


CDDeviceFrame::CDDeviceFrame(audiere::CDDevicePtr device)
: wxFrame(0, -1, wxT("CD Device - ") + CStr2wxString(device->getName()),
          wxDefaultPosition, wxDefaultSize,
          wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL)
{
  m_device = device;

  wxMenu* deviceMenu = new wxMenu;
  deviceMenu->Append(DEVICE_NEW_DEVICE,           wxT("&New Device..."));
  deviceMenu->Append(DEVICE_NEW_CDDEVICE,         wxT("New C&D Device..."));
  deviceMenu->Append(DEVICE_NEW_MIDIDEVICE,       wxT("New &MIDI Device..."));

  wxMenu* helpMenu = new wxMenu;
  helpMenu->Append(HELP_ABOUT, wxT("&About..."));

  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append(deviceMenu, wxT("&Device"));
  menuBar->Append(helpMenu, wxT("&Help"));
  SetMenuBar(menuBar);

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  const int border = 4;

  wxBoxSizer* playSizer = new wxBoxSizer(wxHORIZONTAL);
  playSizer->Add(
    new wxButton(this, CD_PLAY, wxT("Play")),
    1, wxEXPAND | wxALL, 0);
  m_track = new wxTextCtrl(this, -1, wxT("1"));
  playSizer->Add(m_track, 1, wxADJUST_MINSIZE | wxALL, 0);
  sizer->Add(
    playSizer,
    1, wxEXPAND | wxALL, border);

  sizer->Add(
    new wxButton(this, CD_STOP, wxT("Stop")),
    1, wxEXPAND | wxALL, border);
  sizer->Add(
    new wxButton(this, CD_PAUSE, wxT("Pause")),
    1, wxEXPAND | wxALL, border);
  sizer->Add(
    new wxButton(this, CD_RESUME, wxT("Resume")),
    1, wxEXPAND | wxALL, border);

  wxBoxSizer* doorSizer = new wxBoxSizer(wxHORIZONTAL);
  doorSizer->Add(
    new wxButton(this, CD_OPEN_DOOR, wxT("Open Door")),
    1, wxEXPAND | wxALL, 0);
  doorSizer->Add(
    new wxButton(this, CD_CLOSE_DOOR, wxT("Close Door")),
    1, wxEXPAND | wxALL, 0);
  sizer->Add(
    doorSizer,
    1, wxEXPAND | wxALL, border);

  sizer->Add(
    new wxButton(this, CD_CHECK_STATUS, wxT("Check Status")),
    1, wxEXPAND | wxALL, border);
  
  SetAutoLayout(true);
  SetSizer(sizer);

  sizer->Fit(this);
  sizer->SetSizeHints(this);

  SetFocus();
}


void CDDeviceFrame::OnDeviceNewDevice(wxCommandEvent&) {
  wxGetApp().OnNewDevice(this);
}


void CDDeviceFrame::OnDeviceNewCDDevice(wxCommandEvent&) {
  wxGetApp().OnNewCDDevice(this);
}


void CDDeviceFrame::OnDeviceNewMIDIDevice(wxCommandEvent&) {
  wxGetApp().OnNewMIDIDevice(this);
}


void CDDeviceFrame::OnHelpAbout(wxCommandEvent&) {
  wxGetApp().ShowAboutDialog(this);
}


void CDDeviceFrame::OnPlay(wxCommandEvent&) {
  int track = wxAtoi(m_track->GetValue()) - 1;
  if (track < 0) {
    track = 0;
  } else if (track >= m_device->getTrackCount()) {
    return;
  }

  m_device->play(track);
}


void CDDeviceFrame::OnStop(wxCommandEvent&) {
  m_device->stop();
}


void CDDeviceFrame::OnPause(wxCommandEvent&) {
  m_device->pause();
}


void CDDeviceFrame::OnResume(wxCommandEvent&) {
  m_device->resume();
}


void CDDeviceFrame::OnOpenDoor(wxCommandEvent&) {
  m_device->openDoor();
}


void CDDeviceFrame::OnCloseDoor(wxCommandEvent&) {
  m_device->closeDoor();
}


void CDDeviceFrame::OnCheckStatus(wxCommandEvent&) {
  std::ostringstream message;

  if (m_device->isDoorOpen()) {
    message << "Door Open\n";
  } else {
    message << "Door Closed\n";
  }

  if (m_device->containsCD()) {
    message << "CD Present\n";
  } else {
    message << "CD Not Present\n";
  }

  message << m_device->getTrackCount() << " tracks\n";

  if (m_device->isPlaying()) {
    message << "Playing\n";
  } else {
    message << "Not Playing\n";
  }

  wxMessageBox(CStr2wxString(message.str().c_str()), wxT("Check Status"), wxOK, this);
}
