#include "Commands.h"
#include "MIDIStreamFrame.h"


BEGIN_EVENT_TABLE(MIDIStreamFrame, wxMDIChildFrame)
  EVT_BUTTON(MIDI_PLAY,  MIDIStreamFrame::OnPlay)
  EVT_BUTTON(MIDI_PAUSE, MIDIStreamFrame::OnPause)
  EVT_BUTTON(MIDI_STOP,  MIDIStreamFrame::OnStop)

  EVT_CHECKBOX(MIDI_REPEAT, MIDIStreamFrame::OnRepeat)

  EVT_COMMAND_SCROLL(MIDI_POS, MIDIStreamFrame::OnChangePos)

  EVT_TIMER(MIDI_UPDATE, MIDIStreamFrame::OnUpdateStatus)
END_EVENT_TABLE()


MIDIStreamFrame::MIDIStreamFrame(
  wxMDIParentFrame* parent,
  const wxString& title,
  audiere::MIDIStreamPtr stream)
: wxMDIChildFrame(parent, -1, title)
{
  m_stream = stream;

  int f = wxEXPAND | wxALL;

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(new wxButton(this, MIDI_PLAY, wxT("Play")),   1, f, 4);
  sizer->Add(new wxButton(this, MIDI_PAUSE, wxT("Pause")), 1, f, 4);
  sizer->Add(new wxButton(this, MIDI_STOP, wxT("Stop")),   1, f, 4);
  m_is_playing_label = new wxStaticText(this, -1, wxT(""));
  sizer->Add(m_is_playing_label,                      1, f, 4);
  m_repeating        = new wxCheckBox(this, MIDI_REPEAT, wxT("Repeating"));
  sizer->Add(m_repeating,                             1, f, 4);
  m_length_pos_label = new wxStaticText(this, -1, wxT(""));
  sizer->Add(m_length_pos_label,                      1, f, 4);
  m_pos              = new wxSlider(this, MIDI_POS, 0, 0, 1000);
  sizer->Add(m_pos,                                   1, f, 4);

  SetAutoLayout(true);
  SetSizer(sizer);

  sizer->Fit(this);
  sizer->SetSizeHints(this);

  // Create a timer to update the current position.
  m_timer = new wxTimer(this, MIDI_UPDATE);
  m_timer->Start(100);

  UpdateStatus();
}


MIDIStreamFrame::~MIDIStreamFrame() {
  m_timer->Stop();
  delete m_timer;
}


void MIDIStreamFrame::OnPlay(wxCommandEvent&) {
  m_stream->play();
}


void MIDIStreamFrame::OnPause(wxCommandEvent&) {
  m_stream->pause();
}


void MIDIStreamFrame::OnStop(wxCommandEvent&) {
  m_stream->stop();
}


void MIDIStreamFrame::OnRepeat(wxCommandEvent&) {
  m_stream->setRepeat(m_repeating->GetValue());
}


void MIDIStreamFrame::OnChangePos(wxScrollEvent&) {
  float pos = m_pos->GetValue() / 1000.0f;
  int len = m_stream->getLength();
  m_stream->setPosition(int(pos * len));
}

void MIDIStreamFrame::OnUpdateStatus(wxTimerEvent&) {
	UpdateStatus();
}


void MIDIStreamFrame::UpdateStatus() {
  m_is_playing_label->SetLabel(m_stream->isPlaying() ? wxT("Playing") : wxT("Stopped"));

  int pos = m_stream->getPosition();
  int len = m_stream->getLength();
  
  wxString label;
  label.Printf(wxT("Pos/Len:  %d / %d"), pos, len);
  m_length_pos_label->SetLabel(label);

  float l = (len == 0 ? 0.0f : float(pos) / len);
  m_pos->SetValue(int(l * 1000));
}
