#ifndef ADR_EDIT_LOOP_POINTS_DIALOG_H
#define ADR_EDIT_LOOP_POINTS_DIALOG_H


#include <audiere.h>
#include "wx.h"


class EditLoopPointsDialog : public wxDialog {
public:
  EditLoopPointsDialog(
    wxWindow* parent,
    audiere::LoopPointSourcePtr loop_source);

private:
  void OnButton(wxCommandEvent& event);
  void OnClose(wxCloseEvent& event);

  void resetFields();
  void refreshLoopPoints();


  audiere::LoopPointSourcePtr m_loop_source;

  wxListBox*  m_points;
  wxTextCtrl* m_location;
  wxTextCtrl* m_target;
  wxTextCtrl* m_loop_count;
  wxButton*   m_add;
  wxButton*   m_remove;
  wxButton*   m_close;

  DECLARE_EVENT_TABLE()
};


#endif
