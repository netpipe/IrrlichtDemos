#include "MIDIDeviceDialog.h"


BEGIN_EVENT_TABLE(MIDIDeviceDialog, wxDialog)
  EVT_BUTTON(-1, MIDIDeviceDialog::OnButton)
END_EVENT_TABLE()


MIDIDeviceDialog::MIDIDeviceDialog(wxWindow* parent)
: wxDialog(parent, -1, wxString(wxT("New MIDI Device")))
{
  // size of text controls
  static const wxSize size(300, 22);

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  m_name   = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, size);
  sizer->Add(m_name,      0, wxALIGN_CENTER | wxALL, 5);

  wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  m_ok     = new wxButton(this, wxID_OK, wxT("OK"));
  buttonSizer->Add(m_ok,     0, wxALIGN_CENTER | wxALL, 5);
  m_cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
  buttonSizer->Add(m_cancel, 0, wxALIGN_CENTER | wxALL, 5);
  sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);

  SetAutoLayout(true);
  SetSizer(sizer);

  m_name->SetFocus();
  m_ok->SetDefault();

  sizer->Fit(this);
  sizer->SetSizeHints(this);
}


void MIDIDeviceDialog::OnButton(wxCommandEvent& event) {
  if (event.GetEventObject() == m_ok) {
    m_name_str = wxString2CStr(m_name->GetValue());
    EndModal(wxID_OK);
  } else if (event.GetEventObject() == m_cancel) {
    EndModal(wxID_CANCEL);
  }
}
