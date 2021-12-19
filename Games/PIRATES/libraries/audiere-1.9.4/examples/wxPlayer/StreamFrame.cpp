#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif


#include <sstream>
#include "Commands.h"
#include "EditLoopPointsDialog.h"
#include "StreamFrame.h"
#include "TagsDialog.h"


BEGIN_EVENT_TABLE(StreamFrame, wxMDIChildFrame)
  EVT_BUTTON(STREAM_PLAY,  StreamFrame::OnPlay)
  EVT_BUTTON(STREAM_STOP,  StreamFrame::OnStop)
  EVT_BUTTON(STREAM_RESET, StreamFrame::OnReset)
  EVT_BUTTON(STREAM_VIEW_INFO, StreamFrame::OnViewInfo)
  EVT_BUTTON(STREAM_EDIT_LOOP_POINTS, StreamFrame::OnEditLoopPoints)
  EVT_BUTTON(STREAM_VIEW_TAGS, StreamFrame::OnViewTags)

  EVT_CHECKBOX(STREAM_REPEAT, StreamFrame::OnRepeat)

  EVT_COMMAND_SCROLL(STREAM_VOLUME, StreamFrame::OnChangeVolume)
  EVT_COMMAND_SCROLL(STREAM_PAN,    StreamFrame::OnChangePan)
  EVT_COMMAND_SCROLL(STREAM_PITCH,  StreamFrame::OnChangePitch)
  EVT_COMMAND_SCROLL(STREAM_POS,    StreamFrame::OnChangePos)

  EVT_TIMER(STREAM_UPDATE, StreamFrame::OnUpdateStatus)
END_EVENT_TABLE()


StreamFrame::StreamFrame(
  wxMDIParentFrame* parent,
  const wxString& title,
  audiere::OutputStreamPtr stream,
  audiere::SampleSourcePtr source,
  audiere::LoopPointSourcePtr loop_source)
: wxMDIChildFrame(parent, -1, title)
{
  m_stream = stream;
  m_source = source;
  m_loop_source = loop_source;
  m_stream_is_seekable = stream->isSeekable();
  m_stream_length = stream->getLength();

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(
    new wxButton(this, STREAM_PLAY, wxT("Play")),
    1, wxEXPAND | wxALL, 4);
  sizer->Add(
    new wxButton(this, STREAM_STOP, wxT("Stop")),
    1, wxEXPAND | wxALL, 4);
  sizer->Add(
    new wxButton(this, STREAM_RESET, wxT("Reset")),
    1, wxEXPAND | wxALL, 4);
  m_playing_label = new wxStaticText(this, -1, wxT("Stopped"));
  sizer->Add(m_playing_label,    1, wxEXPAND | wxALL, 4);
  m_repeating = new wxCheckBox(this, STREAM_REPEAT, wxT("Repeating"));
  sizer->Add(m_repeating,        1, wxEXPAND | wxALL, 4);
  m_vpp_label = new wxStaticText(this, -1, wxT("Volume - Pan - Pitch"));
  sizer->Add(m_vpp_label,        1, wxEXPAND | wxALL, 4);
  m_volume = new wxSlider(this, STREAM_VOLUME, 100, 0,  100);
  sizer->Add(m_volume,           1, wxEXPAND | wxALL, 4);
  m_pan    = new wxSlider(this, STREAM_PAN,    0, -100, 100);
  sizer->Add(m_pan,              1, wxEXPAND | wxALL, 4);
  m_pitch  = new wxSlider(this, STREAM_PITCH,  100, 50, 200);
  sizer->Add(m_pitch,            1, wxEXPAND | wxALL, 4);
  m_length_pos_label = new wxStaticText(this, -1, wxT("Length - Pos"));
  sizer->Add(m_length_pos_label, 1, wxEXPAND | wxALL, 4);
  m_pos    = new wxSlider(this, STREAM_POS,    0, 0, 1000);
  sizer->Add(m_pos,              1, wxEXPAND | wxALL, 4);

  wxButton* infoButton = new wxButton(
    this, STREAM_VIEW_INFO, wxT("View Info"));
  sizer->Add(infoButton, 1, wxEXPAND | wxALL, 4);
  
  wxButton* editButton = new wxButton(
    this, STREAM_EDIT_LOOP_POINTS, wxT("Edit Loop Points..."));
  sizer->Add(editButton, 1, wxEXPAND | wxALL, 4);
  if (!loop_source) {
    editButton->Enable(false);
  }

  wxButton* tagsButton = new wxButton(
    this, STREAM_VIEW_TAGS, wxT("View Tags..."));
  sizer->Add(tagsButton, 1, wxEXPAND | wxALL, 4);

  if (!m_stream_is_seekable) {
    m_pos->Enable(false);
  }

  UpdateVPPLabel();
  UpdateLengthPosLabel();

  SetAutoLayout(true);
  SetSizer(sizer);

  sizer->Fit(this);
  sizer->SetSizeHints(this);

  // create a timer to update the current position
  m_timer = new wxTimer(this, STREAM_UPDATE);
  m_timer->Start(100);
}


StreamFrame::~StreamFrame() {
  m_timer->Stop();
  delete m_timer;
}


void StreamFrame::OnPlay(wxCommandEvent&) {
  m_stream->play();
}


void StreamFrame::OnStop(wxCommandEvent&) {
  m_stream->stop();
}


void StreamFrame::OnReset(wxCommandEvent&) {
  m_stream->reset();
}


void StreamFrame::OnViewInfo(wxCommandEvent&) {
  int channel_count, sample_rate;
  audiere::SampleFormat format;
  m_source->getFormat(channel_count, sample_rate, format);

  const char* format_name;
  if (format == audiere::SF_U8) {
    format_name = "SF_U8";
  } else if (format == audiere::SF_S16) {
    format_name = "SF_S16";
  } else {
    format_name = "Unknown";
  }

  std::stringstream ss;
  ss << "Channel Count: " << channel_count << std::endl;
  ss << "Sample Rate: " << sample_rate << std::endl;
  ss << "Format: " << format_name << std::endl;
  wxMessageBox(CStr2wxString(ss.str().c_str()), wxT("Stream Info"), wxOK, this);
}


void StreamFrame::OnEditLoopPoints(wxCommandEvent&) {
  EditLoopPointsDialog(this, m_loop_source.get()).ShowModal();
}


void StreamFrame::OnViewTags(wxCommandEvent&) {
  TagsDialog(this, m_source.get()).ShowModal();
}


void StreamFrame::OnRepeat(wxCommandEvent&) {
  m_stream->setRepeat(m_repeating->GetValue());
}


void StreamFrame::OnChangeVolume(wxScrollEvent&) {
  m_stream->setVolume(m_volume->GetValue() / 100.0f);
  UpdateVPPLabel();
}


void StreamFrame::OnChangePan(wxScrollEvent&) {
  m_stream->setPan(m_pan->GetValue() / 100.0f);
  UpdateVPPLabel();
}


void StreamFrame::OnChangePitch(wxScrollEvent&) {
  m_stream->setPitchShift(m_pitch->GetValue() / 100.0f);
  UpdateVPPLabel();
}


void StreamFrame::OnChangePos(wxScrollEvent&) {
  float pos = float(m_pos->GetValue()) / 1000;
  m_stream->setPosition(int(pos * m_stream_length));
}


void StreamFrame::OnUpdateStatus(wxTimerEvent&) {
  if (m_stream_is_seekable) {
    float pos = float(m_stream->getPosition()) / m_stream_length;
    m_pos->SetValue(int(1000 * pos));
  }
  UpdateLengthPosLabel();

  // update the playing label
  m_playing_label->SetLabel(m_stream->isPlaying() ? wxT("Playing") : wxT("Stopped"));
}


void StreamFrame::UpdateVPPLabel() {
  wxString label;
  label.Printf(
    wxT("Vol: %1.2f  Pan: %1.2f  Pitch: %1.2f"),
    m_volume->GetValue() / 100.0f,
    m_pan->GetValue() / 100.0f,
    m_pitch->GetValue() / 100.0f);
  m_vpp_label->SetLabel(label);
}


void StreamFrame::UpdateLengthPosLabel() {
  if (m_stream_is_seekable) {
    wxString label;
    label.Printf(
      wxT("Pos/Len: %d / %d"),
      m_stream->getPosition(),
      m_stream_length);
    m_length_pos_label->SetLabel(label);
  } else {
    m_length_pos_label->SetLabel(wxT("Unseekable Stream"));
  }
}
