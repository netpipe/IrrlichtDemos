#ifndef ADR_SOUND_EFFECT_FRAME_H
#define ADR_SOUND_EFFECT_FRAME_H


#include <audiere.h>
#include "wx.h"


class SoundEffectFrame : public wxMDIChildFrame {
public:
  SoundEffectFrame(wxMDIParentFrame* parent, const wxString& title, audiere::SoundEffectPtr effect);

  void OnPlay(wxCommandEvent&);
  void OnStop(wxCommandEvent&);
  void OnChangeVolume(wxScrollEvent&);
  void OnChangePan(wxScrollEvent&);
  void OnChangePitch(wxScrollEvent&);
  void UpdateVPPLabel();

private:
  audiere::SoundEffectPtr m_effect;

  wxStaticText* m_vpp_label;
  wxSlider*     m_volume;
  wxSlider*     m_pan;
  wxSlider*     m_pitch;

  DECLARE_EVENT_TABLE()
};


#endif
